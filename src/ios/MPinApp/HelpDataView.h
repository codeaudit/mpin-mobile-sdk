//
//  HelpDataView.h
//  MPinApp
//
//  Created by Tihomir Ganev on 9.Sep.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SwipeView.h"

@interface PageControl : UIPageControl
{
    UIImage* activeImage;
    UIImage* inactiveImage;
}
@end

@interface HelpDataView : UIView

@property( nonatomic,strong )  UILabel *lblTitle;
@property( nonatomic,strong )  UILabel *lblSubTitle;
@property( nonatomic,strong )  UIImageView *imgArt;
@property( nonatomic,strong )  UILabel *lblDesc;
@property( nonatomic,strong )  PageControl *pageControl;
@property( nonatomic,strong )  UIButton *btnSkip;
@property( nonatomic,strong )  UIButton *btnNext;


@end


