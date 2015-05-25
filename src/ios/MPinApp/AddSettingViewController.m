//
//  AddSettingViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 1/20/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "AddSettingViewController.h"
#import "AppDelegate.h"
#import "ATMHud.h"
#import "TextFieldTableViewCell.h"
#import "OptionSelectTableViewCell.h"
#import "ConfigurationManager.h"
#import "ThemeManager.h"
#import "MpinStatus.h"
#import "MPin.h"
#import "ThemeManager.h"
#import "SettingsManager.h"
#import "NSString+Helper.h"

static NSString* const kErrorTitle = @"Validation ERROR!";

@interface AddSettingViewController () {
    ATMHud* hud;
    MPin* sdk;
    BOOL bTestingConfig;
    NSString *strURL;
}


@property (nonatomic, strong) UITextField* txtMPINServiceNAME;
@property (nonatomic, strong) UITextField* txtMPINServiceURL;
@property (nonatomic, strong) UITextField* txtMPINServiceRPSPrefix;
@property (nonatomic, strong) UIImageView* imgViewLoginToBrowser;
@property (nonatomic, strong) UIImageView* imgViewRequestOTP;
@property (nonatomic) NSInteger intConfigurationType;
@property (nonatomic) enum SERVICES service;

@property (nonatomic, assign) id currentResponder;

- (void)startLoading;
- (void)stopLoading;
- (IBAction)goBack:(id)sender;
- (IBAction)textFieldReturn:(id)sender;

@end

@implementation AddSettingViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    hud = [[ATMHud alloc] initWithDelegate:self];
    _service = LOGIN_ON_MOBILE;
    [[ThemeManager sharedManager] beautifyViewController:self];
    UITapGestureRecognizer* singleTap =
        [[UITapGestureRecognizer alloc] initWithTarget:self
                                                action:@selector(resignOnTap:)];
    [singleTap setNumberOfTapsRequired:1];
    [singleTap setNumberOfTouchesRequired:1];
    singleTap.cancelsTouchesInView = NO;
    [self.view addGestureRecognizer:singleTap];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;

    self.title = NSLocalizedString(@"ADDCONFIGVC_TITLE",@"");
    [_btnTestConfig setTitle:NSLocalizedString(@"ADDCONFIGVC_BTN_TEST_CONFIG", @"") forState:UIControlStateNormal];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    if (_isEdit)
    {
        self.title = NSLocalizedString(@"ADDCONFIGVC_TITLE_EDIT", @"");
        _service = (int)[[ConfigurationManager sharedManager] getConfigurationTypeAtIndex:_selectedIndex];
        strURL = [[ConfigurationManager sharedManager] getURLAtIndex:_selectedIndex];
    }
    else {
        self.title = NSLocalizedString(@"ADDCONFIGVC_TITLE_ADD", @"");
    }
    bTestingConfig = NO;
    _btnDone.title = NSLocalizedString(@"KEY_DONE", @"");
}

#pragma mark - text field delegates -

- (BOOL)textFieldShouldReturn:(UITextField*)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (void)textFieldDidBeginEditing:(UITextField*)textField
{
    self.currentResponder = textField;
}

- (BOOL)textFieldShouldBeginEditing:(UITextField*)textField
{
    return YES;
}

#pragma mark -  Table view delegate -

- (CGFloat)tableView:(UITableView*)tableView
    heightForRowAtIndexPath:(NSIndexPath*)indexPath
{
    return 70;
}

- (NSInteger)tableView:(UITableView*)tableView
    numberOfRowsInSection:(NSInteger)section;
{
    return 6;
}

