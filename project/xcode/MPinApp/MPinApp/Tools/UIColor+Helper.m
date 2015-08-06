/*
 Copyright (c) 2012-2015, Certivox
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 For full details regarding our CertiVox terms of service please refer to
 the following links:
 * Our Terms and Conditions -
 http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
 http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
 http://www.certivox.com/about-certivox/patents/
 */


#import "UIColor+Helper.h"

@implementation UIColor (Helper)

#pragma mark ColorSpaceModel

- (CGColorSpaceModel)colorSpaceModel
{
    return CGColorSpaceGetModel(CGColorGetColorSpace(self.CGColor));
}

- (BOOL)canProvideRGBComponents
{
    switch (self.colorSpaceModel) {
        case kCGColorSpaceModelRGB:
        case kCGColorSpaceModelMonochrome:
            return YES;
        default:
            return NO;
    }
}

#pragma mark Color based getters

- (CGFloat)red
{
    NSAssert(self.canProvideRGBComponents, @"Must be an RGB color");
    const CGFloat *c = CGColorGetComponents(self.CGColor);
    return c[0];
}

- (CGFloat)green
{
    NSAssert(self.canProvideRGBComponents, @"Must be an RGB color");
    const CGFloat *c = CGColorGetComponents(self.CGColor);
    return c[1];
}

- (CGFloat)blue
{
    NSAssert(self.canProvideRGBComponents, @"Must be an RGB color");
    const CGFloat *c = CGColorGetComponents(self.CGColor);
    return c[2];
}

#pragma mark Conversion

- (NSString *)toHexString
{
    NSAssert(self.canProvideRGBComponents, @"Must be an RGB color");
    
    CGFloat r, g, b;
    r = self.red;
    g = self.green;
    b = self.blue;
    
    if (r < 0.0f) r = 0.0f;
    if (g < 0.0f) g = 0.0f;
    if (b < 0.0f) b = 0.0f;
    if (r > 1.0f) r = 1.0f;
    if (g > 1.0f) g = 1.0f;
    if (b > 1.0f) b = 1.0f;
    
    return [NSString stringWithFormat:@"%02X%02X%02X",
            (int)(r * 255), (int)(g * 255), (int)(b * 255)];
}

#pragma mark Initialization

+ (UIColor *)colorWithRGBHex:(UInt32)hex alpha:(CGFloat)opacity
{
    int r = (hex >> 16) & 0xFF;
    int g = (hex >> 8) & 0xFF;
    int b = (hex) & 0xFF;
    
    return [UIColor colorWithRed:r / 255.0f green:g / 255.0f blue:b / 255.0f alpha:opacity];
}

+ (UIColor *)colorWithHexString:(NSString *)hexToConvert
{
    return [UIColor colorWithHexString:hexToConvert alpha:1.0f];
}

+ (UIColor *)colorWithHexString:(NSString *)hexToConvert alpha:(CGFloat)opacity
{
    NSString *colorString = [[[hexToConvert stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] stringByTrimmingCharactersInSet:[NSCharacterSet symbolCharacterSet]] uppercaseString];
    
    if ([colorString hasPrefix:@"0X"]) colorString = [colorString substringFromIndex:2];
    
    NSScanner *scanner = [NSScanner scannerWithString:colorString];
    unsigned hexNum;
    if (![scanner scanHexInt:&hexNum]) return nil;
    
    return [UIColor colorWithRGBHex:hexNum alpha:opacity];
}

@end
