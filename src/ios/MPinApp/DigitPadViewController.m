//
//  DigitPadViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 1/22/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "DigitPadViewController.h"
#import "UIView+Helper.h"
#import "UIViewController+Helper.h"
#import "Constants.h"

@interface DigitPadViewController ()
@end

@implementation DigitPadViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self invalidateNavBar];
    self.actionButton.enabled = FALSE;
    numberIndex = 0;
    self.number = @"";

    [self.label setBottomBorder:[[SettingsManager sharedManager] color10] width:2.f alpha:.5f];
}

-(void) enableNumButtons {
    for(UIButton * button in self.numButtonsCollection) {
        button.enabled = TRUE;
    }
    self.actionButton.enabled = FALSE;
}

-(void) disableNumButtons {
    for(UIButton * button in self.numButtonsCollection) {
        button.enabled = FALSE;
    }
    self.actionButton.enabled = YES;
}

- (IBAction)clearAction:(id)sender {
    numberIndex = 0;
    self.number = @"";
    self.label.text = @"";
    [self enableNumButtons];
}

- (IBAction)numberSelectedAction:(id)sender {
    UIButton * button = (UIButton *) sender;
    self.number = [self.number stringByAppendingString:button.titleLabel.text];

    if (++numberIndex >= max) {
        [self disableNumButtons];
    }
}

- (IBAction)logInAction:(id)sender
{
    
}

@end
