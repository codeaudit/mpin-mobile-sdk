//

//  QREditorViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 21.Jul.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "QREditorViewController.h"
#import "ConfigListTableViewCell.h"
#import "ConfigurationManager.h"
#import "Utilities.h"

@interface QREditorViewController ( )
{
    NSMutableArray *arrConfigurationsToImport;
}

- ( IBAction )saveConfigs:( id )sender;
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
    [_tblView reloadData];
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
        [[[UIAlertView alloc] initWithTitle:@"Warning"
          message:@"Some of the configurations will be overwritten. Please confirm."
          delegate:self
          cancelButtonTitle:@"Cancel"
          otherButtonTitles:@"OK", nil]
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

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    switch (buttonIndex) {
        case 0:
            NSLog(@"0");
            break;
        default:
            [self save];

            break;
    }
}

@end
