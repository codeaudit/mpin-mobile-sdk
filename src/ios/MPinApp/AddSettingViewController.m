//
//  AddSettingViewController.m
//  MPinApp
//
//  Created by Certivox Developer on 1/20/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "AddSettingViewController.h"
#import "AppDelegate.h"
#import "TextFieldTableViewCell.h"
#import "OptionSelectTableViewCell.h"
#import "ConfigurationManager.h"
#import "ThemeManager.h"
#import "MpinStatus.h"
#import "MPin.h"
#import "ThemeManager.h"
#import "SettingsManager.h"
#import "NSString+Helper.h"

static NSString *const kErrorTitle = @"Validation ERROR!";

@interface AddSettingViewController ( ) {
    MPin *sdk;
    BOOL bTestingConfig;
    NSString *strURL;
}


@property ( nonatomic, strong ) UITextField *txtMPINServiceNAME;
@property ( nonatomic, strong ) UITextField *txtMPINServiceURL;
@property ( nonatomic, strong ) UITextField *txtMPINServiceRPSPrefix;
@property ( nonatomic, strong ) UIImageView *imgViewLoginToBrowser;
@property ( nonatomic, strong ) UIImageView *imgViewRequestOTP;
@property ( nonatomic ) NSInteger intConfigurationType;
@property ( nonatomic ) enum SERVICES service;

@property ( nonatomic, assign ) id currentResponder;

- ( IBAction )goBack:( id )sender;
- ( IBAction )textFieldReturn:( id )sender;
- ( NSString * ) getTXTMPINServiceRPSPrefix;

@end

@implementation AddSettingViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
    _service = LOGIN_ONLINE;
    [[ThemeManager sharedManager] beautifyViewController:self];
    UITapGestureRecognizer *singleTap =
        [[UITapGestureRecognizer alloc] initWithTarget:self
         action:@selector( resignOnTap: )];
    [singleTap setNumberOfTapsRequired:1];
    [singleTap setNumberOfTouchesRequired:1];
    singleTap.cancelsTouchesInView = NO;
    [self.view addGestureRecognizer:singleTap];


    self.title = NSLocalizedString(@"ADDCONFIGVC_TITLE",@"");
    [_btnTestConfig setTitle:NSLocalizedString(@"ADDCONFIGVC_BTN_TEST_CONFIG", @"") forState:UIControlStateNormal];
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;


    if ( _isEdit )
    {
        self.title = NSLocalizedString(@"ADDCONFIGVC_TITLE_EDIT", @"");
        _service = (int)[[ConfigurationManager sharedManager] getConfigurationTypeAtIndex:_selectedIndex];
        strURL = [[ConfigurationManager sharedManager] getURLAtIndex:_selectedIndex];
    }
    else
    {
        self.title = NSLocalizedString(@"ADDCONFIGVC_TITLE_ADD", @"");
    }
    bTestingConfig = NO;
    _btnDone.title = NSLocalizedString(@"KEY_DONE", @"");
}

#pragma mark - text field delegates -

- ( BOOL )textFieldShouldReturn:( UITextField * )textField
{
    [textField resignFirstResponder];

    return YES;
}

- ( void )textFieldDidBeginEditing:( UITextField * )textField
{
    self.currentResponder = textField;
}

- ( BOOL )textFieldShouldBeginEditing:( UITextField * )textField
{
    return YES;
}

#pragma mark -  Table view delegate -

- ( CGFloat )tableView:( UITableView * )tableView
    heightForRowAtIndexPath:( NSIndexPath * )indexPath
{
    return 70;
}

- ( NSInteger )tableView:( UITableView * )tableView
    numberOfRowsInSection:( NSInteger )section;
{
    return 5;
}

