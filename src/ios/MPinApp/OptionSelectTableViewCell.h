//
//  OptionSelectTableViewCell.h
//  MPinApp
//
//  Created by Tihomir Ganev on 9.март.15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

@interface OptionSelectTableViewCell : UITableViewCell
@property (nonatomic, weak) IBOutlet UILabel* lblName;
@property (nonatomic, weak) IBOutlet UIImageView* imgSelected;

-(void) setServiceSelected:(BOOL) selected;
@end
