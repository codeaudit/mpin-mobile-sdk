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

#import "HelpViewController.h"
#import "Constants.h"
#import "MenuViewController.h"
#import "MFSideMenu.h"
#import "SwipeView.h"
#import "ThemeManager.h"

@interface HelpDataView ( )
{}
@end

@implementation HelpDataView

@end



@interface NSArray ( Helper )
+( Boolean ) isEmpty:( NSArray * ) arr;
@end

@implementation NSArray ( Helper )
+( Boolean ) isEmpty:( NSArray * ) arr
{
    if ( arr == nil )
        return YES;

    return ( [arr count] == 0 );
}

@end


@interface HelpViewController ( )
{}
@end

@implementation HelpViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
}

- ( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    _swipeView.pagingEnabled = YES;
    [[ThemeManager sharedManager] beautifyViewController:self];
    self.view.backgroundColor = [[SettingsManager sharedManager] color0];
    [self.navigationController setNavigationBarHidden:YES animated:NO];
    [_swipeView reloadData];
    [_swipeView scrollToItemAtIndex:0 duration:0];
}

#pragma mark - Swipe view -


- ( NSInteger )numberOfItemsInSwipeView:( SwipeView * )swipeView
{
    switch ( _helpMode )
    {
    case HELP_SERVER:
    {
        return 2;

        break;
    }

    case HELP_QUICK_START:
    {
        return 4;

        break;
    }

    case HELP_AN:
    {
        return 1;

        break;
    }

    default:
    {
        return 0;

        break;
    }
    }
}

