//
//  ErrorHandler.m
//  MPinApp
//
//  Created by Tihomir Ganev on 15.May.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "ErrorHandler.h"
#import "ATMHud.h"

@interface CVXATMHud()

@end

@implementation CVXATMHud

- (instancetype)init
{
    if ((self = [super init]))
    {

    }
    return self;
}

@end


@interface ErrorHandler(){
    
}
@property (nonatomic, strong) CVXATMHud *hud;
@end

@implementation ErrorHandler

+ (ErrorHandler*)sharedManager
{
    static ErrorHandler* sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedManager = [[self alloc] init];
    });
    return sharedManager;
}

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        self.hud = [[CVXATMHud alloc] initWithDelegate:self];
    }
    return self;
}
-(void) startLoadingInController:(UIViewController *)viewController message:(NSString *)message
{
    [_hud setActivity:YES];
    [_hud setCaption:message];
    [_hud showInView:viewController.view];
}

-(void) stopLoading
{
    [self hideError];
}

-(void) presentErrorInViewController:(UIViewController *)viewController
                         errorString:(NSString *)strError
                addActivityIndicator:(BOOL)addActivityIndicator
                   autoHideInSeconds:(NSInteger) seconds
{
    _hud.minShowTime = seconds;
    [_hud setCaption:strError];
    [_hud showInView:viewController.view];
    [_hud hide];
}

-(void) hideError
{
    [_hud hide];
    NSLog(@"Hiding hud");
}

@end
