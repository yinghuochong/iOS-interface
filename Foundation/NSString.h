/*	NSString.h
	Copyright (c) 1994-2016, Apple Inc. All rights reserved.
*/

//NSString 直接继承自  NSObject，是一种基础类型； NSMutableString 继承自NSString，是NSString的可变类型。

typedef unsigned short unichar;  //这里自定义了 unichar  类型，其实就是一个  unsigned short，占2个字节

#import <limits.h> //定义各种 最大值最小值宏 的头文件
#import <Foundation/NSObject.h>
#import <Foundation/NSRange.h>
#import <stdarg.h> //定义参数处理的头文件

//KeyType      ObjectType   ？
@class NSData, NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSCharacterSet, NSURL, NSError, NSLocale;

NS_ASSUME_NONNULL_BEGIN

/* 下面的选项用于各种 搜索/查找 和比较方法
*/
typedef NS_OPTIONS(NSUInteger, NSStringCompareOptions) {
    NSCaseInsensitiveSearch = 1,  //大小写不敏感的搜索
    NSLiteralSearch = 2,		/* 严格按字符对比搜索 */
    NSBackwardsSearch = 4,		/* 从后往前搜索 */
    NSAnchoredSearch = 8,		/* 仅搜索开头(或者结尾, if NSBackwardsSearch) */
    NSNumericSearch = 64,		/*  10.2 新增; 字符串中的数字用数值进行比较，比如  Foo2.txt < Foo7.txt < Foo25.txt; 仅用于比较方法，不用于查找 */
    NSDiacriticInsensitiveSearch NS_ENUM_AVAILABLE(10_5, 2_0) = 128, /* 忽略变音符号(o-umlaut == o) */
    NSWidthInsensitiveSearch NS_ENUM_AVAILABLE(10_5, 2_0) = 256, /* 宽度不敏感 ('a' == UFF41) */
    NSForcedOrderingSearch NS_ENUM_AVAILABLE(10_5, 2_0) = 512, /* 如果指定，如果字符串相等但不严格相等比较时强制返回 NSOrderedAscending 或 NSOrderedDescending ,针对于稳定的排序使用 (e.g. "aaa" > "AAA" with NSCaseInsensitiveSearch specified) */
    NSRegularExpressionSearch NS_ENUM_AVAILABLE(10_7, 3_2) = 1024    /* 仅用于 rangeOfString:..., stringByReplacingOccurrencesOfString:..., and replaceOccurrencesOfString:... ; 搜索的字符串按ICU-compatible规则的正则表达式对待   ，如果设置了，除NSCaseInsensitiveSearch 和 NSAnchoredSearch 之外的选项都不可用 */
};

//encoding枚举值
/* Note that in addition to the values explicitly listed below, NSStringEncoding supports encodings provided by CFString.
See CFStringEncodingExt.h for a list of these encodings.
See CFString.h for functions which convert between NSStringEncoding and CFStringEncoding.
*/
typedef NSUInteger NSStringEncoding;
NS_ENUM(NSStringEncoding) {
    NSASCIIStringEncoding = 1,		/* 0..127 only */
    NSNEXTSTEPStringEncoding = 2,
    NSJapaneseEUCStringEncoding = 3,
    NSUTF8StringEncoding = 4,
    NSISOLatin1StringEncoding = 5,
    NSSymbolStringEncoding = 6,
    NSNonLossyASCIIStringEncoding = 7,
    NSShiftJISStringEncoding = 8,          /* kCFStringEncodingDOSJapanese */
    NSISOLatin2StringEncoding = 9,
    NSUnicodeStringEncoding = 10,
    NSWindowsCP1251StringEncoding = 11,    /* Cyrillic; same as AdobeStandardCyrillic */
    NSWindowsCP1252StringEncoding = 12,    /* WinLatin1 */
    NSWindowsCP1253StringEncoding = 13,    /* Greek */
    NSWindowsCP1254StringEncoding = 14,    /* Turkish */
    NSWindowsCP1250StringEncoding = 15,    /* WinLatin2 */
    NSISO2022JPStringEncoding = 21,        /* ISO 2022 Japanese encoding for e-mail */
    NSMacOSRomanStringEncoding = 30,

    NSUTF16StringEncoding = NSUnicodeStringEncoding,      /* An alias for NSUnicodeStringEncoding */

    NSUTF16BigEndianStringEncoding = 0x90000100,          /* NSUTF16StringEncoding encoding with explicit endianness specified */
    NSUTF16LittleEndianStringEncoding = 0x94000100,       /* NSUTF16StringEncoding encoding with explicit endianness specified */

    NSUTF32StringEncoding = 0x8c000100,                   
    NSUTF32BigEndianStringEncoding = 0x98000100,          /* NSUTF32StringEncoding encoding with explicit endianness specified */
    NSUTF32LittleEndianStringEncoding = 0x9c000100        /* NSUTF32StringEncoding encoding with explicit endianness specified */
};

    //ecoding转换枚举值
typedef NS_OPTIONS(NSUInteger, NSStringEncodingConversionOptions) {
    NSStringEncodingConversionAllowLossy = 1,
    NSStringEncodingConversionExternalRepresentation = 2
};


@interface NSString : NSObject <NSCopying, NSMutableCopying, NSSecureCoding>

#pragma mark *** String funnel methods ***

//NSString最小化的子类只需要实现下面两个初始化方法，出于性能考虑，建议实现getCharacters:range:
@property (readonly) NSUInteger length;//使用UTF-16的长度，是以单个字符计数，所以不能用来决定是否可见，或者显示的长度
- (unichar)characterAtIndex:(NSUInteger)index;//NSString 第index位置的字符，index越界会导致异常

/* The initializers available to subclasses. See further below for additional init methods.
*/
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

@end

@interface NSString (NSStringExtensionMethods)

#pragma mark *** Substrings ***

