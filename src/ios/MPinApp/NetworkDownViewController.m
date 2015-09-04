//
//  NetworkDownViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 6.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NetworkDownViewController.h"
#import "ThemeManager.h"
#import "AppDelegate.h"

@interface NetworkDownViewController ( )

@end

@implementation NetworkDownViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

-( void ) viewWillAppear:( BOOL )animated
{
    [super  viewWillAppear:animated];
    [self   registerObservers];
    [[ThemeManager sharedManager] beautifyViewController:self];
    _lblMessage.text = NSLocalizedString(@"CONNECTION_WAS_LOST", @"No Internet Connection");
}

- ( void )viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
    [self unRegisterObservers];
}

- ( void )didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
   #pragma mark - Navigation

   // In a storyboard-based application, you will often want to do a little preparation before navigation
   - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
   }
 */

#pragma mark - NSNotification handlers -

-( void ) networkUp
{
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    [appDelegate connectionUp];
}

-( void ) networkDown
{
    NSLog(@"Network DOWN Notification");
}

-( void ) unRegisterObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_UP_NOTIFICATION" object:nil];
}

- ( void ) registerObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkUp ) name:@"NETWORK_UP_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkDown ) name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
}

@end
