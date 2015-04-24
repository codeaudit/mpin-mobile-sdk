//
//  MenuListTableViewCell.h
//  MPinApp
//
//  Created by Tihomir Ganev on 27.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MenuTableViewCell : UITableViewCell

@property(nonatomic, retain, readwrite) IBOutlet UILabel            *lblMenuID;
@property(nonatomic, retain, readwrite) IBOutlet UIView             *viewSeparator;

@end