/* 为了避免截断像Emoji这样的字符序列，可以用下面的使用方式：
 [str substringFromIndex:[str rangeOfComposedCharacterSequenceAtIndex:index].location]
 [str substringToIndex:NSMaxRange([str rangeOfComposedCharacterSequenceAtIndex:index])]
 [str substringWithRange:[str rangeOfComposedCharacterSequencesForRange:range]
 */
- (NSString *)substringFromIndex:(NSUInteger)from;
- (NSString *)substringToIndex:(NSUInteger)to;
- (NSString *)substringWithRange:(NSRange)range;  //from 和 to range都不能超过调用者的范围

- (void)getCharacters:(unichar *)buffer range:(NSRange)range;   //获取调用者range范围的buffer，buffer必须足够大，range不能超过调用者的大小


#pragma mark *** String comparison and equality ***

/* 在下面的方法中，range 参数代表调用者子range，
 range不会用到被比较的参数上，比如[@"AB" compare:@"ABC" options:0 range:NSMakeRange(0,1)] 是比较 "A" to "ABC", 而不是 "A" to "A",
 会返回 NSOrderedAscending.
 指定一个超过调用者的大小的range会导致一个异常
 
 参数string不能为空，为空会导致不可预测的结果，并且未来可能会修改这个方法
 */
- (NSComparisonResult)compare:(NSString *)string;
- (NSComparisonResult)compare:(NSString *)string options:(NSStringCompareOptions)mask;
- (NSComparisonResult)compare:(NSString *)string options:(NSStringCompareOptions)mask range:(NSRange)rangeOfReceiverToCompare;
- (NSComparisonResult)compare:(NSString *)string options:(NSStringCompareOptions)mask range:(NSRange)rangeOfReceiverToCompare locale:(nullable id)locale; // locale arg used to be a dictionary pre-Leopard. We now accept NSLocale. Assumes the current locale if non-nil and non-NSLocale. nil continues to mean canonical compare, which doesn't depend on user's locale choice.
- (NSComparisonResult)caseInsensitiveCompare:(NSString *)string;
- (NSComparisonResult)localizedCompare:(NSString *)string;
- (NSComparisonResult)localizedCaseInsensitiveCompare:(NSString *)string;

/* localizedStandardCompare:, added in 10.6, should be used whenever file names or other strings are presented in lists and tables where Finder-like sorting is appropriate.  The exact behavior of this method may be tweaked in future releases, and will be different under different localizations, so clients should not depend on the exact sorting order of the strings.
*/
- (NSComparisonResult)localizedStandardCompare:(NSString *)string NS_AVAILABLE(10_6, 4_0);

- (BOOL)isEqualToString:(NSString *)aString;


#pragma mark *** String searching ***

/* These perform locale unaware prefix or suffix match. If you need locale awareness, use rangeOfString:options:range:locale:, passing NSAnchoredSearch (or'ed with NSBackwardsSearch for suffix, and NSCaseInsensitiveSearch|NSDiacriticInsensitiveSearch if needed) for options, NSMakeRange(0, [receiver length]) for range, and [NSLocale currentLocale] for locale.
*/
- (BOOL)hasPrefix:(NSString *)str;//是否包含前缀
- (BOOL)hasSuffix:(NSString *)str; //是否包含后缀

- (NSString *)commonPrefixWithString:(NSString *)str options:(NSStringCompareOptions)mask;//两个字符串的相同前缀

/* Simple convenience methods for string searching. containsString: returns YES if the target string is contained within the receiver. Same as calling rangeOfString:options: with no options, thus doing a case-sensitive, locale-unaware search. localizedCaseInsensitiveContainsString: is the case-insensitive variant which also takes the current locale into effect. Starting in 10.11 and iOS9, the new localizedStandardRangeOfString: or localizedStandardContainsString: APIs are even better convenience methods for user level searching.   More sophisticated needs can be achieved by calling rangeOfString:options:range:locale: directly.
*/
- (BOOL)containsString:(NSString *)str NS_AVAILABLE(10_10, 8_0);
- (BOOL)localizedCaseInsensitiveContainsString:(NSString *)str NS_AVAILABLE(10_10, 8_0);

/* The following two are the most appropriate methods for doing user-level string searches, similar to how searches are done generally in the system.  The search is locale-aware, case and diacritic insensitive. As with other APIs, "standard" in the name implies "system default behavior," so the exact list of search options applied may change over time.  If you need more control over the search options, please use the rangeOfString:options:range:locale: method. You can pass [NSLocale currentLocale] for searches in user's locale.
*/
- (BOOL)localizedStandardContainsString:(NSString *)str NS_AVAILABLE(10_11, 9_0);
- (NSRange)localizedStandardRangeOfString:(NSString *)str NS_AVAILABLE(10_11, 9_0);

/* These methods perform string search, looking for the searchString within the receiver string.  These return length==0 if the target string is not found. So, to check for containment: ([str rangeOfString:@"target"].length > 0).  Note that the length of the range returned by these methods might be different than the length of the target string, due composed characters and such.
 
Note that the first three methods do not take locale arguments, and perform the search in a non-locale aware fashion, which is not appropriate for user-level searching. To do user-level string searching, use the last method, specifying locale:[NSLocale currentLocale], or better yet, use localizedStandardRangeOfString: or localizedStandardContainsString:.
 
The range argument specifies the subrange, rather than the whole, of the receiver to use in the search.  It is an error to specify a range that is outside of the receiver's bounds, and an exception may be raised.
*/
- (NSRange)rangeOfString:(NSString *)searchString;
- (NSRange)rangeOfString:(NSString *)searchString options:(NSStringCompareOptions)mask;
- (NSRange)rangeOfString:(NSString *)searchString options:(NSStringCompareOptions)mask range:(NSRange)rangeOfReceiverToSearch;
- (NSRange)rangeOfString:(NSString *)searchString options:(NSStringCompareOptions)mask range:(NSRange)rangeOfReceiverToSearch locale:(nullable NSLocale *)locale NS_AVAILABLE(10_5, 2_0);

