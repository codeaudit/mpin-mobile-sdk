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


#import "CycleProgressBar.h"

#define kBeginAngle 1.57
#define kEndAngle 7.85
#define kLablelWidth 30

#define kLabelHeight 30
#define kArcWidth 26

#define kRed 0.9
#define kGreen 0.9
#define kBlue 0.9

@implementation CycleProgressBar

@synthesize isLoading;

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        self.backgroundColor = [UIColor whiteColor];
        context = 0;
        ind = 1.0;
        initialRadian = kEndAngle;

        _counterLabel = [[UILabel alloc] initWithFrame:CGRectMake(((frame.size.width - kLablelWidth) / 2), ((frame.size.height - kLabelHeight) * 5 / 12), kLablelWidth, kLabelHeight)];
        [_counterLabel setTextAlignment:NSTextAlignmentCenter];
        [_counterLabel setFont:[UIFont fontWithName:@"OpenSans" size:26]];
        [self addSubview:_counterLabel];

        _secLabel = [[UILabel alloc] initWithFrame:CGRectMake(((frame.size.width - kLablelWidth) / 2), ((frame.size.height - kLabelHeight) * 7 / 12), kLablelWidth, kLabelHeight)];
        [_secLabel setTextAlignment:NSTextAlignmentCenter];
        _secLabel.text = @"SEC";
        [_secLabel setTextColor:[UIColor lightGrayColor]];
        [_secLabel setFont:[UIFont systemFontOfSize:10]];
        [self addSubview:_secLabel];

        isLoading = NO;
    }
    return self;
}

- (void)drawRect:(CGRect)rect
{
    if (context == 0) {
        context = UIGraphicsGetCurrentContext();
    }

    CGContextSetRGBStrokeColor(context, kRed, kGreen, kBlue, 1.0);
    CGContextSetLineWidth(context, kArcWidth);
    CGContextAddArc(context, (rect.size.width / 2), (rect.size.height / 2), ((rect.size.height / 2) - (kArcWidth / 2)), 0.0, kEndAngle, FALSE);
    CGContextStrokePath(context);

    CGContextSetRGBStrokeColor(context, 1.0, 1.0, 1.0, 1.0);
    CGContextSetLineWidth(context, kArcWidth);
    CGContextAddArc(context, (rect.size.width / 2), (rect.size.height / 2), ((rect.size.height / 2) - (kArcWidth / 2)), -kBeginAngle, -initialRadian, FALSE);
    CGContextStrokePath(context);

    initialRadian = initialRadian - 0.01;
    if (initialRadian < kBeginAngle) {
        [self stopAnimation];
        [self sendActionsForControlEvents:UIControlEventTouchUpInside];
    }
}

- (void)onUpdateCounter
{
    self.counterLabel.text = [NSString stringWithFormat:@"%d", --counter];
}

- (void)setDuration:(NSTimeInterval)duration
{
    durInLine = duration / ((initialRadian - kBeginAngle) * 100);
    counter = duration;
    self.counterLabel.text = [NSString stringWithFormat:@"%d", counter];
}

- (void)startAnimation:(NSTimeInterval)withDuration
{
    [self setDuration:withDuration];
    [self startAnimation];
}

- (void)startAnimation
{
    animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(durInLine)target:self selector:@selector(setNeedsDisplay) userInfo:nil repeats:TRUE];
    counterTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0)target:self selector:@selector(onUpdateCounter) userInfo:nil repeats:TRUE];
}

- (void)stopAnimation
{
    if (animationTimer != nil) {
        [animationTimer invalidate];
        animationTimer = nil;
    }
    if (counterTimer != nil) {
        [counterTimer invalidate];
        counterTimer = nil;
    }
}

@end
