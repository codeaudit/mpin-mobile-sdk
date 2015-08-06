//
//  AboutViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 6.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//


@interface AboutViewController : UIViewController

@property ( nonatomic, weak ) IBOutlet UIButton *btnGuide;
@property ( nonatomic, weak ) IBOutlet UIButton *btnSDK_URL;
@property ( nonatomic, weak ) IBOutlet UIButton *btnHomepage;
@property ( nonatomic, weak ) IBOutlet UIButton *btnSupport;
@property ( nonatomic, weak ) IBOutlet UIButton *btnTerms;
@property ( nonatomic, weak ) IBOutlet UIButton *btnValues;

-( IBAction )btnGuideTap:( id )sender;
-( IBAction )btnSKDTap:( id )sender;
-( IBAction )btnHomepageTap:( id )sender;
-( IBAction )btnSupportTap:( id )sender;
-( IBAction )btnTermsTap:( id )sender;
-( IBAction )btnValuesTap:( id )sender;

@property ( nonatomic, weak ) IBOutlet UITextField *backend;
- ( IBAction )sendToken:( id )sender;

@end
