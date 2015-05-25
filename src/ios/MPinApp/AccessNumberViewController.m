//
//  AccessNumberViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 1/22/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "AccessNumberViewController.h"
#import "BackButton.h"
#import "UIView+Helper.m"
#import "ThemeManager.h"
#import "SettingsManager.h"
#import "MPin.h"

const NSString *constStrAccessNumberLenghtKey = @"accessNumberDigits";
const NSString *constStrAccessNumberUseCheckSum = @"accessNumberUseCheckSum";

@interface AccessNumberViewController ()
{
    int intAccessNumberLenght;
}

- (void) clear;
-(IBAction)btnBackTap:(id)sender;


@end

@implementation AccessNumberViewController

- (void) viewDidLoad
{
    [super viewDidLoad];
    
    BackButton *btnBack = [[BackButton alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:self action:@selector(btnBackTap:)];
    [btnBack setup];
    self.navigationItem.leftBarButtonItem = btnBack;
}

-(void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [[ThemeManager sharedManager] beautifyViewController:self];
    _lblEmail.text = _strEmail;
    _txtAN.text = @"";
    [_txtAN setBottomBorder:[[SettingsManager sharedManager] color7] width:2.f alpha:.5f];
    NSString *strANLenght       = [MPin GetClientParam:constStrAccessNumberLenghtKey];
    
    intAccessNumberLenght = [strANLenght intValue];
    max = intAccessNumberLenght;
    self.title = NSLocalizedString(@"ACCESSNUMBERVC_TITLE", @"");
    _lblNote.text = [NSString stringWithFormat:NSLocalizedString(@"ACCESSNUMBERVC_NOTE", @""), intAccessNumberLenght];
    
}

- (IBAction)logInAction:(id)sender {
    if( ( self.delegate != nil ) && ( [self.delegate respondsToSelector:@selector(onAccessNumber:)]) )
    {
        if([self.number isEqualToString:@""]) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:NSLocalizedString(@"ERROR_WRONG_AN", @"") delegate:nil cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"") otherButtonTitles:nil, nil];
            [self clear];
            [alert show];
            return;
        }
        [self.delegate onAccessNumber:self.number];
    }
    [self.navigationController popViewControllerAnimated:NO];
}

-(IBAction)btnBackTap:(id)sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- (IBAction)numberSelectedAction:(id)sender
{
    if ([self.number length] >= intAccessNumberLenght)
    {
        return;
    }
    UIButton * button = (UIButton *) sender;
    if (++numberIndex >= max) {
        [self disableNumButtons];
    }
    self.number = [self.number stringByAppendingString:button.titleLabel.text];
    self.txtAN.text =  [NSString stringWithFormat:@"%@ %@",self.txtAN.text, button.titleLabel.text];
}

- (void) clear {
    numberIndex = 0;
    self.number = @"";
    [self enableNumButtons];
    _txtAN.text = @"";
}

- (IBAction)clearAction:(id)sender {
    [self clear];
}
@end
