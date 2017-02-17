/*	NSDictionary.h
	Copyright (c) 1994-2016, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSEnumerator.h>

@class NSArray<ObjectType>, NSSet<ObjectType>, NSString, NSURL;

/****************	Immutable Dictionary	****************/

NS_ASSUME_NONNULL_BEGIN

@interface NSDictionary<__covariant KeyType, __covariant ObjectType> : NSObject <NSCopying, NSMutableCopying, NSSecureCoding, NSFastEnumeration>
//元素数量
@property (readonly) NSUInteger count;
- (nullable ObjectType)objectForKey:(KeyType)aKey;
//key枚举器
- (NSEnumerator<KeyType> *)keyEnumerator;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
#if TARGET_OS_WIN32
- (instancetype)initWithObjects:(const ObjectType _Nonnull [_Nullable])objects forKeys:(const KeyType _Nonnull [_Nullable])keys count:(NSUInteger)cnt;
#else
- (instancetype)initWithObjects:(const ObjectType _Nonnull [_Nullable])objects forKeys:(const KeyType <NSCopying> _Nonnull [_Nullable])keys count:(NSUInteger)cnt NS_DESIGNATED_INITIALIZER;
#endif
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

@end

@interface NSDictionary<KeyType, ObjectType> (NSExtendedDictionary)
//顺序不确定
@property (readonly, copy) NSArray<KeyType> *allKeys;
//获取所有value等于anObject的key的数组
- (NSArray<KeyType> *)allKeysForObject:(ObjectType)anObject;
@property (readonly, copy) NSArray<ObjectType> *allValues;
/*
 {
 1 = hello;
 2 = world;
 }
 */
@property (readonly, copy) NSString *description;
/*"1" = "hello";
 "2" = "world";
 */
@property (readonly, copy) NSString *descriptionInStringsFileFormat;//会有引号引起来
- (NSString *)descriptionWithLocale:(nullable id)locale;
//level是 tab的数量
- (NSString *)descriptionWithLocale:(nullable id)locale indent:(NSUInteger)level;
- (BOOL)isEqualToDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (NSEnumerator<ObjectType> *)objectEnumerator;//对象枚举器
//根据keys声称已values的数组，如果没找到用marker占位   marker不能为空
- (NSArray<ObjectType> *)objectsForKeys:(NSArray<KeyType> *)keys notFoundMarker:(ObjectType)marker;
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile;
- (BOOL)writeToURL:(NSURL *)url atomically:(BOOL)atomically; // the atomically flag is ignored if url of a type that cannot be written atomically.
//对key进行排序
- (NSArray<KeyType> *)keysSortedByValueUsingSelector:(SEL)comparator;
// count refers to the number of elements in the dictionary
- (void)getObjects:(ObjectType __unsafe_unretained [])objects andKeys:(KeyType __unsafe_unretained [])keys count:(NSUInteger)count NS_AVAILABLE(10_7, 5_0) NS_SWIFT_UNAVAILABLE("Use 'allKeys' and/or 'allValues' instead");

- (nullable ObjectType)objectForKeyedSubscript:(KeyType)key NS_AVAILABLE(10_8, 6_0);

- (void)enumerateKeysAndObjectsUsingBlock:(void (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))block NS_AVAILABLE(10_6, 4_0);
- (void)enumerateKeysAndObjectsWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))block NS_AVAILABLE(10_6, 4_0);

- (NSArray<KeyType> *)keysSortedByValueUsingComparator:(NSComparator NS_NOESCAPE)cmptr NS_AVAILABLE(10_6, 4_0);
- (NSArray<KeyType> *)keysSortedByValueWithOptions:(NSSortOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmptr NS_AVAILABLE(10_6, 4_0);
//把满足某些条件的key找出来
- (NSSet<KeyType> *)keysOfEntriesPassingTest:(BOOL (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);
- (NSSet<KeyType> *)keysOfEntriesWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))predicate NS_AVAILABLE(10_6, 4_0);

@end

@interface NSDictionary<KeyType, ObjectType> (NSDeprecated)
/// This method is unsafe because it could potentially cause buffer overruns. You should use -getObjects:andKeys:count:
- (void)getObjects:(ObjectType __unsafe_unretained [])objects andKeys:(KeyType __unsafe_unretained [])keys  NS_SWIFT_UNAVAILABLE("Use 'allKeys' and/or 'allValues' instead");
@end

@interface NSDictionary<KeyType, ObjectType> (NSDictionaryCreation)

