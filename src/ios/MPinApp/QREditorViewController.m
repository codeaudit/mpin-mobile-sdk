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

#import "QREditorViewController.h"
#import "ConfigListTableViewCell.h"
#import "ConfigurationManager.h"
#import "Utilities.h"
#import "ThemeManager.h"

@interface QREditorViewController ( )
{
    NSMutableArray *arrConfigurationsToImport;
}

- ( IBAction )saveConfigs:( id )sender;
-( IBAction )close:( id )sender;

@end

@implementation QREditorViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- ( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [self registerObservers];
    [_tblView reloadData];
}

- (void) viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self unRegisterObservers];
}

-( void ) viewDidAppear:( BOOL )animated
{
    [super viewDidAppear:animated];
    [[ThemeManager sharedManager] beautifyViewController:self];
}

- ( void )didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-( void ) setArrConfigs:( NSArray * )arrQRConfigs
{
    _arrQRConfigs = arrQRConfigs;
    arrConfigurationsToImport = [NSMutableArray arrayWithCapacity:_arrQRConfigs.count];
    for ( int i = 0; i < _arrQRConfigs.count; i++ )
    {
        arrConfigurationsToImport [i] = @1;
    }
}

#pragma mark - Table view delegate -

- ( CGFloat )tableView:( UITableView * )tableView heightForRowAtIndexPath:( NSIndexPath * )indexPath
{
    return 60.f;
}

- ( NSInteger )tableView:( UITableView * )tableView numberOfRowsInSection:( NSInteger )section
{
    return _arrQRConfigs.count;
}

- ( UITableViewCell * )tableView:( UITableView * )tableView cellForRowAtIndexPath:( NSIndexPath * )indexPath
{
    static NSString *QRListTableViewCell = @"QRListTableViewCell";
    ConfigListTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:QRListTableViewCell];
    if ( cell == nil )
        cell = [[ConfigListTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:QRListTableViewCell];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.lblConfigurationName.font = [UIFont fontWithName:@"OpenSans" size:16.f];
    cell.lblConfigurationType.font = [UIFont fontWithName:@"OpenSans" size:14.f];


    return cell;
}

- ( void )tableView:( UITableView * )tableView willDisplayCell:( UITableViewCell * )cell forRowAtIndexPath:( NSIndexPath * )indexPath
{
    ConfigListTableViewCell *c = (ConfigListTableViewCell *)cell;
    c.lblConfigurationName.text = _arrQRConfigs [indexPath.row] [@"name"];
    c.lblConfigurationType.text = _arrQRConfigs [indexPath.row] [@"type"];
    [c.imgViewSelected setImage:[UIImage imageNamed:@"checked"]];
    NSNumber *number = arrConfigurationsToImport [indexPath.row];
    BOOL boolShouldImport = [number boolValue];
    [c setIsSelectedImage:boolShouldImport];

    NSInteger isExisting = [[ConfigurationManager sharedManager] configurationExists:[_arrQRConfigs objectAtIndex:indexPath.row]];

    if ( isExisting > -1 )
    {
        c.lblConfigurationName.textColor = [UIColor redColor];
    }
    else
    {
        c.lblConfigurationName.textColor = [UIColor blackColor];
    }
}

- ( void )tableView:( UITableView * )tableView didSelectRowAtIndexPath:( NSIndexPath * )indexPath
{
    NSNumber *number = arrConfigurationsToImport [indexPath.row];
    BOOL boolShouldImport = [number boolValue];
    if ( boolShouldImport )
    {
        arrConfigurationsToImport [indexPath.row] = @"0";
    }
    else
    {
        arrConfigurationsToImport [indexPath.row] = @"1";
    }
    [_tblView reloadData];
}

#pragma mark - My actions -

- ( IBAction )saveConfigs:( id )sender
{
    BOOL boolShouldConfirm = NO;
    for ( int i = 0; i < [_arrQRConfigs count]; i++ )
    {
        NSNumber *number = arrConfigurationsToImport [i];
        BOOL boolShouldImport = [number boolValue];

        if ( !boolShouldImport )
        {
            continue;
        }

        NSInteger index = [[ConfigurationManager sharedManager] configurationExists:_arrQRConfigs [i]];
        if ( index > -1 )
        {
            boolShouldConfirm = YES;
            break;
        }
    }

    if ( boolShouldConfirm )
    {
        [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"WARNING",@"Warning")
          message:NSLocalizedString(@"WARNING_OVERWRITING_CONFIGURATIONS", @"Some of the configurations will be overwritten. Please confirm.")
          delegate:self
          cancelButtonTitle:NSLocalizedString(@"KEY_CANCEL", @"Cancel")
          otherButtonTitles:NSLocalizedString(@"KEY_OK",  @"OK"), nil]
         show];
    }
    else
    {
        [self save];
    }
}

- ( void ) save
{
    for ( int i = 0; i < [_arrQRConfigs count]; i++ )
    {
        NSNumber *number = arrConfigurationsToImport [i];
        BOOL boolShouldImport = [number boolValue];

        if ( !boolShouldImport )
        {
            continue;
        }

        NSInteger indexOfConfiguration = [[ConfigurationManager sharedManager] configurationExists:_arrQRConfigs [i]];

        if ( indexOfConfiguration > -1 )
        {
            [[ConfigurationManager sharedManager] saveConfigurationAtIndex:indexOfConfiguration
             url:[[_arrQRConfigs objectAtIndex:i] valueForKey:kJSON_URL]
             serviceType:[Utilities ServerJSONConfigTypeToService_type:[[_arrQRConfigs objectAtIndex:i] valueForKey:kJSON_TYPE]]
             name:[[_arrQRConfigs objectAtIndex:i] valueForKey:kJSON_NAME]];
        }
        else
        {
            [[ConfigurationManager sharedManager] addConfiguration:[[_arrQRConfigs objectAtIndex:i] valueForKey:kJSON_URL]
             serviceType:[Utilities ServerJSONConfigTypeToService_type:[[_arrQRConfigs objectAtIndex:i] valueForKey:kJSON_TYPE]]
             name:[[_arrQRConfigs objectAtIndex:i] valueForKey:kJSON_NAME]
             prefixName:[[_arrQRConfigs objectAtIndex:i] valueForKey:kJSON_PREFIX]
            ];
        }
    }
    [[ConfigurationManager sharedManager] saveConfigurations];
    [self.navigationController popToRootViewControllerAnimated:YES];
}

#pragma mark - Alert view delegate -

- ( void )alertView:( UIAlertView * )alertView didDismissWithButtonIndex:( NSInteger )buttonIndex
{
    switch ( buttonIndex )
    {
    case 0:

        break;

    default:
        [self save];

        break;
    }
}

-( IBAction )close:( id )sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

#pragma mark - NSNotification handlers -

-( void ) networkUp
{
    [[ThemeManager sharedManager] hideNetworkDown:self];
}

-( void ) networkDown
{
    NSLog(@"Network DOWN Notification");

    [self.view layoutIfNeeded];
    [UIView animateWithDuration:kFltNoNetworkMessageAnimationDuration animations: ^ {
        self.constraintNoNetworkViewHeight.constant = 36.0f;
        [self.view layoutIfNeeded];
    }];
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
