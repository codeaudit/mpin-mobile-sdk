//
//  CycleProgressBar.h
//  progressTest
//
//  Created by Admin on 2/23/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

@interface CycleProgressBar : UIControl {
	NSTimer * animationTimer;
    NSTimer * counterTimer;
	CGContextRef context;
	NSUInteger ind;
	CGFloat  initialRadian;
	NSTimeInterval durInLine;
	UIImageView * loadingScreen;
	BOOL isLoading;
	UIActivityIndicatorView * loadingIndicator;
    int counter;
}

@property (nonatomic, retain , readwrite) UILabel * counterLabel;
@property (nonatomic, retain , readwrite) UILabel * secLabel;

@property (nonatomic, assign) BOOL isLoading; 

- (void) startAnimation;
- (void) startAnimation:(NSTimeInterval) withDuration;
- (void) stopAnimation;
- (void) setDuration:(NSTimeInterval) duration;
- (void) onUpdateCounter;

@end