- ( UITableViewCell * )tableView:( UITableView * )tableView cellForRowAtIndexPath:( NSIndexPath * )indexPath
{
    switch ( indexPath.row )
    {
    case 0:
    {
        TextFieldTableViewCell *cell0 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell0"];
        if ( cell0 == nil )
            cell0 = [[TextFieldTableViewCell alloc]
                     initWithStyle:UITableViewCellStyleDefault
                     reuseIdentifier:@"ConfigCell0"];
        [cell0.txtText setBottomBorder:[[SettingsManager sharedManager] color5]
         width:2.f
         alpha:.5f];
        _txtMPINServiceNAME = cell0.txtText;

        return cell0;
    }

    case 1:
    {
        TextFieldTableViewCell *cell0 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell0"];
        if ( cell0 == nil )
            cell0 = [[TextFieldTableViewCell alloc]
                     initWithStyle:UITableViewCellStyleDefault
                     reuseIdentifier:@"ConfigCell0"];
        [cell0.txtText setBottomBorder:[[SettingsManager sharedManager] color5]
         width:2.f
         alpha:.5f];
        _txtMPINServiceURL = cell0.txtText;

        return cell0;
    }

    case 2:
    {
        TextFieldTableViewCell *cell0 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell0"];
        if ( cell0 == nil )
            cell0 = [[TextFieldTableViewCell alloc]
                     initWithStyle:UITableViewCellStyleDefault
                     reuseIdentifier:@"ConfigCell0"];
        [cell0.txtText setBottomBorder:[[SettingsManager sharedManager] color5]
         width:2.f
         alpha:.5f];
        _txtMPINServiceRPSPrefix = cell0.txtText;

        return cell0;
    }

    case 3:
    case 4:
    {
        OptionSelectTableViewCell *cell1 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell1"];
        if ( cell1 == nil )
            cell1 = [[OptionSelectTableViewCell alloc]
                     initWithStyle:UITableViewCellStyleDefault
                     reuseIdentifier:@"ConfigCell1"];

        return cell1;
    } break;

    default:
        break;
    }

    return nil;
}

- ( void )tableView:( UITableView * )tableView willDisplayCell:( UITableViewCell * )cell forRowAtIndexPath:( NSIndexPath * )indexPath
{
    switch ( indexPath.row )
    {
    case 0:
        ( (TextFieldTableViewCell *)cell ).lblName.text = NSLocalizedString(@"ADDCONFIGVC_NAME",@"");
        ( (TextFieldTableViewCell *)cell ).txtText.placeholder = NSLocalizedString(@"ADDCONFIGVC_NAME",@"");
        if ( _isEdit )
        {
            ( (TextFieldTableViewCell *)cell ).txtText.text = [[ConfigurationManager sharedManager]
                                                               getNameAtIndex:_selectedIndex];
        }
        break;

    case 1:
        ( (TextFieldTableViewCell *)cell ).lblName.text = NSLocalizedString(@"ADDCONFIGVC_URL",@"");
        ( (TextFieldTableViewCell *)cell ).txtText.placeholder = NSLocalizedString(@"ADDCONFIGVC_URL",@"");
        if ( _isEdit )
        {
            ( (TextFieldTableViewCell *)cell ).txtText.text = strURL;
        }
        break;

    case 2:
        ( (TextFieldTableViewCell *)cell ).lblName.text = NSLocalizedString(@"ADDCONFIGVC_PREFIX",@"");
        ( (TextFieldTableViewCell *)cell ).txtText.placeholder = NSLocalizedString(@"ADDCONFIGVC_PREFIX",@"");
        if ( _isEdit )
        {
            ( (TextFieldTableViewCell *)cell ).txtText.text = [[ConfigurationManager sharedManager]
                                                               getPrefixAtIndex:_selectedIndex];
        }
        break;

//    case 3:
//        ( (OptionSelectTableViewCell *)cell ).lblName.text = NSLocalizedString(@"LOGIN_MOBILE_APP", @"");
//        switch ( _service )
//        {
//        case LOGIN_ON_MOBILE:
//            [( (OptionSelectTableViewCell *)cell )setServiceSelected:YES];
//            break;
//
//        case LOGIN_ONLINE:
//            [( (OptionSelectTableViewCell *)cell )setServiceSelected:NO];
//            break;
//
//        case LOGIN_WITH_OTP:
//            [( (OptionSelectTableViewCell *)cell )setServiceSelected:NO];
//            break;
//        }
//
//        break;

    case 3:
        ( (OptionSelectTableViewCell *)cell ).lblName.text = NSLocalizedString(@"LOGIN_ONLINE_SESSION", @"");
        switch ( _service )
        {
        case LOGIN_ON_MOBILE:
            [( (OptionSelectTableViewCell *)cell )setServiceSelected:NO];
            break;

        case LOGIN_ONLINE:
            [( (OptionSelectTableViewCell *)cell )setServiceSelected:YES];
            break;

        case LOGIN_WITH_OTP:
            [( (OptionSelectTableViewCell *)cell )setServiceSelected:NO];
            break;
        }

        break;

    case 4:
        ( (OptionSelectTableViewCell *)cell ).lblName.text = NSLocalizedString(@"LOGIN_OTP", @"");
        switch ( _service )
        {
        case LOGIN_ON_MOBILE:
            [( (OptionSelectTableViewCell *)cell )setServiceSelected:NO];
            break;

        case LOGIN_ONLINE:
            [( (OptionSelectTableViewCell *)cell )setServiceSelected:NO];
            break;

        case LOGIN_WITH_OTP:
            [( (OptionSelectTableViewCell *)cell )setServiceSelected:YES];
            break;
        }

        break;

    default:
        break;
    }
}

