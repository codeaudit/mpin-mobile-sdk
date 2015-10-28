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
