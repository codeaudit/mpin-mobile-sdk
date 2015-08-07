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



#import "OTP.h"
#import "OTPViewController.h"
#import "CycleProgressBar.h"
#import "ThemeManager.h"
#import "BackButton.h"

@interface OTPViewController ( )
{
    ThemeManager *themeManager;
    BackButton   *btnBack;
}

- ( IBAction ) back:( UIBarButtonItem * )sender;
- ( void ) onProgressBarFinish:( id )sender;

@property ( nonatomic, weak ) UIView *viewPreloaderContainer;
@property ( nonatomic, strong )   CycleProgressBar *cpb;

@end

@implementation OTPViewController
- ( void )viewDidLoad
{
    [super viewDidLoad];
    [[ThemeManager sharedManager] beautifyViewController:self];
    NSMutableString *strOtp = [NSMutableString stringWithString:@""];
    for ( int i = 0; i < [self.otpData.otp length]; i++ )
    {
        [strOtp appendString:[self.otpData.otp substringWithRange:NSMakeRange(i, 1)]];
        [strOtp appendString:@" "];
    }
    self.lblOTP.text = strOtp;
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    self.cpb = [[CycleProgressBar alloc] initWithFrame:CGRectMake(screenRect.size.width / 2 - 90.0, ( 2 * screenRect.size.height ) / 3 - 90.0, 180.0, 180.0)];
    [self.cpb addTarget:self action:@selector( onProgressBarFinish: ) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:self.cpb];
    btnBack = [[BackButton alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:self action:@selector( back: )];
}

- ( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];

    [self.cpb startAnimation:self.otpData.ttlSeconds];
    _lblEmail.text = _strEmail;
    [btnBack setup];
    self.navigationItem.leftBarButtonItem = btnBack;
    self.title = NSLocalizedString(@"OTPVC_TITLE", @"");
    self.lblMessage.text = NSLocalizedString(@"OTPVC_LBL_MESSAGE", @"");
    self.lblYourPassword.text = NSLocalizedString(@"OTPVC_LBL_YOUR_PASSWORD", @"");
}

- ( void ) viewDidDisappear:( BOOL )animated
{
    [super viewDidDisappear:animated];
    [self.cpb stopAnimation];
}

- ( void ) onProgressBarFinish:( id )sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

-( IBAction )OnClickNavButton:( id )sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- ( IBAction ) back:( UIBarButtonItem * )sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

@end
