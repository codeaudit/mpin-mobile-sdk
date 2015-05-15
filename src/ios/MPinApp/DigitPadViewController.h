//
//  DigitPadViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 1/22/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

@interface DigitPadViewController : UIViewController {
    int numberIndex;
    int max;
}


@property(nonatomic, weak) IBOutlet UILabel *label;
@property (nonatomic, retain) IBOutletCollection(UIButton) NSArray *numButtonsCollection;
@property(nonatomic, weak) IBOutlet UIButton *actionButton;
@property(nonatomic, weak) IBOutlet UIButton *clearButton;

@property (nonatomic, strong) NSString * number;

- (void) enableNumButtons;
- (void) disableNumButtons;

- (IBAction)logInAction:(id)sender;
- (IBAction)clearAction:(id)sender;
- (IBAction)numberSelectedAction:(id)sender;

@end
