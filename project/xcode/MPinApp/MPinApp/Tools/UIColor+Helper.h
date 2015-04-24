
#import <UIKit/UIKit.h>

@interface UIColor (Helper)

@property (nonatomic, readonly) CGColorSpaceModel colorSpaceModel;
@property (nonatomic, readonly) CGFloat red;
@property (nonatomic, readonly) CGFloat green;
@property (nonatomic, readonly) CGFloat blue;

@property (NS_NONATOMIC_IOSONLY, readonly) BOOL canProvideRGBComponents;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString* toHexString;

+ (UIColor*)colorWithHexString:(NSString*)hexToConvert;
+ (UIColor*)colorWithHexString:(NSString*)hexToConvert alpha:(CGFloat)opacity;
+ (UIColor*)colorWithRGBHex:(UInt32)hex alpha:(CGFloat)opacity;

@end
