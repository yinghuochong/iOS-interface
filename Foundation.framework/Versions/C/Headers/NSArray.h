/*	NSArray.h
	Copyright (c) 1994-2016, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSEnumerator.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSObjCRuntime.h>

@class NSData, NSIndexSet, NSString, NSURL;

/****************	Immutable Array		****************/

NS_ASSUME_NONNULL_BEGIN

//__covariant - 协变性，子类型可以强转到父类型（里氏替换原则）
//__contravariant - 逆变性，父类型可以强转到子类型（WTF?）
/*
 比起使用系统的泛型容器，更好玩的是自定义一个泛型类，目前这里还没什么文档，但拦不住我们写测试代码，假设我们要自定义一个 Stack 容器类：
 
 @interface Stack<ObjectType> : NSObject
 - (void)pushObject:(ObjectType)object;
 - (ObjectType)popObject;
 @property (nonatomic, readonly) NSArray<ObjectType> *allObjects;
 @end
 这个 ObjectType 是传入类型的 placeholder，它只能在 @interface 上定义（类声明、类扩展、Category），如果你喜欢用 T 表示也 ok，这个类型在 @interface 和 @end 区间的作用域有效，可以把它作为入参、出参、甚至内部 NSArray 属性的泛型类型，应该说一切都是符合预期的。我们还可以给 ObjectType 增加类型限制，比如：
 
 // 只接受 NSNumber * 的泛型
 @interface Stack<ObjectType: NSNumber *> : NSObject
 // 只接受满足 NSCopying 协议的泛型
 @interface Stack<ObjectType: id<NSCopying>> : NSObject
 若什么都不加，表示接受任意类型 ( id )；当类型不满足时编译器将产生 error。
 实例化一个 Stack，一切工作正常：
 
 
 
 对于多参数的泛型，用逗号隔开，其他都一样，可以参考 NSDictionary 的头文件。
 
 ref : http://blog.sunnyxx.com/2015/06/12/objc-new-features-in-2015/
 */


@interface NSArray<__covariant ObjectType> : NSObject <NSCopying, NSMutableCopying, NSSecureCoding, NSFastEnumeration>

@property (readonly) NSUInteger count;
- (ObjectType)objectAtIndex:(NSUInteger)index;
/*
 为什么要用NS_DESIGNATED_INITIALIZER
 Objective-C 中主要通过NS_DESIGNATED_INITIALIZER宏来实现指定构造器的。这里之所以要用这个宏，往往是想告诉调用者要用这个方法去初始化（构造）类对象。
 怎样避免使用NS_DESIGNATED_INITIALIZER产生的警告
 如果子类指定了新的初始化器，那么在这个初始化器内部必须调用父类的Designated Initializer。并且需要重写父类的Designated Initializer，将其指向子类新的初始化器。
 如果定义NS_DESIGNATED_INITIALIZER，大多是不想让调用者调用父类的初始化函数，只希望通过该类指定的初始化进行初始化，这时候就可以用NS_UNAVAILABLE宏。
 如果调用者使用init 初始化，编译器就会给出一个编译错误。使用NS_UNAVAILABLE后，就不需要在.m中重写父类初始化函数了。如果要允许调用者使用init就需要在.m中重写父类的初始化函数，如上提到的，否则就会报警告。
 避免使用new
 如果使用new来创建对象的话，即使init被声明为NS_UNAVAILABLE，也不会收到编译器的警告和错误提示了。
 ref: http://blog.csdn.net/zcube/article/details/51657417
 */

- (instancetype)init NS_DESIGNATED_INITIALIZER;
//从C数组进行初始化  cnt是数量

- (instancetype)initWithObjects:(const ObjectType _Nonnull [_Nullable])objects count:(NSUInteger)cnt NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

@end

