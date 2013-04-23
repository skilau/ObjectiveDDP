#import <UIKit/UIKit.h>
#import <ObjectiveDDP/ObjectiveDDP.h>
#import "AddViewController.h"

@class ObjectiveDDP;

@interface ViewController : UIViewController <ObjectiveDDPDelegate, UITableViewDataSource, AddViewControllerDelegate>
@property (strong, nonatomic) ObjectiveDDP *ddp;
@property (copy, nonatomic) NSString *A_string;
@end
