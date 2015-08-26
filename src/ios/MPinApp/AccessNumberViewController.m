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

#import "AccessNumberViewController.h"
#import "BackButton.h"
#import "UIView+Helper.m"
#import "ThemeManager.h"
#import "SettingsManager.h"
#import "MPin.h"

const NSString *constStrAccessNumberLenghtKey = @"accessNumberDigits";
const NSString *constStrAccessNumberUseCheckSum = @"accessNumberUseCheckSum";

@interface AccessNumberViewController ( )
{
    int intAccessNumberLenght;
    MPin *sdk;
}

- ( void ) clear;
-( IBAction )btnBackTap:( id )sender;


@end

@implementation AccessNumberViewController

- ( void ) viewDidLoad
{
    [super viewDidLoad];
    BackButton *btnBack = [[BackButton alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:self action:@selector( btnBackTap: )];
    [btnBack setup];
    self.navigationItem.leftBarButtonItem = btnBack;
}

-( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    sdk = [[MPin alloc] init];
    sdk.delegate = self;
    [self clearAction:self];
    [[NSNotificationCenter defaultCenter] removeObserver:self
     name:kShowPinPadNotification
     object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad: ) name:kShowPinPadNotification object:nil];
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

-( void ) viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- ( IBAction )logInAction:( id )sender
{
    if ( ( self.delegate != nil ) && ( [self.delegate respondsToSelector:@selector( onAccessNumber: )] ) )
    {
        if ( [self.strNumber isEqualToString:@""] )
        {
            [[ErrorHandler sharedManager] presentMessageInViewController:self
             errorString:NSLocalizedString(@"ERROR_WRONG_AN", @"")
             addActivityIndicator:NO
             minShowTime:3];

            [self clear];

            return;
        }
    }

    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"" addActivityIndicator:YES minShowTime:0];
    [sdk AuthenticateAN:_currentUser accessNumber:self.strNumber askForFingerprint:NO];
}

-( IBAction )btnBackTap:( id )sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- ( IBAction )numberSelectedAction:( id )sender
{
    if ( [self.strNumber length] >= intAccessNumberLenght )
    {
        return;
    }
    UIButton *button = (UIButton *) sender;
    if ( ++numberIndex >= max )
    {
        [self disableNumButtons];
    }
    self.strNumber = [self.strNumber stringByAppendingString:button.titleLabel.text];
    self.txtAN.text =  [NSString stringWithFormat:@"%@ %@",self.txtAN.text, button.titleLabel.text];
}

- ( void ) clear
{
    numberIndex = 0;
    self.strNumber = @"";
    [self enableNumButtons];
    _txtAN.text = @"";
}

- ( IBAction )clearAction:( id )sender
{
    [self clear];
}

- ( void ) OnAuthenticateAccessNumberCompleted:( id ) sender user:( id<IUser>) user
{}

- ( void )OnAuthenticateAccessNumberError:( id )sender error:( NSError * )error
{
    NSLog(@"OnAuthenticateAccessNumberError");
    NSLog(@"%@", error.description);
    MpinStatus *mpinStatus = [error.userInfo objectForKey:kMPinSatus];
    [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(mpinStatus.statusCodeAsString, mpinStatus.errorMessage) addActivityIndicator:NO hideAfter:3];
    [self clearAction:self];
    [MPin sendPin:kEmptyStr];
}

- ( void )showPinPad:(NSNotification *)notification
{
    [[ErrorHandler sharedManager] hideMessage];
    PinPadViewController *pinpadViewController = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"pinpad"];

    pinpadViewController.strAccessNumber = self.strNumber;
    pinpadViewController.currentUser = _currentUser;
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    sdk.delegate = pinpadViewController;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

@end