/* These return the range of the first character from the set in the string, not the range of a sequence of characters. 
 
The range argument specifies the subrange, rather than the whole, of the receiver to use in the search.  It is an error to specify a range that is outside of the receiver's bounds, and an exception may be raised.
*/
- (NSRange)rangeOfCharacterFromSet:(NSCharacterSet *)searchSet;
- (NSRange)rangeOfCharacterFromSet:(NSCharacterSet *)searchSet options:(NSStringCompareOptions)mask;
- (NSRange)rangeOfCharacterFromSet:(NSCharacterSet *)searchSet options:(NSStringCompareOptions)mask range:(NSRange)rangeOfReceiverToSearch;

- (NSRange)rangeOfComposedCharacterSequenceAtIndex:(NSUInteger)index;
- (NSRange)rangeOfComposedCharacterSequencesForRange:(NSRange)range NS_AVAILABLE(10_5, 2_0);

- (NSString *)stringByAppendingString:(NSString *)aString;
- (NSString *)stringByAppendingFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);


#pragma mark *** Extracting numeric values ***

/* The following convenience methods all skip initial space characters (whitespaceSet) and ignore trailing characters. They are not locale-aware. NSScanner or NSNumberFormatter can be used for more powerful and locale-aware parsing of numbers.
*/
@property (readonly) double doubleValue;
@property (readonly) float floatValue;
@property (readonly) int intValue;
@property (readonly) NSInteger integerValue NS_AVAILABLE(10_5, 2_0);
@property (readonly) long long longLongValue NS_AVAILABLE(10_5, 2_0);
@property (readonly) BOOL boolValue NS_AVAILABLE(10_5, 2_0);  // Skips initial space characters (whitespaceSet), or optional -/+ sign followed by zeroes. Returns YES on encountering one of "Y", "y", "T", "t", or a digit 1-9. It ignores any trailing characters.


#pragma mark *** Case changing ***

/* The following three return the canonical (non-localized) mappings. They are suitable for programming operations that require stable results not depending on the user's locale preference.  For locale-aware case mapping for strings presented to users, use the "localized" methods below.
*/
@property (readonly, copy) NSString *uppercaseString;
@property (readonly, copy) NSString *lowercaseString;
@property (readonly, copy) NSString *capitalizedString;

/* The following three return the locale-aware case mappings. They are suitable for strings presented to the user.
*/
@property (readonly, copy) NSString *localizedUppercaseString NS_AVAILABLE(10_11, 9_0);
@property (readonly, copy) NSString *localizedLowercaseString NS_AVAILABLE(10_11, 9_0);
@property (readonly, copy) NSString *localizedCapitalizedString NS_AVAILABLE(10_11, 9_0);

/* The following methods perform localized case mappings based on the locale specified. Passing nil indicates the canonical mapping.  For the user preference locale setting, specify +[NSLocale currentLocale].
*/
- (NSString *)uppercaseStringWithLocale:(nullable NSLocale *)locale NS_AVAILABLE(10_8, 6_0);
- (NSString *)lowercaseStringWithLocale:(nullable NSLocale *)locale NS_AVAILABLE(10_8, 6_0);
- (NSString *)capitalizedStringWithLocale:(nullable NSLocale *)locale NS_AVAILABLE(10_8, 6_0);


#pragma mark *** Finding lines, sentences, words, etc ***
//根据给定的range计算出几个位置 ， startPtr代表range开始位置字符所在行的首字符位置，lineEndPtr代表range结束位置字符所在行的末字符的位置，contentsEndPtr 和lineEndPtr类似 去掉特殊字符的位置
//换行字符包含\r、\n 等参考 【U+000A 'LINE FEED (LF)' (\n)、U+000D 'CARRIAGE RETURN (CR)' (\r)、U+0085 'NEXT LINE (NEL)'、U+2028 'LINE SEPARATOR'、U+2029  'PARAGRAPH SEPARATOR'、\r\n, in that order (also known as CRLF)】
- (void)getLineStart:(nullable NSUInteger *)startPtr end:(nullable NSUInteger *)lineEndPtr contentsEnd:(nullable NSUInteger *)contentsEndPtr forRange:(NSRange)range;
- (NSRange)lineRangeForRange:(NSRange)range; //返回给定range 首字符所在行的开始与尾字符所在行结束  范围

//这两个方法与上面两个类似，只不过是按段落字符 【carriage return (U+000D), newline (U+000A), or paragraph separator (U+2029)】
- (void)getParagraphStart:(nullable NSUInteger *)startPtr end:(nullable NSUInteger *)parEndPtr contentsEnd:(nullable NSUInteger *)contentsEndPtr forRange:(NSRange)range;
- (NSRange)paragraphRangeForRange:(NSRange)range;

typedef NS_OPTIONS(NSUInteger, NSStringEnumerationOptions) {
    // Pass in one of the "By" options:
    NSStringEnumerationByLines = 0,                       // Equivalent to lineRangeForRange:
    NSStringEnumerationByParagraphs = 1,                  // Equivalent to paragraphRangeForRange:
    NSStringEnumerationByComposedCharacterSequences = 2,  // Equivalent to rangeOfComposedCharacterSequencesForRange:
    NSStringEnumerationByWords = 3,
    NSStringEnumerationBySentences = 4,
    // ...and combine any of the desired additional options:
    NSStringEnumerationReverse = 1UL << 8,
    NSStringEnumerationSubstringNotRequired = 1UL << 9,
    NSStringEnumerationLocalized = 1UL << 10              // User's default locale
};

