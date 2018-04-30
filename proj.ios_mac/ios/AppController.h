#import <UIKit/UIKit.h>

//#import <Chartboost/Chartboost.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate>{//, ChartboostDelegate> {
    UIWindow *window;
@public
    RootViewController    *viewController;
}

@end