- (UITableViewCell*)tableView:(UITableView*)tableView
        cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
    switch (indexPath.row) {
    case 0: {
        TextFieldTableViewCell* cell0 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell0"];
        if (cell0 == nil)
            cell0 = [[TextFieldTableViewCell alloc]
                  initWithStyle:UITableViewCellStyleDefault
                reuseIdentifier:@"ConfigCell0"];
        [cell0.txtText setBottomBorder:[[SettingsManager sharedManager] color5]
                                 width:2.f
                                 alpha:.5f];
        _txtMPINServiceNAME = cell0.txtText;
        return cell0;
    }
    case 1: {
        TextFieldTableViewCell* cell0 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell0"];
        if (cell0 == nil)
            cell0 = [[TextFieldTableViewCell alloc]
                  initWithStyle:UITableViewCellStyleDefault
                reuseIdentifier:@"ConfigCell0"];
        [cell0.txtText setBottomBorder:[[SettingsManager sharedManager] color5]
                                 width:2.f
                                 alpha:.5f];
        _txtMPINServiceURL = cell0.txtText;
        return cell0;
    }
    case 2: {
        TextFieldTableViewCell* cell0 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell0"];
        if (cell0 == nil)
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
    case 5: {
        OptionSelectTableViewCell* cell1 =
            [tableView dequeueReusableCellWithIdentifier:@"ConfigCell1"];
        if (cell1 == nil)
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

- (void)tableView:(UITableView*)tableView willDisplayCell:(UITableViewCell*)cell forRowAtIndexPath:(NSIndexPath*)indexPath
{

    switch (indexPath.row) {
    case 0:
        ((TextFieldTableViewCell*)cell).lblName.text = NSLocalizedString(@"ADDCONFIGVC_NAME",@"");
        ((TextFieldTableViewCell*)cell).txtText.placeholder = NSLocalizedString(@"ADDCONFIGVC_NAME",@"");
        if (_isEdit) {
            ((TextFieldTableViewCell*)cell).txtText.text = [[ConfigurationManager sharedManager]
                getNameAtIndex:_selectedIndex];
        }
        break;
    case 1:
        ((TextFieldTableViewCell*)cell).lblName.text = NSLocalizedString(@"ADDCONFIGVC_URL",@"");
        ((TextFieldTableViewCell*)cell).txtText.placeholder = NSLocalizedString(@"ADDCONFIGVC_URL",@"");
        if (_isEdit) {
            ((TextFieldTableViewCell*)cell).txtText.text = strURL;
        }
        break;
    case 2:
        ((TextFieldTableViewCell*)cell).lblName.text = NSLocalizedString(@"ADDCONFIGVC_PREFIX",@"");
        ((TextFieldTableViewCell*)cell).txtText.placeholder = NSLocalizedString(@"ADDCONFIGVC_PREFIX",@"");
        if (_isEdit) {
            ((TextFieldTableViewCell*)cell).txtText.text = [[ConfigurationManager sharedManager]
                getPrefixAtIndex:_selectedIndex];
        }
        break;
    case 3:
        ((OptionSelectTableViewCell*)cell).lblName.text = NSLocalizedString(@"LOGIN_MOBILE_APP", @"");
        switch (_service) {

        case LOGIN_ON_MOBILE:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:YES];
            break;

        case LOGIN_ONLINE:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:NO];
            break;

        case LOGIN_WITH_OTP:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:NO];
            break;
        }

        break;
    case 4:
        ((OptionSelectTableViewCell*)cell).lblName.text = NSLocalizedString(@"LOGIN_ONLINE_SESSION", @"");
        switch (_service) {
        case LOGIN_ON_MOBILE:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:NO];
            break;

        case LOGIN_ONLINE:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:YES];
            break;

        case LOGIN_WITH_OTP:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:NO];
            break;
        }

        break;
    case 5:
        ((OptionSelectTableViewCell*)cell).lblName.text = NSLocalizedString(@"LOGIN_OTP", @"");
        switch (_service) {
        case LOGIN_ON_MOBILE:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:NO];
            break;

        case LOGIN_ONLINE:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:NO];
            break;

        case LOGIN_WITH_OTP:
            [((OptionSelectTableViewCell*)cell)setServiceSelected:YES];
            break;
        }

        break;
    default:
        break;
    }
}

- (void)tableView:(UITableView*)tableView
    didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    switch (indexPath.row) {
    case 3:
        _service = LOGIN_ON_MOBILE;
        break;
    case 4:
        _service = LOGIN_ONLINE;
        break;
    case 5:
        _service = LOGIN_WITH_OTP;
        break;
    }
    [_tblView reloadData];
}

#pragma mark - my methods -
- (void)resignOnTap:(id)sender
{
    [self.currentResponder resignFirstResponder];
}

- (void)startLoading
{

    [hud setCaption:@"Loading please wait!"];
    [hud setActivity:YES];
    [hud showInView:self.view];
}
- (void)stopLoading
{
    [hud hide];
}

- (IBAction)textFieldReturn:(id)sender
{
    [sender resignFirstResponder];
}