/* In the enumerate methods, the blocks will be invoked inside an autorelease pool, so any values assigned inside the block should be retained.
*/
//根据配置进行子串遍历  可以根据 行、字符、单词、段落、 等等  enclosingRange包含换行字符
- (void)enumerateSubstringsInRange:(NSRange)range options:(NSStringEnumerationOptions)opts usingBlock:(void (^)(NSString * _Nullable substring, NSRange substringRange, NSRange enclosingRange, BOOL *stop))block NS_AVAILABLE(10_6, 4_0);
//根据换行符来按行枚举字符串，当需要停止的时候 设置 *stop = YES
- (void)enumerateLinesUsingBlock:(void (^)(NSString *line, BOOL *stop))block NS_AVAILABLE(10_6, 4_0);


#pragma mark *** Character encoding and converting to/from c-string representations ***
//返回一个内部的C指针，NSString会进行释放操作，所以如果外部需要在NSString生命周期外使用返回值需要自己去拷贝一份
@property (nullable, readonly) const char *UTF8String NS_RETURNS_INNER_POINTER;	// Convenience to return null-terminated UTF8 representation
//最快的ecoding方式，可能会比较耗费内存
@property (readonly) NSStringEncoding fastestEncoding;    	// Result in O(1) time; a rough estimate
//不是最快的，但是内存比较节省
@property (readonly) NSStringEncoding smallestEncoding;   	// Result in O(n) time; the encoding in which the string is most compact
//根据encoding生成NSData，如果设置了lossy为YES可能会丢失一些信息，比如重音什么的，第二个方法默认是NO，不会丢失任何信息
- (nullable NSData *)dataUsingEncoding:(NSStringEncoding)encoding allowLossyConversion:(BOOL)lossy;   // External representation
- (nullable NSData *)dataUsingEncoding:(NSStringEncoding)encoding;                                    // External representation
//判断是否可以用某种编码方式转换，如果用encoding转换不会丢失数据就返回YES，否则NO
- (BOOL)canBeConvertedToEncoding:(NSStringEncoding)encoding;

/* Methods to convert NSString to a NULL-terminated cString using the specified encoding. Note, these are the "new" cString methods, and are not deprecated like the older cString methods which do not take encoding arguments.
*/
//返回一个内部的C指针，调用者释放的时候内存可能会被释放需要注意，如果转换失败返回NULL
- (nullable const char *)cStringUsingEncoding:(NSStringEncoding)encoding NS_RETURNS_INNER_POINTER;	// "Autoreleased"; NULL return if encoding conversion not possible; for performance reasons, lifetime of this should not be considered longer than the lifetime of the receiving string (if the receiver string is freed, this might go invalid then, before the end of the autorelease scope)
//如果转换失败或者buffer太小 返回NO，  需要注意，该方法会在字符串结尾加一个空字符，所以需要maxBufferCount需要比实际大一个字符大小
- (BOOL)getCString:(char *)buffer maxLength:(NSUInteger)maxBufferCount encoding:(NSStringEncoding)encoding;	// NO return if conversion not possible due to encoding errors or too small of a buffer. The buffer should include room for maxBufferCount bytes; this number should accomodate the expected size of the return value plus the NULL termination character, which this method adds. (So note that the maxLength passed to this method is one more than the one you would have passed to the deprecated getCString:maxLength:.)

/* Use this to convert string section at a time into a fixed-size buffer, without any allocations.  Does not NULL-terminate. 
    buffer is the buffer to write to; if NULL, this method can be used to computed size of needed buffer.
    maxBufferCount is the length of the buffer in bytes. It's a good idea to make sure this is at least enough to hold one character's worth of conversion. 
    usedBufferCount is the length of the buffer used up by the current conversion. Can be NULL.
    encoding is the encoding to convert to.
    options specifies the options to apply.
    range is the range to convert.
    leftOver is the remaining range. Can be NULL.
    YES return indicates some characters were converted. Conversion might usually stop when the buffer fills, 
      but it might also stop when the conversion isn't possible due to the chosen encoding. 
*/
//使用这个方法来转换字符串到一个固定大小的buffer里，不包含结束的空字符，
//buffer如果为空，这个方法可以用来计算需要buffer的大小
//maxBufferCount 是buffer的字节大小，最好至少能容纳转换一个字符
//usedBufferCount 是当前转换使用的长度，可以为空
//encoding 是编码方式
//range 是要转换的区间
// leftOver是剩余的range，可以为空
//如果有字符成功转换，返回YES  ，转换可能停止当buffer满了，也可能因为选择的encoding方式不能转换。
- (BOOL)getBytes:(nullable void *)buffer maxLength:(NSUInteger)maxBufferCount usedLength:(nullable NSUInteger *)usedBufferCount encoding:(NSStringEncoding)encoding options:(NSStringEncodingConversionOptions)options range:(NSRange)range remainingRange:(nullable NSRangePointer)leftover;

/* These return the maximum and exact number of bytes needed to store the receiver in the specified encoding in non-external representation. The first one is O(1), while the second one is O(n). These do not include space for a terminating null.
*/
//返回一个最大可能需要的大小，可能会大于实际需要，未考虑在结尾加空字符， 如果大于NSIntegerMax或者不能转换 则返回0 O(1)
- (NSUInteger)maximumLengthOfBytesUsingEncoding:(NSStringEncoding)enc;	// Result in O(1) time; the estimate may be way over what's needed. Returns 0 on error (overflow)
//返回实际需要的大小，未考虑在结尾加空字符， 如果大于NSIntegerMax或者不能转换 则返回0   O(n)
- (NSUInteger)lengthOfBytesUsingEncoding:(NSStringEncoding)enc;		// Result in O(n) time; the result is exact. Returns 0 on error (cannot convert to specified encoding, or overflow)

#if FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(8)
@property (class, readonly) const NSStringEncoding *availableStringEncodings;
#endif
//返回一个编码方式的 人类可读的名字
+ (NSString *)localizedNameOfStringEncoding:(NSStringEncoding)encoding;

