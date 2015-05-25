//
//  IdentityBlockedViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 21.Apr.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//
#import "MFSideMenu.h"
#import "IdentityBlockedViewController.h"
#import "ThemeManager.h"
#import "MPin.h"

@interface IdentityBlockedViewController ()

- (IBAction)showLeftMenuPressed:(id)sender;
- (IBAction)btnGoToIdListPressed:(id)sender;
- (IBAction)btnDeleteIdPressed:(id)sender;

@end

@implementation IdentityBlockedViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [[ThemeManager sharedManager] beautifyViewController:self];
}

-(void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (_strUserEmail != nil)
    {
        _lblUserEmail.text = _strUserEmail;
    }
    else
    {
        _lblUserEmail.text = @"";
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)showLeftMenuPressed:(id)sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- (IBAction)btnGoToIdListPressed:(id)sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}


- (IBAction)btnDeleteIdPressed:(id)sender
{
   [[[UIAlertView alloc] initWithTitle:@"REMOVE IDENTITY" message:@"This action will remove the identity permanently.  Are you shure?" delegate:self cancelButtonTitle:@"NO" otherButtonTitles:@"YES", nil] show];
    
}

#pragma mark - Alert view delegate -

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        [MPin DeleteUser:_iuser];
        [self.navigationController popToRootViewControllerAnimated:YES];
    }
}

@end
