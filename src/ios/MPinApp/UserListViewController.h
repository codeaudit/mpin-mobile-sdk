/*
   Copyright (c) 2012-2015, Certivox
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   For full details regarding our CertiVox terms of service please refer to
   the following links:
 * Our Terms and Conditions -
   http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
   http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
   http://www.certivox.com/about-certivox/patents/
 */

#import "AccessNumberViewController.h"
#import "MPin+AsyncOperations.h"
#import "SuperViewController.h"
#import "NotificationService.h"

@interface UserListTableViewCell : UITableViewCell


@property( nonatomic, strong ) IBOutlet UILabel            *lblUserID;
@property( nonatomic, strong ) IBOutlet UIImageView        *imgViewUser;
@property( nonatomic, strong ) IBOutlet UIImageView        *imgViewSelected;

@end

@interface UserListViewController : SuperViewController <UITableViewDataSource, UITableViewDelegate, AccessNumberDelegate, MPinSDKDelegate, NotificationDelegate>


- ( void ) invalidate;
- ( IBAction )showLeftMenuPressed:( id )sender;
- ( IBAction )btnAuthTap:( id )sender;
- (void) setBackend;
@property( nonatomic, weak ) IBOutlet UIView             *viewButtonsContainer;
@property( nonatomic, weak ) IBOutlet UITableView        *table;
@property( nonatomic, weak ) IBOutlet UIButton           *btnReset;
@property( nonatomic, weak ) IBOutlet UIButton           *btnDelete;
@property( nonatomic, weak ) IBOutlet UIButton           *btnAuthenticate;
@property ( nonatomic, strong ) NSMutableArray *users;

@end
