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


#import "ErrorHandler.h"
#import "ATMHud.h"

@interface CVXATMHud ( )

@end

@implementation CVXATMHud

- ( instancetype )init
{
    if ( ( self = [super init] ) )
    {}
    
    return self;
}

@end


@interface ErrorHandler ( ) {}

@end

@implementation ErrorHandler

+ ( ErrorHandler * )sharedManager
{
    static ErrorHandler *sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^ {
        sharedManager = [[self alloc] init];
    });
    
    return sharedManager;
}

- ( instancetype )init
{
    self = [super init];
    if ( self )
    {
        self.hud = [[CVXATMHud alloc] initWithDelegate:self];
    }
    
    return self;
}

-( void ) startLoadingInController:( UIViewController * )viewController message:( NSString * )message
{
    [_hud setActivity:YES];
    [_hud setCaption:message];
    [_hud showInView:viewController.view];
}

-( void ) stopLoading
{
    [self hideMessage];
}

- ( void ) updateMessage:( NSString * ) strMessage
    addActivityIndicator:( BOOL )addActivityIndicator
               hideAfter:( NSInteger ) hideAfter
{
    NSLog(@"%f",_hud.minShowTime);
    _hud.minShowTime = hideAfter;
    if ( addActivityIndicator )
    {
        [_hud setActivity:YES];
    }
    else
    {
        [_hud setActivity:NO];
    }
    [_hud setCaption:strMessage];
    [_hud update];
    if ( hideAfter > 0 )
    {
        [self performSelector:@selector( hideMessage ) withObject:nil afterDelay:hideAfter];
    }
}

-( void ) presentMessageInViewController:( UIViewController * )viewController
                             errorString:( NSString * )strError
                    addActivityIndicator:( BOOL )addActivityIndicator
                             minShowTime:( NSInteger ) seconds
{
    NSLog(@"%f",_hud.minShowTime);
    _hud.center = CGPointMake([[UIScreen mainScreen] bounds].size.width / 2, [[UIScreen mainScreen] bounds].size.height / 4);
    _hud.minShowTime = seconds;
    [_hud setCaption:strError];
    
    if ( addActivityIndicator )
    {
        [_hud setActivity:YES];
    }
    else
    {
        [_hud setActivity:NO];
    }
    
    [_hud showInView:viewController.view];
    
    if ( seconds > 0 )
    {
        [_hud hide];
    }
}

-( void ) hideMessage
{
    [_hud hide];
    NSLog(@"Hiding hud");
}

@end