- ( void )tableView:( UITableView * )tableView didSelectRowAtIndexPath:( NSIndexPath * )indexPath
{
    switch ( indexPath.row )
    {
//    case 3:
//        _service = LOGIN_ON_MOBILE;
//        break;

    case 3:
        _service = LOGIN_ONLINE;
        break;

    case 4:
        _service = LOGIN_WITH_OTP;
        break;
    }
    [_tblView reloadData];
}

#pragma mark - My methods -

- ( void )resignOnTap:( id )sender
{
    [self.currentResponder resignFirstResponder];
}

- ( NSString * ) getTXTMPINServiceRPSPrefix
{
    if ( [NSString isBlank:_txtMPINServiceRPSPrefix.text] )
        return nil;

    return _txtMPINServiceRPSPrefix.text;
}

#pragma mark - Actions  -

- ( IBAction )goBack:( id )sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

- ( IBAction )btnTestConfigTap:( id )sender
{
    bTestingConfig = YES;

    if ( ![self isValidName:_txtMPINServiceNAME.text] )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ADDCONFIGVC_ERROR_EMPTY_NAME", @"")
         addActivityIndicator:NO
         minShowTime:3];
    }
    else
    if ( ![NSString isValidURL:_txtMPINServiceURL.text] )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ADDCONFIGVC_ERROR_INVALID_URL", @"")
         addActivityIndicator:NO
         minShowTime:3];
    }
    else
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"TESTING_CONFIGURATION", @"Testing configuration")
         addActivityIndicator:YES
         minShowTime:0];
        if ( [_txtMPINServiceRPSPrefix.text isEqualToString:@""] )
        {
            [sdk TestBackend:_txtMPINServiceURL.text rpsPrefix:nil];
        }
        else
        {
            [sdk TestBackend:_txtMPINServiceURL.text rpsPrefix:_txtMPINServiceRPSPrefix.text];
        }
    }
}

- ( IBAction )textFieldReturn:( id )sender
{
    [sender resignFirstResponder];
}

