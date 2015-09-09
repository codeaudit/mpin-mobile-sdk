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
#import "HelpDataView.h"



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
    HelpDataView *helpView = (HelpDataView *)view;

    NSLog(@"Loading page at index %ld", (long)index);

    if ( helpView == nil )
    {
        helpView = [[HelpDataView alloc] initWithFrame:self.swipeView.bounds];
        [[ThemeManager sharedManager] customiseHelpView:helpView];
    }

    switch ( _helpMode )
    {
    case HELP_SERVER:

        return [self setupServer:helpView atIndex:index];

    case HELP_QUICK_START:

        return [self setupQuickStart:helpView atIndex:index];

    case HELP_AN:

        return [self setupAN:helpView atIndex:index];
    }

    return helpView;
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

- ( HelpDataView * ) setupQuickStart:( HelpDataView * )helpView atIndex:( NSInteger )index
{
    helpView.pageControl.numberOfPages = 4;
    switch ( index )
    {
    case 0:
        [helpView.imgArt setImage:[UIImage imageNamed:@"Guide0"]];
        helpView.lblSubTitle.text = @"Create an identity";
        helpView.lblDesc.text = @"Enter your email to register.";
        [helpView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [helpView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [helpView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 1:
        [helpView.imgArt setImage:[UIImage imageNamed:@"Guide1"]];
        helpView.lblSubTitle.text = @"Confirm your email";
        helpView.lblDesc.text = @"Click the link in the email and you are ready to choose your PIN.";
        [helpView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [helpView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [helpView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 2:
        [helpView.imgArt setImage:[UIImage imageNamed:@"Guide2"]];
        helpView.lblSubTitle.text = @"Create your PIN";
        helpView.lblDesc.text = @"It's much simpler than a password and more secure.";
        [helpView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [helpView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [helpView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 3:
        [helpView.imgArt setImage:[UIImage imageNamed:@"Guide3"]];
        helpView.lblSubTitle.text = @"You are ready to go!";
        helpView.lblDesc.text = @"You can now use your M-Pin identity any time you want.";
        [helpView.btnNext setTitle:@"DONE" forState:UIControlStateNormal];
        [helpView.btnNext setImage:nil forState:UIControlStateNormal];
        [helpView.btnNext addTarget:self action:@selector( done: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    default:
        break;
    }
    [helpView.btnSkip addTarget:self action:@selector( skip: ) forControlEvents:UIControlEventTouchUpInside];
    helpView.pageControl.currentPage = index;
    helpView.lblTitle.text = @"Setup your phone to use M-Pin";

    return helpView;
}

- ( HelpDataView * ) setupServer:( HelpDataView * )helpView atIndex:( NSInteger )index
{
    helpView.pageControl.numberOfPages = 2;
    switch ( index )
    {
    case 0:
        [helpView.imgArt setImage:[UIImage imageNamed:@"Guide4"]];
        helpView.lblSubTitle.text = @"Download and setup your own M-Pin Server";
        helpView.lblDesc.text = @"Visit certivox.com/products, choose from M-Pin Core or M-Pin SSO, then follow the online installation instructions.";
        [helpView.btnNext setTitle:@"" forState:UIControlStateNormal];
        [helpView.btnNext setImage:[UIImage imageNamed:@"arrow-right-white"] forState:UIControlStateNormal];
        [helpView.btnNext addTarget:self action:@selector( next: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    case 1:
        [helpView.imgArt setImage:[UIImage imageNamed:@"Guide5"]];
        helpView.lblSubTitle.text = @"Add your server to this app";
        helpView.lblDesc.text = @"Simply hit “+” on the next screen then enter the name and URL of your M-Pin Server and you are ready to authenticate to your service using this app.";
        [helpView.btnNext setImage:nil forState:UIControlStateNormal];
        [helpView.btnNext setTitle:@"DONE" forState:UIControlStateNormal];
        [helpView.btnNext addTarget:self action:@selector( done: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    default:
        break;
    }
    [helpView.btnSkip addTarget:self action:@selector( skip: ) forControlEvents:UIControlEventTouchUpInside];
    helpView.pageControl.currentPage = index;
    helpView.lblTitle.text = @"Get your own M-Pin Server";

    return helpView;
}

- ( HelpDataView * ) setupAN:( HelpDataView * )helpView atIndex:( NSInteger )index
{
    helpView.pageControl.numberOfPages = 1;
    switch ( index )
    {
    case 0:
        [helpView.imgArt setImage:[UIImage imageNamed:@"Guide6"]];
        helpView.lblSubTitle.text = @"Get your Access Number";
        helpView.lblDesc.text = @"Login to discuss.certivox.com on your desktop browser and choose \"Sign in with phone\".";
        [helpView.btnNext setTitle:@"DONE" forState:UIControlStateNormal];
        [helpView.btnNext addTarget:self action:@selector( done: ) forControlEvents:UIControlEventTouchUpInside];

        break;

    default:
        break;
    }
    [helpView.btnSkip addTarget:self action:@selector( skip: ) forControlEvents:UIControlEventTouchUpInside];
    helpView.pageControl.currentPage = index;
    helpView.lblTitle.text = @"Login to the Certivox community";

    return helpView;
}

@end