@interface NSArray<ObjectType> (NSExtendedArray)
//拷贝一个新的数组，并把anObject放到最后，anObject不能为空
- (NSArray<ObjectType> *)arrayByAddingObject:(ObjectType)anObject;
- (NSArray<ObjectType> *)arrayByAddingObjectsFromArray:(NSArray<ObjectType> *)otherArray;
//根据separator链接成一个字符串
- (NSString *)componentsJoinedByString:(NSString *)separator;
//是否存在对象
- (BOOL)containsObject:(ObjectType)anObject;
//会用固定格式将数组打印出来
@property (readonly, copy) NSString *description;
- (NSString *)descriptionWithLocale:(nullable id)locale;
- (NSString *)descriptionWithLocale:(nullable id)locale indent:(NSUInteger)level;
//返回两个数组里第一个想同的对象，如果没有返回nil
- (nullable ObjectType)firstObjectCommonWithArray:(NSArray<ObjectType> *)otherArray;
//获取range里的对象，放到objects，objects是一个c数组的buffer
- (void)getObjects:(ObjectType __unsafe_unretained [])objects range:(NSRange)range NS_SWIFT_UNAVAILABLE("Use 'subarrayWithRange()' instead");
//返回指定对象的索引，返回找到的第一个，未找到返回NSNotFound,调用isEqual进行对比
- (NSUInteger)indexOfObject:(ObjectType)anObject;
//在指定range里找，返回找到的第一个
- (NSUInteger)indexOfObject:(ObjectType)anObject inRange:(NSRange)range;
//和上面两个类似，对比地址是否相同
- (NSUInteger)indexOfObjectIdenticalTo:(ObjectType)anObject;
//通过isEqual对比两个数组是否相同
- (NSUInteger)indexOfObjectIdenticalTo:(ObjectType)anObject inRange:(NSRange)range;
- (BOOL)isEqualToArray:(NSArray<ObjectType> *)otherArray;
@property (nullable, nonatomic, readonly) ObjectType firstObject NS_AVAILABLE(10_6, 4_0);
@property (nullable, nonatomic, readonly) ObjectType lastObject;
//返回正序和倒序的枚举器
- (NSEnumerator<ObjectType> *)objectEnumerator;
- (NSEnumerator<ObjectType> *)reverseObjectEnumerator;
//生成一个NSData 的对象，主要是用来进行数组的排序
@property (readonly, copy) NSData *sortedArrayHint;
//根据自定义函数进行排序，返回的新数组只是拿着对象，不是拷贝
- (NSArray<ObjectType> *)sortedArrayUsingFunction:(NSInteger (NS_NOESCAPE *)(ObjectType, ObjectType, void * _Nullable))comparator context:(nullable void *)context;
//可以提供hint来加速排序，如果数组是大致排好序的，如果元素总数是N，增加或者删除了P个元素，P远小于N，可以重用之前排好序的N个元素，使用Merge排序来对P和N进行排序，调用sortedArrayHint获得hint，应在当原始数组被排好序之后立即获得
- (NSArray<ObjectType> *)sortedArrayUsingFunction:(NSInteger (NS_NOESCAPE *)(ObjectType, ObjectType, void * _Nullable))comparator context:(nullable void *)context hint:(nullable NSData *)hint;
//使用comparator对数组中元素进行比较，可以调用数组中元素对应的比较方法，比如NSString的caseInsensitiveCompare ，也可以在自己的类中实现自己的比较方法
//ref : https://my.oschina.net/pengloo53/blog/173810
- (NSArray<ObjectType> *)sortedArrayUsingSelector:(SEL)comparator;
//取一个子数组 range不能越界
- (NSArray<ObjectType> *)subarrayWithRange:(NSRange)range;
//数组中只能是 NSString, NSData, NSArray, or NSDictionary ，如果路径包含~ ,需要调用stringByExpandingTildeInPath来处理一下
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile;
- (BOOL)writeToURL:(NSURL *)url atomically:(BOOL)atomically;
//让所有对象都执行一个方法，aSelector不能为空
- (void)makeObjectsPerformSelector:(SEL)aSelector NS_SWIFT_UNAVAILABLE("Use enumerateObjectsUsingBlock: or a for loop instead");
- (void)makeObjectsPerformSelector:(SEL)aSelector withObject:(nullable id)argument NS_SWIFT_UNAVAILABLE("Use enumerateObjectsUsingBlock: or a for loop instead");
//根据一组index生成一个数组，index不能越界 indexes 不能为空
- (NSArray<ObjectType> *)objectsAtIndexes:(NSIndexSet *)indexes;
//根据下标获取对象，类似objectAtIndex ，不应该直接调用，id value = array[3];时调用
- (ObjectType)objectAtIndexedSubscript:(NSUInteger)idx NS_AVAILABLE(10_8, 6_0);
//根据一个block来枚举对象， block不能为空，设置stop为YES停止遍历
- (void)enumerateObjectsUsingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))block NS_AVAILABLE(10_6, 4_0);
//可以使用opts参数来表明是否逆序遍历
- (void)enumerateObjectsWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))block NS_AVAILABLE(10_6, 4_0);
- (void)enumerateObjectsAtIndexes:(NSIndexSet *)s options:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))block NS_AVAILABLE(10_6, 4_0);
//predicate为对对象进行测试的block，如果通过测试在block中返回YES，函数返回第一个通过测试的索引，如果没有返回NSNotFound
- (NSUInteger)indexOfObjectPassingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);
- (NSUInteger)indexOfObjectWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);
- (NSUInteger)indexOfObjectAtIndexes:(NSIndexSet *)s options:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);
//类似上面的方法，返回一个通过测试的索引集合
- (NSIndexSet *)indexesOfObjectsPassingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);
- (NSIndexSet *)indexesOfObjectsWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);
- (NSIndexSet *)indexesOfObjectsAtIndexes:(NSIndexSet *)s options:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);
//根据cmptr进行排序，可以指定是不是稳定排序
- (NSArray<ObjectType> *)sortedArrayUsingComparator:(NSComparator NS_NOESCAPE)cmptr NS_AVAILABLE(10_6, 4_0);
- (NSArray<ObjectType> *)sortedArrayWithOptions:(NSSortOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmptr NS_AVAILABLE(10_6, 4_0);

typedef NS_OPTIONS(NSUInteger, NSBinarySearchingOptions) {
	NSBinarySearchingFirstEqual = (1UL << 8),
	NSBinarySearchingLastEqual = (1UL << 9),
	NSBinarySearchingInsertionIndex = (1UL << 10),
};
/*返回指定排序数组里某个对象的索引，
 obj是需要搜索的对象，不能为空
 r不能越界，不能同时指定 NSBinarySearchingFirstEqual 和 NSBinarySearchingLastEqual 其他的可以组合
 opts必须是排序好的，
 
 如果未指定NSBinarySearchingInsertionIndex 则：
 如果对象找到了，并且未指定 First 和 last，则返回找到的索引
 First 返回第一个找到的，last返回最后一个找到的，未找到返回NSNotFound
 
 如果制定了NSBinarySearchingInsertionIndex 则返回应该在哪里插入这个对象：
 如未指定First和Last，则返回一个比找到的对象索引大的一个索引
 first返回比第一个找到的小的，last返回比第最后一个找到的大的
 如果未找到，返回最近的比他大的对象的索引，或者数组的最后一个索引
 */
- (NSUInteger)indexOfObject:(ObjectType)obj inSortedRange:(NSRange)r options:(NSBinarySearchingOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmp NS_AVAILABLE(10_6, 4_0); // binary search

@end

@interface NSArray<ObjectType> (NSArrayCreation)

+ (instancetype)array;//创建并返回一个空数组
+ (instancetype)arrayWithObject:(ObjectType)anObject;//返回只包含一个对象的数组
//根据c数组生成一个数组，cnt不能为负，且应小于objects的数量
+ (instancetype)arrayWithObjects:(const ObjectType [])objects count:(NSUInteger)cnt;
//NS_REQUIRES_NIL_TERMINATION  attribute((sentinel)) 告知编译器 需要一个结尾的参数,告知编译器参数的列表已经到最后一个不要再继续执行下去了,这里需要以nil结尾  http://www.jianshu.com/p/f61ff5e72b72
+ (instancetype)arrayWithObjects:(ObjectType)firstObj, ... NS_REQUIRES_NIL_TERMINATION;
+ (instancetype)arrayWithArray:(NSArray<ObjectType> *)array;//通过另一个数组来初始化这个数组

- (instancetype)initWithObjects:(ObjectType)firstObj, ... NS_REQUIRES_NIL_TERMINATION;
- (instancetype)initWithArray:(NSArray<ObjectType> *)array;
//flag为YES会拷贝一份生成新的对象，数组中的对象应该都应该实现 NSCopying 协议
- (instancetype)initWithArray:(NSArray<ObjectType> *)array copyItems:(BOOL)flag;
//https://developer.apple.com/library/prerelease/content/documentation/Cocoa/Conceptual/PropertyLists/Introduction/Introduction.html#//apple_ref/doc/uid/10000048i
+ (nullable NSArray<ObjectType> *)arrayWithContentsOfFile:(NSString *)path;
+ (nullable NSArray<ObjectType> *)arrayWithContentsOfURL:(NSURL *)url;
- (nullable NSArray<ObjectType> *)initWithContentsOfFile:(NSString *)path;
- (nullable NSArray<ObjectType> *)initWithContentsOfURL:(NSURL *)url;

@end

@interface NSArray<ObjectType> (NSDeprecated)

/* This method is unsafe because it could potentially cause buffer overruns. You should use -getObjects:range: instead.
*/
- (void)getObjects:(ObjectType __unsafe_unretained [])objects NS_SWIFT_UNAVAILABLE("Use 'as [AnyObject]' instead");

@end

/****************	Mutable Array		****************/

@interface NSMutableArray<ObjectType> : NSArray<ObjectType>

- (void)addObject:(ObjectType)anObject;//anObject不能为空
- (void)insertObject:(ObjectType)anObject atIndex:(NSUInteger)index;
- (void)removeLastObject;
- (void)removeObjectAtIndex:(NSUInteger)index;
- (void)replaceObjectAtIndex:(NSUInteger)index withObject:(ObjectType)anObject;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCapacity:(NSUInteger)numItems NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

@end

@interface NSMutableArray<ObjectType> (NSExtendedMutableArray)
//把otherArray加到数组后面
- (void)addObjectsFromArray:(NSArray<ObjectType> *)otherArray;
- (void)exchangeObjectAtIndex:(NSUInteger)idx1 withObjectAtIndex:(NSUInteger)idx2;
- (void)removeAllObjects;
//移除range里所有等于anObject对象的对象，通过isEqual来比较
- (void)removeObject:(ObjectType)anObject inRange:(NSRange)range;
- (void)removeObject:(ObjectType)anObject;
//通过地址进行比较
- (void)removeObjectIdenticalTo:(ObjectType)anObject inRange:(NSRange)range;
- (void)removeObjectIdenticalTo:(ObjectType)anObject;
//从某个位置开始移除指定数量的元素
- (void)removeObjectsFromIndices:(NSUInteger *)indices numIndices:(NSUInteger)cnt NS_DEPRECATED(10_0, 10_6, 2_0, 4_0);
//移除数组中的对象
- (void)removeObjectsInArray:(NSArray<ObjectType> *)otherArray;
- (void)removeObjectsInRange:(NSRange)range;
- (void)replaceObjectsInRange:(NSRange)range withObjectsFromArray:(NSArray<ObjectType> *)otherArray range:(NSRange)otherRange;
- (void)replaceObjectsInRange:(NSRange)range withObjectsFromArray:(NSArray<ObjectType> *)otherArray;
- (void)setArray:(NSArray<ObjectType> *)otherArray;
- (void)sortUsingFunction:(NSInteger (NS_NOESCAPE *)(ObjectType,  ObjectType, void * _Nullable))compare context:(nullable void *)context;
- (void)sortUsingSelector:(SEL)comparator;
//根据索引列表和对象列表添加，第一个对象根据第一个索引添加，对象书和indexes数需要一样，
- (void)insertObjects:(NSArray<ObjectType> *)objects atIndexes:(NSIndexSet *)indexes;
- (void)removeObjectsAtIndexes:(NSIndexSet *)indexes;
- (void)replaceObjectsAtIndexes:(NSIndexSet *)indexes withObjects:(NSArray<ObjectType> *)objects;
/*
 mutableArray[3] = @"someValue"; // equivalent to [mutableArray replaceObjectAtIndex:3 withObject:@"someValue"]
 */
- (void)setObject:(ObjectType)obj atIndexedSubscript:(NSUInteger)idx NS_AVAILABLE(10_8, 6_0);

- (void)sortUsingComparator:(NSComparator NS_NOESCAPE)cmptr NS_AVAILABLE(10_6, 4_0);
- (void)sortWithOptions:(NSSortOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmptr NS_AVAILABLE(10_6, 4_0);

@end

@interface NSMutableArray<ObjectType> (NSMutableArrayCreation)

+ (instancetype)arrayWithCapacity:(NSUInteger)numItems;

+ (nullable NSMutableArray<ObjectType> *)arrayWithContentsOfFile:(NSString *)path;
+ (nullable NSMutableArray<ObjectType> *)arrayWithContentsOfURL:(NSURL *)url;
- (nullable NSMutableArray<ObjectType> *)initWithContentsOfFile:(NSString *)path;
- (nullable NSMutableArray<ObjectType> *)initWithContentsOfURL:(NSURL *)url;

@end

NS_ASSUME_NONNULL_END