- ( IBAction )onSave:( id )sender
{
    _txtMPINServiceURL.text = [_txtMPINServiceURL.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    bTestingConfig = NO;

    if ( ![self isValidName:_txtMPINServiceNAME.text] )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ADDCONFIGVC_ERROR_EMPTY_NAME", @"")
         addActivityIndicator:NO
         minShowTime:3];
    }
    else
    if ( ![NSString isValidURL:_txtMPINServiceURL.text] )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ADDCONFIGVC_ERROR_INVALID_URL", @"")
         addActivityIndicator:NO
         minShowTime:3];
    }
    else
    {
        NSString *caption = @"";
        if ( _isEdit )
        {
            if ( [_txtMPINServiceURL.text isEqualToString:[[ConfigurationManager sharedManager] getURLAtIndex:_selectedIndex]] )
            {
                caption = NSLocalizedString(@"HUD_SAVE_CONFIG", @"");
            }
            else
            {
                caption = NSLocalizedString(@"HUD_SAVE_CONFIG_AND_DEL", @"");
            }
        }
        else
        {
            caption = NSLocalizedString(@"HUD_SAVE_CONFIG", @"");
        }

        [_btnDone setEnabled:NO];
        [sdk TestBackend:_txtMPINServiceURL.text rpsPrefix:[self getTXTMPINServiceRPSPrefix]];

        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:caption
         addActivityIndicator:YES
         minShowTime:0];
    }
}

#pragma mark - Mpin sdk callbacks -

- ( void )OnTestBackendCompleted:( id )sender
{
    if ( bTestingConfig )
    {
        [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"ADDCONFIGVC_MESSAGE_CONFIG_OK", @"")
         addActivityIndicator:NO
         hideAfter:3];
        bTestingConfig = NO;
        [_btnDone setEnabled:YES];
    }
    else
    {
        if ( _selectedIndex >= 0 )
        {
            if ( _isEdit )
            {
                [[ConfigurationManager sharedManager] saveConfigurationAtIndex:_selectedIndex
                 url:_txtMPINServiceURL.text
                 serviceType:(int)_service
                 name:_txtMPINServiceNAME.text
                 prefixName:[self getTXTMPINServiceRPSPrefix]];
            }
            else
            {
                [[ConfigurationManager sharedManager] addConfigurationWithURL:_txtMPINServiceURL.text
                 serviceType:(int)_service
                 name:_txtMPINServiceNAME.text
                 prefixName:[self getTXTMPINServiceRPSPrefix]];
            }
        }

        [sdk SetBackend:[[ConfigurationManager sharedManager] getSelectedConfiguration]];
    }
}

- ( void )OnTestBackendError:( id )sender error:( NSError * )error
{
    _txtMPINServiceURL.text = [_txtMPINServiceURL.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];

    NSString *message = NSLocalizedString(mpinStatus.statusCodeAsString, @"UNKNOWN ERROR");

    if ( [NSString isNotBlank:mpinStatus.errorMessage] )
    {
        message = [NSString stringWithFormat:@"%@\n%@", message, mpinStatus.errorMessage];
    }


    [[ErrorHandler sharedManager] updateMessage:[NSString stringWithFormat:@"%@", message]
     addActivityIndicator:NO
     hideAfter:3];
    [_btnDone setEnabled:YES];
}

- ( void )OnSetBackendCompleted:( id )sender
{
    [_btnDone setEnabled:YES];
    dispatch_after(dispatch_time( DISPATCH_TIME_NOW, (int64_t)( NSEC_PER_SEC ) ), dispatch_get_main_queue(), ^ {
        [self.navigationController popViewControllerAnimated:YES];
    });
}

- ( void )OnSetBackendError:( id )sender error:( NSError * )error
{
    [self OnTestBackendError:sender error:error];
}

- ( BOOL ) isValidName:( NSString * ) name
{
    if ( [name isEqualToString:@""] || [[name stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceCharacterSet]] isEqualToString:@""] )
    {
        return NO;
    }

    return YES;
}

- ( BOOL ) isValidURL:( NSString * ) url
{
    if ( [_txtMPINServiceNAME.text isEqualToString:@""] || [[_txtMPINServiceNAME.text stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceCharacterSet]] isEqualToString:@""] )
    {
        return NO;
    }

    return YES;
}

@end
