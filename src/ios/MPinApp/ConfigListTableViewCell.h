//
//  ConfigListTableViewCell.h
//  MPinApp
//
//  Created by Tihomir Ganev on 20.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

@interface ConfigListTableViewCell : UITableViewCell

@property(nonatomic,weak) IBOutlet UILabel      *lblConfigurationName;
@property(nonatomic,weak) IBOutlet UILabel      *lblConfigurationType;
@property(nonatomic,weak) IBOutlet UIImageView  *imgViewSelected;

-(void) setIsSelectedImage: (BOOL)selected;
@end
