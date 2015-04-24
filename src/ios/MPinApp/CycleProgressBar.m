//
//  CycleProgressBar.m
//  progressTest
//
//  Created by Admin on 2/23/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

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