/* User-dependent encoding whose value is derived from user's default language and potentially other factors. The use of this encoding might sometimes be needed when interpreting user documents with unknown encodings, in the absence of other hints.  This encoding should be used rarely, if at all. Note that some potential values here might result in unexpected encoding conversions of even fairly straightforward NSString content --- for instance, punctuation characters with a bidirectional encoding.
 */
#if FOUNDATION_SWIFT_SDK_EPOCH_AT_LEAST(8)
@property (class, readonly) NSStringEncoding defaultCStringEncoding;	// Should be rarely used
#endif


#pragma mark *** Other ***

//参考 ：http://unicode.org/faq/normalization.html
@property (readonly, copy) NSString *decomposedStringWithCanonicalMapping; //字符串的内容是使用Unicode范式D标准化获取的
@property (readonly, copy) NSString *precomposedStringWithCanonicalMapping;//字符串的内容是使用Unicode范式C标准化获取的
@property (readonly, copy) NSString *decomposedStringWithCompatibilityMapping; //字符串的内容是使用Unicode范式KD标准化获取的
@property (readonly, copy) NSString *precomposedStringWithCompatibilityMapping;//字符串的内容是使用Unicode范式KC标准化获取的

//根据给定的 字符串 来分割调用者
//相邻的分割串  会产生一个空串，在开头或者结尾有分隔符也会产生一个空串
//如果没有分隔符 ，返回一个调用者本身
//加入分隔符是"," 那么
// @"Karin, Carrie, David" -> { @"Karin", @"Carrie", @"David"" }
//@", Norman, Stanley, Fletcher"  ->  { "", @"Norman", @"Stanley", @"Fletcher" }
//"Karin"  -> { @"Karin" }
- (NSArray<NSString *> *)componentsSeparatedByString:(NSString *)separator;
//和上面的方法类似  ，参数为NSCharacterSet类型
- (NSArray<NSString *> *)componentsSeparatedByCharactersInSet:(NSCharacterSet *)separator NS_AVAILABLE(10_5, 2_0);
//去掉字符串中 开头和结尾的NSCharacterSet字符
- (NSString *)stringByTrimmingCharactersInSet:(NSCharacterSet *)set;
//使用padString 将调用者填充到newLength长度，padIndex标示从padString的第几个字符开始填充
//[@"abc" stringByPaddingToLength: 9 withString: @"." startingAtIndex:0];// Results in "abc......"
//[@"abc" stringByPaddingToLength: 2 withString: @"." startingAtIndex:0];// Results in "ab"
//[@"abc" stringByPaddingToLength: 9 withString: @"xyz" startingAtIndex:1];// Results in "abcyzxyzx"  //注意是从y字符开始填充的
- (NSString *)stringByPaddingToLength:(NSUInteger)newLength withString:(NSString *)padString startingAtIndex:(NSUInteger)padIndex;

/* Returns a string with the character folding options applied. theOptions is a mask of compare flags with *InsensitiveSearch suffix.
*/
- (NSString *)stringByFoldingWithOptions:(NSStringCompareOptions)options locale:(nullable NSLocale *)locale NS_AVAILABLE(10_5, 2_0);

/* Replace all occurrences of the target string in the specified range with replacement. Specified compare options are used for matching target. If NSRegularExpressionSearch is specified, the replacement is treated as a template, as in the corresponding NSRegularExpression methods, and no other options can apply except NSCaseInsensitiveSearch and NSAnchoredSearch.
*/
//替换字符串
- (NSString *)stringByReplacingOccurrencesOfString:(NSString *)target withString:(NSString *)replacement options:(NSStringCompareOptions)options range:(NSRange)searchRange NS_AVAILABLE(10_5, 2_0);

/* Replace all occurrences of the target string with replacement. Invokes the above method with 0 options and range of the whole string.
*/
//替换字符串  调用上面的方法  options传0 ，range 为整个字符串
- (NSString *)stringByReplacingOccurrencesOfString:(NSString *)target withString:(NSString *)replacement NS_AVAILABLE(10_5, 2_0);

/* Replace characters in range with the specified string, returning new string.
*/
//替换range里的字符串为 replacement
- (NSString *)stringByReplacingCharactersInRange:(NSRange)range withString:(NSString *)replacement NS_AVAILABLE(10_5, 2_0);

typedef NSString *NSStringTransform NS_EXTENSIBLE_STRING_ENUM;

/* Perform string transliteration.  The transformation represented by transform is applied to the receiver. reverse indicates that the inverse transform should be used instead, if it exists. Attempting to use an invalid transform identifier or reverse an irreversible transform will return nil; otherwise the transformed string value is returned (even if no characters are actually transformed). You can pass one of the predefined transforms below (NSStringTransformLatinToKatakana, etc), or any valid ICU transform ID as defined in the ICU User Guide. Arbitrary ICU transform rules are not supported.
*/
//字符串翻译函数，reverse 参数表示将应用相反的映射进行翻译,使用无效的映射标识，或尝试反转一个不可反转的映射，将返回 nil
- (nullable NSString *)stringByApplyingTransform:(NSStringTransform)transform reverse:(BOOL)reverse NS_AVAILABLE(10_11, 9_0);	// Returns nil if reverse not applicable or transform is invalid

FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToKatakana         NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToHiragana         NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToHangul           NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToArabic           NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToHebrew           NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToThai             NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToCyrillic         NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformLatinToGreek            NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformToLatin                 NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformMandarinToLatin         NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformHiraganaToKatakana      NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformFullwidthToHalfwidth    NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformToXMLHex                NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformToUnicodeName           NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformStripCombiningMarks     NS_AVAILABLE(10_11, 9_0);
FOUNDATION_EXPORT NSStringTransform const NSStringTransformStripDiacritics         NS_AVAILABLE(10_11, 9_0);


/* Write to specified url or path using the specified encoding.  The optional error return is to indicate file system or encoding errors.
 */
