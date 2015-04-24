//
//  OptionSelectTableViewCell.m
//  MPinApp
//
//  Created by Tihomir Ganev on 9.март.15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "OptionSelectTableViewCell.h"

@implementation OptionSelectTableViewCell

- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(void) setServiceSelected:(BOOL) selected
{
    if (selected)
    {
        _imgSelected.image = [UIImage imageNamed:@"checked"];
    }
    else
    {
        _imgSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
    }
}

@end