+ (instancetype)dictionary;
//value,key,value,key ,nil
+ (instancetype)dictionaryWithObject:(ObjectType)object forKey:(KeyType <NSCopying>)key;
#if TARGET_OS_WIN32
+ (instancetype)dictionaryWithObjects:(const ObjectType [])objects forKeys:(const KeyType [])keys count:(NSUInteger)cnt;
#else
+ (instancetype)dictionaryWithObjects:(const ObjectType [])objects forKeys:(const KeyType <NSCopying> [])keys count:(NSUInteger)cnt;
#endif

+ (instancetype)dictionaryWithObjectsAndKeys:(id)firstObject, ... NS_REQUIRES_NIL_TERMINATION NS_SWIFT_UNAVAILABLE("Use dictionary literals instead");

+ (instancetype)dictionaryWithDictionary:(NSDictionary<KeyType, ObjectType> *)dict;
+ (instancetype)dictionaryWithObjects:(NSArray<ObjectType> *)objects forKeys:(NSArray<KeyType <NSCopying>> *)keys;

- (instancetype)initWithObjectsAndKeys:(id)firstObject, ... NS_REQUIRES_NIL_TERMINATION;
- (instancetype)initWithDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (instancetype)initWithDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary copyItems:(BOOL)flag;
- (instancetype)initWithObjects:(NSArray<ObjectType> *)objects forKeys:(NSArray<KeyType <NSCopying>> *)keys;

+ (nullable NSDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfFile:(NSString *)path;
+ (nullable NSDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfURL:(NSURL *)url;
- (nullable NSDictionary<KeyType, ObjectType> *)initWithContentsOfFile:(NSString *)path;
- (nullable NSDictionary<KeyType, ObjectType> *)initWithContentsOfURL:(NSURL *)url;

@end

/****************	Mutable Dictionary	****************/

@interface NSMutableDictionary<KeyType, ObjectType> : NSDictionary<KeyType, ObjectType>
//Does nothing if aKey does not exist.
- (void)removeObjectForKey:(KeyType)aKey;
//value不能为nil,如果需要nil，传NSNull
- (void)setObject:(ObjectType)anObject forKey:(KeyType <NSCopying>)aKey;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCapacity:(NSUInteger)numItems NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

@end

@interface NSMutableDictionary<KeyType, ObjectType> (NSExtendedMutableDictionary)
//从其他dic添加，如果已经有了，会覆盖，key必须实现nscopying协议，会拷贝一份，value会retain
- (void)addEntriesFromDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (void)removeAllObjects;
- (void)removeObjectsForKeys:(NSArray<KeyType> *)keyArray;
- (void)setDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (void)setObject:(nullable ObjectType)obj forKeyedSubscript:(KeyType <NSCopying>)key NS_AVAILABLE(10_8, 6_0);

@end

@interface NSMutableDictionary<KeyType, ObjectType> (NSMutableDictionaryCreation)

+ (instancetype)dictionaryWithCapacity:(NSUInteger)numItems;

+ (nullable NSMutableDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfFile:(NSString *)path;
+ (nullable NSMutableDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfURL:(NSURL *)url;
- (nullable NSMutableDictionary<KeyType, ObjectType> *)initWithContentsOfFile:(NSString *)path;
- (nullable NSMutableDictionary<KeyType, ObjectType> *)initWithContentsOfURL:(NSURL *)url;

@end

@interface NSDictionary<KeyType, ObjectType> (NSSharedKeySetDictionary)

/*  Use this method to create a key set to pass to +dictionaryWithSharedKeySet:.
 The keys are copied from the array and must be copyable.
 If the array parameter is nil or not an NSArray, an exception is thrown.
 If the array of keys is empty, an empty key set is returned.
 The array of keys may contain duplicates, which are ignored (it is undefined which object of each duplicate pair is used).
 As for any usage of hashing, is recommended that the keys have a well-distributed implementation of -hash, and the hash codes must satisfy the hash/isEqual: invariant.
 Keys with duplicate hash codes are allowed, but will cause lower performance and increase memory usage.
 */
/* 获取公共的key */
+ (id)sharedKeySetForKeys:(NSArray<KeyType <NSCopying>> *)keys NS_AVAILABLE(10_8, 6_0);

@end

@interface NSMutableDictionary<KeyType, ObjectType> (NSSharedKeySetDictionary)

/*  Create a mutable dictionary which is optimized for dealing with a known set of keys.
 Keys that are not in the key set can still be set into the dictionary, but that usage is not optimal.
 As with any dictionary, the keys must be copyable.
 If keyset is nil, an exception is thrown.
 If keyset is not an object returned by +sharedKeySetForKeys:, an exception is thrown.
 */
+ (NSMutableDictionary<KeyType, ObjectType> *)dictionaryWithSharedKeySet:(id)keyset NS_AVAILABLE(10_8, 6_0);

@end

NS_ASSUME_NONNULL_END
