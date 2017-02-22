/*	NSDate.h
	Copyright (c) 1994-2016, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSNotification.h>

@class NSString;

NS_ASSUME_NONNULL_BEGIN
//Notification的名字定义可以用  NSNotificationName
FOUNDATION_EXPORT NSNotificationName const NSSystemClockDidChangeNotification NS_AVAILABLE(10_6, 4_0);

typedef double NSTimeInterval;
//从1979年到2001年1月1号的时间戳
#define NSTimeIntervalSince1970  978307200.0

@interface NSDate : NSObject <NSCopying, NSSecureCoding>
//2001年1月1号到现在的时间戳
@property (readonly) NSTimeInterval timeIntervalSinceReferenceDate;

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithTimeIntervalSinceReferenceDate:(NSTimeInterval)ti NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

@end

@interface NSDate (NSExtendedDate)
//调用者到anotherDate之间的时间戳
- (NSTimeInterval)timeIntervalSinceDate:(NSDate *)anotherDate;
@property (readonly) NSTimeInterval timeIntervalSinceNow;
@property (readonly) NSTimeInterval timeIntervalSince1970;

- (id)addTimeInterval:(NSTimeInterval)seconds NS_DEPRECATED(10_0, 10_6, 2_0, 4_0);
- (instancetype)dateByAddingTimeInterval:(NSTimeInterval)ti NS_AVAILABLE(10_6, 2_0);
//返回调用者和anotherDate之间较早的日期
- (NSDate *)earlierDate:(NSDate *)anotherDate;
- (NSDate *)laterDate:(NSDate *)anotherDate;
- (NSComparisonResult)compare:(NSDate *)other;
- (BOOL)isEqualToDate:(NSDate *)otherDate;

@property (readonly, copy) NSString *description;
- (NSString *)descriptionWithLocale:(nullable id)locale;

#if FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(8)
@property (class, readonly) NSTimeInterval timeIntervalSinceReferenceDate;
#endif

@end

@interface NSDate (NSDateCreation)

+ (instancetype)date;
//从现在开始多少秒后的日期
+ (instancetype)dateWithTimeIntervalSinceNow:(NSTimeInterval)secs;
//从timeIntervalSinceReferenceDate开始多少秒后的日期
+ (instancetype)dateWithTimeIntervalSinceReferenceDate:(NSTimeInterval)ti;
+ (instancetype)dateWithTimeIntervalSince1970:(NSTimeInterval)secs;
+ (instancetype)dateWithTimeInterval:(NSTimeInterval)secsToBeAdded sinceDate:(NSDate *)date;

#if FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(8)
@property (class, readonly, copy) NSDate *distantFuture;
@property (class, readonly, copy) NSDate *distantPast;
#endif

- (instancetype)initWithTimeIntervalSinceNow:(NSTimeInterval)secs;
- (instancetype)initWithTimeIntervalSince1970:(NSTimeInterval)secs;
- (instancetype)initWithTimeInterval:(NSTimeInterval)secsToBeAdded sinceDate:(NSDate *)date;

@end

NS_ASSUME_NONNULL_END