//字符串翻译函数，reverse 参数表示将应用相反的映射进行翻译,使用无效的映射标识，或尝试反转一个不可反转的映射，将返回 nil
- (BOOL)writeToURL:(NSURL *)url atomically:(BOOL)useAuxiliaryFile encoding:(NSStringEncoding)enc error:(NSError **)error;
//和上面的方法类似，写入文件
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile encoding:(NSStringEncoding)enc error:(NSError **)error;

@property (readonly, copy) NSString *description;
//如果两个字符串想同，则哈希值相同
@property (readonly) NSUInteger hash;


#pragma mark *** Initializers ***

/* 一般情况下NSString的 下面的方法不应用于子类，这里假设子类都按照自己希望的方式来实现他们自己的create方法，
 指定的初始化方法只有 init 和 initWithCoder:
*/
//根据 UTF-16 编码的c字符数组来初始化一个NSString， characters永远不能为空，否则导致会异常，即使length为0；
//注意：当初始化出错的时候 不管freeBuffer是否是YES，该方法不会释放characters，需要调用者自己释放
- (instancetype)initWithCharactersNoCopy:(unichar *)characters length:(NSUInteger)length freeWhenDone:(BOOL)freeBuffer;	//该方法是no copy ，用的是同一块地址

//该方法不会处理characters的内存，拷贝一份来初始化NSString
- (instancetype)initWithCharacters:(const unichar *)characters length:(NSUInteger)length;

//根据UTF8编码的C字符串数组来初始化一个NSString，c字符串不能为空，否则导致异常，该方法会copy一份c数组来初始化,如果字符串有异常  ，初始化返回空
- (nullable instancetype)initWithUTF8String:(const char *)nullTerminatedCString;

//使用一个NSString来初始化NSString，aString不能为nil，否则导致异常
- (instancetype)initWithString:(NSString *)aString;

//根据格式format和参数来初始化字符串 format均不能为空，为空会导致异常，
//...标示可变参数，argList为参数列表 参考：http://www.cnblogs.com/hanyonglu/archive/2011/05/07/2039916.html
//NS_FORMAT_FUNCTION 是格式化检查宏，宏的第一个参数表示函数的第几个参数是格式化串，第二个参数表示函数第几个参数是可变参数 参考：http://blog.csdn.net/houseq/article/details/38819901
- (instancetype)initWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);

//该方法会调用 initWithFormat:locale:arguments: 来初始化，并不传本地化参数
- (instancetype)initWithFormat:(NSString *)format arguments:(va_list)argList NS_FORMAT_FUNCTION(1,0);

//如果使用本地locale ，传 [NSLocale currentLocale]  如果使用系统的 locale传nil
//格式化符号参考：https://developer.apple.com/library/prerelease/content/documentation/Cocoa/Conceptual/Strings/Articles/formatSpecifiers.html#//apple_ref/doc/uid/TP40004265-SW1
- (instancetype)initWithFormat:(NSString *)format locale:(nullable id)locale, ... NS_FORMAT_FUNCTION(1,3);
- (instancetype)initWithFormat:(NSString *)format locale:(nullable id)locale arguments:(va_list)argList NS_FORMAT_FUNCTION(1,0);

//根据NSData初始化NSString，如果初始化失败返回null
- (nullable instancetype)initWithData:(NSData *)data encoding:(NSStringEncoding)encoding;

//根据字节buffer初始化NSString，如果异常返回null，如果bytes的长度大于len，会返回null
- (nullable instancetype)initWithBytes:(const void *)bytes length:(NSUInteger)len encoding:(NSStringEncoding)encoding;
//和上面的方法一样，只不过不拷贝一份数据初始化，如果初始化失败，调用者需要自己释放bytes（即使freeBuffer为YES）
- (nullable instancetype)initWithBytesNoCopy:(void *)bytes length:(NSUInteger)len encoding:(NSStringEncoding)encoding freeWhenDone:(BOOL)freeBuffer;	/* "NoCopy" is a hint */

 //返回一个空NSString
+ (instancetype)string;
+ (instancetype)stringWithString:(NSString *)string;//通过拷贝参数初始化一个NSString ，参数不能为空
//根据UTF-16编码的C数组初始化，characters不能为空
+ (instancetype)stringWithCharacters:(const unichar *)characters length:(NSUInteger)length;
//根据一个UTF8串初始化NSString
+ (nullable instancetype)stringWithUTF8String:(const char *)nullTerminatedCString;
+ (instancetype)stringWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);
//使用[NSLocale currentLocale]
+ (instancetype)localizedStringWithFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);

//如果nullTerminatedCString 不是一个以空结束的字符串或者encoding不是字符串的编码方式，返回结果不确定 ，nullTerminatedCString中间也不能有空字符
- (nullable instancetype)initWithCString:(const char *)nullTerminatedCString encoding:(NSStringEncoding)encoding;
+ (nullable instancetype)stringWithCString:(const char *)cString encoding:(NSStringEncoding)enc;

/* 这些方法使用指定的encoding，如果返回值是null，error会返回相关的错误信息*/
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url encoding:(NSStringEncoding)enc error:(NSError **)error;
- (nullable instancetype)initWithContentsOfFile:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error;
+ (nullable instancetype)stringWithContentsOfURL:(NSURL *)url encoding:(NSStringEncoding)enc error:(NSError **)error;
+ (nullable instancetype)stringWithContentsOfFile:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error;

/* 这些方法会自动识别encoding，并且返回真是的encoding，这些方法在后面的版本中会越来越智能，使用额外的技术来识别encoding，如果返回值为空，可以通过error参数取到错信息*/
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url usedEncoding:(nullable NSStringEncoding *)enc error:(NSError **)error;//url不能打开或者enc不能决定，返回nil
- (nullable instancetype)initWithContentsOfFile:(NSString *)path usedEncoding:(nullable NSStringEncoding *)enc error:(NSError **)error;
+ (nullable instancetype)stringWithContentsOfURL:(NSURL *)url usedEncoding:(nullable NSStringEncoding *)enc error:(NSError **)error;
+ (nullable instancetype)stringWithContentsOfFile:(NSString *)path usedEncoding:(nullable NSStringEncoding *)enc error:(NSError **)error;

