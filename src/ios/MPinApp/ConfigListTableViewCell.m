//
//  ConfigListTableViewCell.m
//  MPinApp
//
//  Created by Tihomir Ganev on 20.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "ConfigListTableViewCell.h"

@implementation ConfigListTableViewCell

- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(void) setIsSelectedImage: (BOOL)selected
{
    if (selected)
    {
        _imgViewSelected.image = [UIImage imageNamed:@"checked"];
    }
    else
    {
        _imgViewSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
    }
    
}
@end