- ( UIView * )swipeView:( SwipeView * )swipeView viewForItemAtIndex:( NSInteger )index reusingView:( UIView * )view
{
    HelpDataView *arView = (HelpDataView *)view;
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenHeight = screenRect.size.height;
    CGFloat screenWidth = screenRect.size.width;

    NSLog(@"Loading page at index %ld", (long)index);

    if ( arView == nil )
    {
        arView = [[HelpDataView alloc] initWithFrame:self.swipeView.bounds];
        arView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;

        arView.lblTitle = [[UILabel alloc] initWithFrame:CGRectMake(10, 10, screenWidth - 20, 50)];
        arView.lblTitle.textColor = [UIColor redColor];
        arView.lblTitle.textAlignment = NSTextAlignmentCenter;
        arView.lblTitle.backgroundColor = [[SettingsManager sharedManager] color0];
        [arView addSubview:arView.lblTitle];

        arView.lblSubTitle = [[UILabel alloc] initWithFrame:CGRectMake(10, ceil(screenHeight / 3) + 80, screenWidth - 20, 50)];
        arView.lblSubTitle.textColor = [UIColor redColor];
        arView.lblSubTitle.textAlignment = NSTextAlignmentCenter;
        arView.lblSubTitle.backgroundColor = [UIColor whiteColor];
        [arView addSubview:arView.lblSubTitle];

        arView.lblDesc = [[UILabel alloc] initWithFrame:CGRectMake(10, ceil(screenHeight / 3) + 140, screenWidth - 20, screenHeight - ceil(screenHeight / 3) - 60 - 60 - 60 - 60 - 60)];
        arView.lblDesc.textColor = [UIColor redColor];
        arView.lblDesc.textAlignment = NSTextAlignmentCenter;
        arView.lblDesc.backgroundColor = [[SettingsManager sharedManager] color0];;
        [arView addSubview:arView.lblDesc];

        arView.btnSkip = [[UIButton alloc] initWithFrame:CGRectMake(0, screenHeight - 55 - 20, screenWidth / 2, 55)];
        arView.btnSkip.backgroundColor = [[SettingsManager sharedManager] color0];
        [arView.btnSkip setTitle:@"SKIP" forState:UIControlStateNormal];
        [arView.btnSkip setTitleColor:[[SettingsManager sharedManager] color10] forState:UIControlStateNormal];
        [arView.btnSkip.titleLabel setFont:[UIFont fontWithName:@"OpenSans" size:18.f]];
        [arView.btnSkip addTarget:self action:@selector( skip: ) forControlEvents:UIControlEventTouchUpInside];
        [arView addSubview:arView.btnSkip];

        arView.btnNext = [[UIButton alloc] initWithFrame:CGRectMake(screenWidth / 2, screenHeight - 55 - 20, screenWidth / 2, 55)];
        arView.btnNext.backgroundColor = [[SettingsManager sharedManager] color10];
        [arView addSubview:arView.btnNext];



        arView.imgArt = [[UIImageView alloc] initWithFrame:CGRectMake( 10, 70, screenWidth - 20, ceil(screenHeight / 3) )];
        arView.imgArt.backgroundColor = [UIColor whiteColor];
        arView.imgArt.contentMode = UIViewContentModeScaleAspectFit;
        [arView addSubview:arView.imgArt];

        arView.lblDesc.textColor = [[SettingsManager sharedManager] color10];
        arView.lblSubTitle.textColor = [[SettingsManager sharedManager] color10];
        arView.lblTitle.textColor = [[SettingsManager sharedManager] color10];

        arView.lblDesc.font         = [UIFont fontWithName:@"OpenSans" size:14.f];
        arView.lblSubTitle.font     = [UIFont fontWithName:@"OpenSans" size:18.f];
        arView.lblTitle.font        = [UIFont fontWithName:@"OpenSans" size:18.f];

        arView.pageControl = [[PageControl alloc] initWithFrame:CGRectMake(10, screenHeight - 130, screenWidth - 20, 55)];
        arView.pageControl.backgroundColor = [[SettingsManager sharedManager] color0];
        arView.pageControl.pageIndicatorTintColor           = [[SettingsManager sharedManager] color4];
        arView.pageControl.currentPageIndicatorTintColor    = [[SettingsManager sharedManager] color10];
        arView.pageControl.userInteractionEnabled = NO;
        [arView addSubview:arView.pageControl];
        arView.lblTitle.numberOfLines = 0;
        arView.lblSubTitle.numberOfLines = 0;
        arView.lblDesc.numberOfLines = 0;
    }

    switch ( _helpMode )
    {
    case HELP_SERVER:

        return [self setupServer:arView atIndex:index];

    case HELP_QUICK_START:

        return [self setupQuickStart:arView atIndex:index];

    case HELP_AN:

        return [self setupAN:arView atIndex:index];
    }

    return arView;
}

- ( CGSize )swipeViewItemSize:( SwipeView * )swipeView
{
    return self.swipeView.bounds.size;
}

#pragma mark - Actions -
- ( IBAction )next:( id )sender
{
    [_swipeView scrollToItemAtIndex:_swipeView.currentItemIndex + 1 duration:0.8];
}

- ( IBAction )skip:( id )sender
{
    [self done:self];
}

- ( IBAction )done:( id )sender
{
    MenuViewController *menuVC = (MenuViewController *)self.menuContainerViewController.leftMenuViewController;
    switch ( _helpMode )
    {
    case HELP_SERVER:
    {
        [menuVC setCenterWithID:SETTINGS];
        break;
    }

    case HELP_QUICK_START:
    case HELP_AN:
    {
        [menuVC setCenterWithID:USER_LIST];
        break;
    }
    }
}