@end

typedef NSString * NSStringEncodingDetectionOptionsKey NS_STRING_ENUM;

@interface NSString (NSStringEncodingDetection)

#pragma mark *** Encoding detection ***

/* This API is used to detect the string encoding of a given raw data. It can also do lossy string conversion. It converts the data to a string in the detected string encoding. The data object contains the raw bytes, and the option dictionary contains the hints and parameters for the analysis. The opts dictionary can be nil. If the string parameter is not NULL, the string created by the detected string encoding is returned. The lossy substitution string is emitted in the output string for characters that could not be converted when lossy conversion is enabled. The usedLossyConversion indicates if there is any lossy conversion in the resulted string. If no encoding can be detected, 0 is returned.
 这个接口用来检测给定数据的编码方式，也可以用来粗略的转换成字符串，该接口会将data使用监测到的编码方式进行转换，opts是检测时候的参数，可以为空，如果该参数不为空，转换好的字符串会通过string参数返回，usedLossyConversion参数表明转换过程中是否会有数据丢失,如果发现没有任何编码方式可以转换，返回0
 opts参数可能的值
 1、一个建议的编码方式的数组（数组中没有第三方的选项，数组里所有的编码方式会被考虑，但是数组中的会有更好的性能，而且数组里的表编码方式是按性能优先级来罗列的）
 2、一个未使用的编码方式的数组（这里列出来的转换时不被考虑）
 3、一个bool值标示是否只考虑建议的编码方式
 4、一个bool值标示是否允许损耗
 5、给一些神秘的字节指定转换
 6、当前用户的语言
 7、一个bool值标示是否数据是Windows系统生成的
 如果字典中的数据有错误的类型，比如建议的编码方式如果不是一个数组  会导致异常
 如果字典中的数据有未知的，比如建议的编码方式不是一个有效的编码方式，字典里的这条参数就会被忽略
The possible items for the dictionary are:
 1) an array of suggested string encodings (without specifying the 3rd option in this list, all string encodings are considered but the ones in the array will have a higher preference; moreover, the order of the encodings in the array is important: the first encoding has a higher preference than the second one in the array)
 2) an array of string encodings not to use (the string encodings in this list will not be considered at all)
 3) a boolean option indicating whether only the suggested string encodings are considered
 4) a boolean option indicating whether lossy is allowed
 5) an option that gives a specific string to substitude for mystery bytes
 6) the current user's language
 7) a boolean option indicating whether the data is generated by Windows
 
If the values in the dictionary have wrong types (for example, the value of NSStringEncodingDetectionSuggestedEncodingsKey is not an array), an exception is thrown.
If the values in the dictionary are unknown (for example, the value in the array of suggested string encodings is not a valid encoding), the values will be ignored.
*/
+ (NSStringEncoding)stringEncodingForData:(NSData *)data
                          encodingOptions:(nullable NSDictionary<NSStringEncodingDetectionOptionsKey, id> *)opts
                          convertedString:(NSString * _Nullable * _Nullable)string
                      usedLossyConversion:(nullable BOOL *)usedLossyConversion NS_AVAILABLE(10_10, 8_0);

/* The following keys are for the option dictionary for the string encoding detection API.
*/
FOUNDATION_EXPORT NSStringEncodingDetectionOptionsKey const NSStringEncodingDetectionSuggestedEncodingsKey           NS_AVAILABLE(10_10, 8_0);   // NSArray of NSNumbers which contain NSStringEncoding values; if this key is not present in the dictionary, all encodings are weighted the same
FOUNDATION_EXPORT NSStringEncodingDetectionOptionsKey const NSStringEncodingDetectionDisallowedEncodingsKey          NS_AVAILABLE(10_10, 8_0);   // NSArray of NSNumbers which contain NSStringEncoding values; if this key is not present in the dictionary, all encodings are considered
FOUNDATION_EXPORT NSStringEncodingDetectionOptionsKey const NSStringEncodingDetectionUseOnlySuggestedEncodingsKey    NS_AVAILABLE(10_10, 8_0);   // NSNumber boolean value; if this key is not present in the dictionary, the default value is NO
FOUNDATION_EXPORT NSStringEncodingDetectionOptionsKey const NSStringEncodingDetectionAllowLossyKey                   NS_AVAILABLE(10_10, 8_0);   // NSNumber boolean value; if this key is not present in the dictionary, the default value is YES
FOUNDATION_EXPORT NSStringEncodingDetectionOptionsKey const NSStringEncodingDetectionFromWindowsKey                  NS_AVAILABLE(10_10, 8_0);   // NSNumber boolean value; if this key is not present in the dictionary, the default value is NO
FOUNDATION_EXPORT NSStringEncodingDetectionOptionsKey const NSStringEncodingDetectionLossySubstitutionKey            NS_AVAILABLE(10_10, 8_0);   // NSString value; if this key is not present in the dictionary, the default value is U+FFFD
FOUNDATION_EXPORT NSStringEncodingDetectionOptionsKey const NSStringEncodingDetectionLikelyLanguageKey               NS_AVAILABLE(10_10, 8_0);   // NSString value; ISO language code; if this key is not present in the dictionary, no such information is considered

@end



@interface NSMutableString : NSString

#pragma mark *** Mutable string ***

/* NSMutableString primitive (funnel) method. See below for the other mutation methods.
 用 aString 替换字符串中 range 范围内的字符串，range不能超过字符串长度，astring 不能为空  这是Mutable最原始的方法
*/
- (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)aString;

@end

@interface NSMutableString (NSMutableStringExtensionMethods)