- (IBAction)onSave:(id)sender
{
    bTestingConfig = NO;
    if ([_txtMPINServiceURL.text isEqualToString:@""]) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:kErrorTitle
                                                        message:NSLocalizedString(@"ADDCONFIGVC_ERROR_EMPTY_URL", @"")
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"")
                                              otherButtonTitles:nil, nil];
        [alert show];
        return;
    }

    if (![self isValidURL:_txtMPINServiceURL.text]) {
        UIAlertView* alert =
            [[UIAlertView alloc] initWithTitle:kErrorTitle
                                       message:NSLocalizedString(@"ADDCONFIGVC_ERROR_INVALID_URL", @"")
                                      delegate:nil
                             cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"")
                             otherButtonTitles:nil, nil];

        [alert show];
        return;
    }
    int minShowTime = 1;
    NSString* caption = @"";
    if (_isEdit) {
        if ([_txtMPINServiceURL.text isEqualToString:[[ConfigurationManager sharedManager] getURLAtIndex:_selectedIndex]]) {
            caption = NSLocalizedString(@"HUD_SAVE_CONFIG", @"");
        }
        else {
            caption = NSLocalizedString(@"HUD_SAVE_CONFIG_AND_DEL", @"");
            minShowTime = 3;
        }
    }
    else {
        caption = NSLocalizedString(@"HUD_SAVE_CONFIG", @"");
    }
    hud.minShowTime = minShowTime;
    [hud setCaption:caption];
    [hud setActivity:YES];
    [hud showInView:self.view];

    ///TODO :: add rpsPrefix test field to this VIEW it is needed for some configurations
    [sdk TestBackend:_txtMPINServiceURL.text rpsPrefix:nil];
}

- (void)OnTestBackendCompleted:(id)sender
{
    [self stopLoading];
    if (bTestingConfig)
    {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:nil
                                                        message:NSLocalizedString(@"ADDCONFIGVC_MESSAGE_CONFIG_OK", @"")
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"")
                                              otherButtonTitles:nil, nil];
        [alert show];
        
        bTestingConfig = NO;

    }
    else
    {
        int minShowTime = 1;
        if (_selectedIndex >= 0)
        {
            if (_isEdit)
            {
                [[ConfigurationManager sharedManager] saveConfigurationAtIndex:_selectedIndex
                                                                           url:_txtMPINServiceURL.text
                                                                   serviceType:(int)_service
                                                                          name:_txtMPINServiceNAME.text];
            }
            else
            {
                [[ConfigurationManager sharedManager] addConfigurationWithURL:_txtMPINServiceURL.text
                                                                  serviceType:(int)_service
                                                                         name:_txtMPINServiceNAME.text];
            }
        }
        else
            NSLog(@"WARNING: confSettings Array is not set. The application  will not funciton properly");
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(minShowTime * NSEC_PER_SEC)),
                       dispatch_get_main_queue(), ^{
                           [self.navigationController popViewControllerAnimated:YES];
                           [self stopLoading];
                       });
    }
    
}

- (void)OnTestBackendError:(id)sender error:(NSError*)error
{
    [self stopLoading];
    MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
    NSString *message = NSLocalizedString(@"ADDCONFIGVC_INVALID_CONFIG", @"");

    UIAlertView* alert = [[UIAlertView alloc]
            initWithTitle:[mpinStatus getStatusCodeAsString]
                  message:message
                 delegate:nil
        cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"")
        otherButtonTitles:nil, nil];
    [alert show];
}

- (BOOL)isValidURL:(NSString*)strTestURL
{
    if ([NSString isBlank:strTestURL]) {
        return NO;
    }

    NSString* regExPattern = @"(http|https)://((\\w)*|([0-9]*)|([-|_])*)+([\\.|/"@"]((\\w)*|([0-9]*)|([-|_])*))+";

    NSRegularExpression* regEx = [[NSRegularExpression alloc] initWithPattern:regExPattern
                                                                      options:NSRegularExpressionCaseInsensitive
                                                                        error:nil];
    NSUInteger regExMatches = [regEx numberOfMatchesInString:strTestURL
                                                     options:0
                                                       range:NSMakeRange(0, [strTestURL length])];

    if (regExMatches == 0) {
        return NO;
    }
    else {
        return YES;
    }
}

- (IBAction)goBack:(id)sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnTestConfigTap:(id)sender
{
    bTestingConfig = YES;
    if ([self isValidURL:_txtMPINServiceURL.text])
    {
        [self startLoading];
        if ([_txtMPINServiceRPSPrefix.text isEqualToString:@""])
        {
            [sdk TestBackend:_txtMPINServiceURL.text rpsPrefix:nil];
        }
        else
        {
            [sdk TestBackend:_txtMPINServiceURL.text rpsPrefix:_txtMPINServiceRPSPrefix.text];
        }
    }
    else
    {
        [self stopLoading];
        UIAlertView* alert =
        [[UIAlertView alloc] initWithTitle:kErrorTitle
                                   message:NSLocalizedString(@"ADDCONFIGVC_ERROR_INVALID_URL", @"")
                                  delegate:nil
                         cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"")
                         otherButtonTitles:nil, nil];
        
        [alert show];
    }
}

@end
