//
//  HelpViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 8/31/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HelpViewController : UIViewController

@property( nonatomic,weak ) IBOutlet UILabel *mainTitle;
@property( nonatomic,weak ) IBOutlet UIImageView *art;
@property( nonatomic,weak ) IBOutlet UILabel *subTitle;
@property( nonatomic,weak ) IBOutlet UITextView *desc;


@property( nonatomic,weak ) IBOutlet UIButton *skip;
@property( nonatomic,weak ) IBOutlet UIButton *next;

@property( nonatomic,retain ) NSArray *dataSource;

- ( IBAction )nextItem:( id )sender;
- ( IBAction )back:( id )sender;

@end