/* Additional mutation methods.  For subclassers these are all available implemented in terms of the primitive replaceCharactersInRange:range: method.
*/
//下面的方法都是基于上面的方法实现的
//aString不能为空，loc不能超过长度
- (void)insertString:(NSString *)aString atIndex:(NSUInteger)loc;
- (void)deleteCharactersInRange:(NSRange)range;
- (void)appendString:(NSString *)aString;
- (void)appendFormat:(NSString *)format, ... NS_FORMAT_FUNCTION(1,2);
- (void)setString:(NSString *)aString;

/* 
 用replacement替换所有的target ,返回替换的个数
This method replaces all occurrences of the target string with the replacement string, in the specified range of the receiver string, and returns the number of replacements. NSBackwardsSearch means the search is done from the end of the range (the results could be different); NSAnchoredSearch means only anchored (but potentially multiple) instances will be replaced. NSLiteralSearch and NSCaseInsensitiveSearch also apply. NSNumericSearch is ignored. Use NSMakeRange(0, [receiver length]) to process whole string. If NSRegularExpressionSearch is specified, the replacement is treated as a template, as in the corresponding NSRegularExpression methods, and no other options can apply except NSCaseInsensitiveSearch and NSAnchoredSearch.
*/
- (NSUInteger)replaceOccurrencesOfString:(NSString *)target withString:(NSString *)replacement options:(NSStringCompareOptions)options range:(NSRange)searchRange;

/* 字符串翻译函数
Perform string transliteration.  The transformation represented by transform is applied to the given range of string in place. Only the specified range will be modified, but the transform may look at portions of the string outside that range for context. If supplied, resultingRange is modified to reflect the new range corresponding to the original range. reverse indicates that the inverse transform should be used instead, if it exists. Attempting to use an invalid transform identifier or reverse an irreversible transform will return NO; otherwise YES is returned, even if no characters are actually transformed. You can pass one of the predefined transforms listed above (NSStringTransformLatinToKatakana, etc), or any valid ICU transform ID as defined in the ICU User Guide. Arbitrary ICU transform rules are not supported.
*/
- (BOOL)applyTransform:(NSString *)transform reverse:(BOOL)reverse range:(NSRange)range updatedRange:(nullable NSRangePointer)resultingRange NS_AVAILABLE(10_11, 9_0);

/* In addition to these two, NSMutableString responds properly to all NSString creation methods.
 初始化函数

 */
- (NSMutableString *)initWithCapacity:(NSUInteger)capacity;
+ (NSMutableString *)stringWithCapacity:(NSUInteger)capacity;

@end



FOUNDATION_EXPORT NSExceptionName const NSCharacterConversionException;
FOUNDATION_EXPORT NSExceptionName const NSParseErrorException; // raised by -propertyList
#define NSMaximumStringLength	(INT_MAX-1)

#pragma mark *** Deprecated/discouraged APIs ***

@interface NSString (NSExtendedStringPropertyListParsing)

/* These methods are no longer recommended since they do not work with property lists and strings files in binary plist format. Please use the APIs in NSPropertyList.h instead.
 */
- (id)propertyList;
- (nullable NSDictionary *)propertyListFromStringsFileFormat;

@end

@interface NSString (NSStringDeprecated)

/* The following methods are deprecated and will be removed from this header file in the near future. These methods use NSString.defaultCStringEncoding as the encoding to convert to, which means the results depend on the user's language and potentially other settings. This might be appropriate in some cases, but often these methods are misused, resulting in issues when running in languages other then English. UTF8String in general is a much better choice when converting arbitrary NSStrings into 8-bit representations. Additional potential replacement methods are being introduced in NSString as appropriate.
*/
- (nullable const char *)cString NS_RETURNS_INNER_POINTER NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (nullable const char *)lossyCString NS_RETURNS_INNER_POINTER NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (NSUInteger)cStringLength NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (void)getCString:(char *)bytes NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (void)getCString:(char *)bytes maxLength:(NSUInteger)maxLength NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (void)getCString:(char *)bytes maxLength:(NSUInteger)maxLength range:(NSRange)aRange remainingRange:(nullable NSRangePointer)leftoverRange NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);

- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (BOOL)writeToURL:(NSURL *)url atomically:(BOOL)atomically NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);

- (nullable id)initWithContentsOfFile:(NSString *)path NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (nullable id)initWithContentsOfURL:(NSURL *)url NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
+ (nullable id)stringWithContentsOfFile:(NSString *)path NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
+ (nullable id)stringWithContentsOfURL:(NSURL *)url NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);

- (nullable id)initWithCStringNoCopy:(char *)bytes length:(NSUInteger)length freeWhenDone:(BOOL)freeBuffer NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (nullable id)initWithCString:(const char *)bytes length:(NSUInteger)length NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
- (nullable id)initWithCString:(const char *)bytes NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
+ (nullable id)stringWithCString:(const char *)bytes length:(NSUInteger)length NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);
+ (nullable id)stringWithCString:(const char *)bytes NS_DEPRECATED(10_0, 10_4, 2_0, 2_0);

/* This method is unsafe because it could potentially cause buffer overruns. You should use -getCharacters:range: instead.
*/
- (void)getCharacters:(unichar *)buffer;

@end

NS_ENUM(NSStringEncoding) {
    NSProprietaryStringEncoding = 65536    /* Installation-specific encoding */
};

/* The rest of this file is bookkeeping stuff that has to be here. Don't use this stuff, don't refer to it.
*/
#if !defined(_OBJC_UNICHAR_H_)
#define _OBJC_UNICHAR_H_
#endif
#define NS_UNICHAR_IS_EIGHT_BIT 0

@interface NSSimpleCString : NSString {
@package
    char *bytes;
    int numBytes;
#if __LP64__
    int _unused;
#endif
}
@end

@interface NSConstantString : NSSimpleCString
@end

#if __LP64__
#else
extern void *_NSConstantStringClassReference;
#endif

NS_ASSUME_NONNULL_END