- ( HelpDataView * ) setupQuickStart:( HelpDataView * )arView atIndex:( NSInteger )index
{
    arView.pageControl.numberOfPages = 4;
    switch ( index )
    {
    case 0:
        [arView.imgArt setImage:[UIImage imageNamed:@"Guide0"]];
        arView.lblSubTitle.text = @"Create an identity";
        arView.lblDesc.text = @"Enter your email to register.";
        [arView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [arView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [arView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 1:
        [arView.imgArt setImage:[UIImage imageNamed:@"Guide1"]];
        arView.lblSubTitle.text = @"Confirm your email";
        arView.lblDesc.text = @"Click the link in the email and you are ready to choose your PIN.";
        [arView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [arView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [arView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 2:
        [arView.imgArt setImage:[UIImage imageNamed:@"Guide2"]];
        arView.lblSubTitle.text = @"Create your PIN";
        arView.lblDesc.text = @"It's much simpler than a password and more secure.";
        [arView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [arView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [arView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 3:
        [arView.imgArt setImage:[UIImage imageNamed:@"Guide3"]];
        arView.lblSubTitle.text = @"You are ready to go!";
        arView.lblDesc.text = @"You can now use your M-Pin identity any time you want.";
        [arView.btnNext setTitle:@"DONE" forState:UIControlStateNormal];
        [arView.btnNext setImage:nil forState:UIControlStateNormal];
        [arView.btnNext addTarget:self action:@selector( done: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    default:
        break;
    }
    arView.pageControl.currentPage = index;
    arView.lblTitle.text = @"Setup your phone to use M-Pin";

    return arView;
}

- ( HelpDataView * ) setupServer:( HelpDataView * )arView atIndex:( NSInteger )index
{
    arView.pageControl.numberOfPages = 2;
    switch ( index )
    {
    case 0:
        [arView.imgArt setImage:[UIImage imageNamed:@"Guide4"]];
        arView.lblSubTitle.text = @"Download and setup your own M-Pin Server";
        arView.lblDesc.text = @"Visit certivox.com/products, choose from M-Pin Core or M-Pin SSO, then follow the online installation instructions.";
        [arView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [arView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [arView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 1:
        [arView.imgArt setImage:[UIImage imageNamed:@"Guide5"]];
        arView.lblSubTitle.text = @"Add your server to this app";
        arView.lblDesc.text = @"Simply hit “+” on the next screen then enter the name and URL of your M-Pin Server and you are ready to authenticate to your service using this app.";
        [arView.btnNext setImage:nil forState:UIControlStateNormal];
        [arView.btnNext setTitle:@"DONE" forState:UIControlStateNormal];
        [arView.btnNext addTarget:self action:@selector( done: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    default:
        break;
    }
    arView.pageControl.currentPage = index;
    arView.lblTitle.text = @"Get your own M-Pin Server";

    return arView;
}

- ( HelpDataView * ) setupAN:( HelpDataView * )arView atIndex:( NSInteger )index
{
    arView.pageControl.numberOfPages = 1;
    switch ( index )
    {
    case 0:
        [arView.imgArt setImage:[UIImage imageNamed:@"Guide6    "]];
        arView.lblSubTitle.text = @"Get your Access Number";
        arView.lblDesc.text = @"Login to discuss.certivox.com on your desktop browser and choose \"Sign in with phone\".";
        [arView.btnNext setTitle:@"DONE" forState:UIControlStateNormal];
        [arView.btnNext addTarget:self action:@selector( done: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    default:
        break;
    }
    arView.pageControl.currentPage = index;
    arView.lblTitle.text = @"Login to the Certivox community";

    return arView;
}

@end

@implementation PageControl
//
//-( id ) initWithCoder:( NSCoder * )aDecoder
//{
//    self = [super initWithCoder:aDecoder];
//
//    activeImage     = [UIImage imageNamed:@"PageIndicatorSelected"];
//    inactiveImage   = [UIImage imageNamed:@"PageIndicator"];
//
//    return self;
//}
//
//-( void ) updateDots
//{
//    for ( int i = 0; i < [self.subviews count]; i++ )
//    {
//        UIImageView *dot = self.subviews[i];
//        if ([dot isMemberOfClass:[UIView class]])
//        {
//            continue;
//        }
//        if ( i == self.currentPage )
//            dot.image = activeImage;
//        else
//            dot.image = inactiveImage;
//    }
//}
//
//-( void ) setCurrentPage:( NSInteger )page
//{
//    [super setCurrentPage:page];
//    [self updateDots];
//}

@end
