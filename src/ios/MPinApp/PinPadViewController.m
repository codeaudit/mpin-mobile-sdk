//
//  ViewController.m
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/14/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "PinPadViewController.h"
#import "Constants.h"
#import "MPin.h"
#import "BackButton.h"
#import "UIView+Helper.m"
#import "MFSideMenu.h"
#import "ThemeManager.h"

#define PIN_LENGTH 4

static NSMutableArray* kCircles;

@interface PinPadViewController ()

@property (nonatomic, weak) IBOutlet UIImageView* imgViewDigit0;
@property (nonatomic, weak) IBOutlet UIImageView* imgViewDigit1;
@property (nonatomic, weak) IBOutlet UIImageView* imgViewDigit2;
@property (nonatomic, weak) IBOutlet UIImageView* imgViewDigit3;

- (void)renderNumberTextField:(NSInteger)numberLenght;
- (IBAction)back:(UIBarButtonItem*)sender;

@property (nonatomic, weak) IBOutlet BackButton* backButton;

@end

@implementation PinPadViewController

- (void)renderNumberTextField:(NSInteger)numberLenght
{
    kCircles = [NSMutableArray arrayWithCapacity:PIN_LENGTH + 1];
    NSString* pinField = @"";
    for (int i = 0; i < PIN_LENGTH; i++)
        pinField = [pinField stringByAppendingString:@"⚪"];
    [kCircles addObject:pinField];
    for (int i = 0; i < PIN_LENGTH; i++) {
        pinField = [pinField stringByReplacingCharactersInRange:NSMakeRange(i, 1) withString:@"⚫"];
        [kCircles addObject:pinField];
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    max = PIN_LENGTH;
    [self renderNumberTextField:PIN_LENGTH];
    self.label.text = kCircles[0];
    [self.pinView setBottomBorder:[[SettingsManager sharedManager] color7] width:2.f alpha:.5f];
    [[ThemeManager sharedManager] beautifyViewController:self];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.number = @"";
    _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];

    [self.menuContainerViewController setPanMode:MFSideMenuPanModeNone];

    BackButton* btnBack = [[BackButton alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:self action:@selector(back:)];
    [btnBack setup];

    if (!_boolShouldShowBackButton) {
        self.navigationItem.hidesBackButton = YES;
        self.navigationItem.leftBarButtonItem = nil;
    }
    else {
        self.navigationItem.hidesBackButton = NO;
        self.navigationItem.leftBarButtonItem = btnBack;
    }

    self.lblEmail.text = self.userId;
}

- (IBAction)back:(UIBarButtonItem*)sender
{

    [MPin sendPin:kEmptyStr];
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}
- (IBAction)logInAction:(id)sender
{
    [MPin sendPin:self.number];
    [self.navigationController popViewControllerAnimated:NO];
}

- (IBAction)clearAction:(id)sender
{
    [super clearAction:sender];
    _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];

    self.label.text = kCircles[numberIndex];
}

- (IBAction)numberSelectedAction:(id)sender
{
    NSLog(@"Number: %@", self.number);
    [super numberSelectedAction:sender];

    switch ([self.number length]) {
    case 0:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;
    case 1:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;
    case 2:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;
    case 3:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;
    case 4:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-full"];
        break;
    default:
        break;
    }
}

@end
