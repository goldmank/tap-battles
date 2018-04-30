#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "SystemUtilsIOS.h"
#import "IAPHelper.h"
#import "MuGames.h"

#import "KeychainItemWrapper.h"

#import <Security/Security.h>

//#import <FacebookSDK/FacebookSDK.h>

//#import "Flurry.h"

//#import "AppsFlyerTracker.h"

#include "GameScene.h"
#include "ProductInfo.h"

//#import <Parse/Parse.h>
//
//#import <Chartboost/CBNewsfeed.h>
//#import <CommonCrypto/CommonDigest.h>
//#import <AdSupport/AdSupport.h>
//
//#import <Tapjoy/Tapjoy.h>

#import "SimpleAudioEngine.h"

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static SystemUtilsIOS * pSystemUtils = new SystemUtilsIOS();
static AppDelegate s_sharedApplication(pSystemUtils);

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    //[AppsFlyerTracker sharedTracker].appsFlyerDevKey = @"iq6kHqUBhf6yUarqomRZyk";
    //[AppsFlyerTracker sharedTracker].appleAppID = @"969915327";
    
    //[Flurry startSession:@"D65NCKMYSQ9CRQ8K4WMN"];
    
    // Initialize the Chartboost library
    //[Chartboost startWithAppId:@"55866d510d60252e3b1785cf"
    //            appSignature:@"5073b074a5d05a01eb39333aa18aa2c1591d0510"
    //            delegate:self];
    
    //[Parse enableLocalDatastore];
    //[Parse setApplicationId:@"zKQdVfiif6MMMzERhcPmFnLueQQvQaPilgEosItQ" clientKey:@"G6vRdtW5aUjbFncFcs1isPUxSlhphAxpNV7s7GaA"];
    //[PFAnalytics trackAppOpenedWithLaunchOptions:launchOptions];
    
    // Tapjoy Connect Notifications
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                             selector:@selector(tjcConnectSuccess:)
//                                                 name:TJC_CONNECT_SUCCESS
//                                               object:nil];
//    [[NSNotificationCenter defaultCenter] addObserver:self
//                                             selector:@selector(tjcConnectFail:)
//                                                 name:TJC_CONNECT_FAILED
//                                               object:nil];
//    [Tapjoy requestTapjoyConnect:@"80fb0fc0-fb5d-4a94-a6fc-9724a8ea9ceb" secretKey:@"awG2g6sEqrYkFaRLQSPA" options:@{ TJC_OPTION_ENABLE_LOGGING : @(YES) } ];
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Override point for customization after application launch.
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Use RootViewController manage CCEAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = eaglView;
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    [self getProductsInfo];
    
    [self initMuGames];
    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    [self addSplash];
    
    cocos2d::Application::getInstance()->run();
        
    return YES;
}

- (void)addSplash
{
    UIImage* image = [UIImage imageNamed:[self getLaunchImageName]];
    if ([[UIScreen mainScreen] respondsToSelector: @selector(scale)])
    {
        float screenScale = [[UIScreen mainScreen] scale];
        if (screenScale > 1.)
            image = [UIImage imageWithCGImage:image.CGImage scale:screenScale orientation:image.imageOrientation];
    }
    UIImageView *splashView = [[UIImageView alloc] initWithImage:image];
    [viewController.view addSubview:splashView];
    [splashView performSelector:@selector(removeFromSuperview) withObject:nil afterDelay:0.5f];
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    
    //[[AppsFlyerTracker sharedTracker] trackAppLaunch];
    
    //[FBAppEvents activateApp];

    cocos2d::CCDirector::sharedDirector()->resume();
    
    // Handle the user leaving the app while the Facebook login dialog is being shown
    // For example: when the user presses the iOS "home" button while the login dialog is active
    //[FBAppCall handleDidBecomeActive];
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

- (void)initMuGames
{
//    NSString * uid = [self getUniqueDeviceIdentifierAsString];
//    if (nil == uid)
//    {
//        uid = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
//        Pref::getInstance->setUserId([uid UTF8String]);
//    }
//    const char * deviceId = Pref::getInstance()->getUserId();
//    if ('\0' == deviceId[0])
//    {
//        //TODO: need to use different method - it's not 100% unique and can be not deleted after uninstall
//        //Deleting an app from the device Resets identifierForVendor, if this was the last app with a specific Team ID.
//        //deviceId = [[[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString] UTF8String];
//        
//        NSString *uuid = [[NSUUID UUID] UUIDString];
//        deviceId = [uuid UTF8String];
//        Pref::getInstance()->setUserId(deviceId);
//        Pref::getInstance()->save();
//    }
//    CCLOG("deviceID: %s", deviceId);
//    const char * appVersion = [[NSString stringWithFormat:@"%@", [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"]] UTF8String];
//    const char * sysVersion = [[[UIDevice currentDevice] systemVersion] UTF8String];
//    char os[50];
//    sprintf(os, "iOS %s", sysVersion);
//    const char * deviceType = [[UIDevice currentDevice].model UTF8String];
//    MuGames::getInstance()->init(appVersion, deviceId, os, deviceType);
}

-(NSString *)getUniqueDeviceIdentifierAsString
{
    NSString * retVal = nil;
    
//    KeychainItemWrapper *keychain = [[KeychainItemWrapper alloc] initWithIdentifier:@"DeviceID" accessGroup:nil];
//    NSString * deviceID = [keychain objectForKey:(id)kSecValueData];
//    if(deviceID == nil || deviceID.length == 0)
//    {
//        CFUUIDRef uuidRef = CFUUIDCreate(kCFAllocatorDefault);
//        CFStringRef uuidStringRef = CFUUIDCreateString(kCFAllocatorDefault, uuidRef);
//        CFRelease(uuidRef);
//        [keychain setObject:(__bridge NSString *)uuidStringRef forKey:(id)kSecValueData];
//        //NSLog(@"new device id = %@", [keychain objectForKey:(id)kSecValueData]);
//        retVal = (__bridge NSString *)uuidStringRef;
//        CFRelease(uuidStringRef);
//    }
//    else
//    {
//        //NSLog(@"device id = %@", deviceID);
//        retVal = deviceID;
//    }
    
    return retVal;
}

- (void)getProductsInfo {
    
    [[IAPHelper sharedInstance] requestProductsWithCompletionHandler:^(BOOL success, NSArray *products) {
        if (success)
        {
            std::vector<ProductInfo> productsArray;
            for (id prod in products)
            {
                SKProduct * product = (SKProduct *)prod;
                
                [product retain]; // so we'll have the reference in ProductInfo.data to use in other places inside the app
                
                NSNumberFormatter *priceFormatter = [[NSNumberFormatter alloc] init];
                [priceFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
                [priceFormatter setLocale:product.priceLocale];
                
                NSString * priceLocal = [NSString stringWithFormat:@"%@ %@", product.price, priceFormatter.internationalCurrencySymbol];
                
                NSLog(@"product name = %s price = %@", [product.localizedDescription UTF8String], priceLocal);
                ProductInfo info([product.productIdentifier UTF8String],
                                 [product.localizedDescription UTF8String],
                                 [priceLocal UTF8String],
                                 [[product.price stringValue] UTF8String],
                                 [priceFormatter.internationalCurrencySymbol UTF8String],
                                 (void *)product
                                 );
                productsArray.push_back(info);
            }
            pSystemUtils->initProducts(productsArray);
        }
        else
        {
            NSLog(@"failed load products");
        }
    }];
}

//- (void)didCompleteRewardedVideo:(CBLocation)location withReward:(int)reward
//{
//    cocos2d::CCScene * pCurrScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
//    if (NULL != pCurrScene)
//    {
//        BaseScene * pScene = (BaseScene *)pCurrScene->getChildren().at(0);
//        GameScene * pGameScene = dynamic_cast<GameScene*>(pScene);
//        if (NULL != pGameScene)
//        {
//            pGameScene->didCompleteRewardedVideo(reward);
//        }
//    }
//}
//
//- (void)willDisplayVideo:(CBLocation)location
//{
//    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
//}
//
//- (void)didCloseRewardedVideo:(CBLocation)location
//{
//    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
//}
//
//-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)srcApp annotation:(id)annotation
//{
//    return [FBAppCall handleOpenURL:url sourceApplication:srcApp];
//}
//
//-(void)tjcConnectSuccess:(NSNotification*)notifyObj
//{
//    NSLog(@"Tapjoy connect Succeeded");
//}
//
//
//- (void)tjcConnectFail:(NSNotification*)notifyObj
//{
//    NSLog(@"Tapjoy connect Failed");
//}

-(NSString*)getLaunchImageName
{
    NSArray* images= @[@"LaunchImage.png",
                       @"LaunchImage@2x.png",
                       @"LaunchImage-700@2x.png",
                       @"LaunchImage-568h@2x.png",
                       @"LaunchImage-700-568h@2x.png",
                       @"LaunchImage-700-Portrait@2x~ipad.png",
                       @"LaunchImage-Portrait@2x~ipad.png",
                       @"LaunchImage-700-Portrait~ipad.png",
                       @"LaunchImage-Portrait~ipad.png",
                       @"LaunchImage-Landscape@2x~ipad.png",
                       @"LaunchImage-700-Landscape@2x~ipad.png",
                       @"LaunchImage-Landscape~ipad.png",
                       @"LaunchImage-700-Landscape~ipad.png",
                       @"LaunchImage-800-667h@2x.png",
                       @"LaunchImage-800-Portrait-736h@3x.png",
                       @"LaunchImage-800-Landscape-736h@3x.png",
                       ];
    
    UIImage *splashImage;
    
    if ([self isDeviceiPhone])
    {
        if ([self isDeviceiPhone4] && [self isDeviceRetina])
        {
            splashImage = [UIImage imageNamed:images[1]];
            if (splashImage.size.width!=0)
                return images[1];
            else
                return images[2];
        }
        else if ([self isDeviceiPhone5])
        {
            splashImage = [UIImage imageNamed:images[1]];
            if (splashImage.size.width!=0)
                return images[3];
            else
                return images[4];
        }
        else if ([self isDeviceiPhone6])
        {
            splashImage = [UIImage imageNamed:images[1]];
            if (splashImage.size.width!=0)
                return images[13];
            else
                return images[14];
        }
        else
            return images[0]; //Non-retina iPhone
    }
    else if ([[UIDevice currentDevice] orientation]==UIDeviceOrientationPortrait || [[UIDevice currentDevice] orientation] == UIDeviceOrientationPortraitUpsideDown)//iPad Portrait
    {
        if ([self isDeviceRetina])
        {
            splashImage = [UIImage imageNamed:images[5]];
            if (splashImage.size.width!=0)
                return images[5];
            else
                return images[6];
        }
        else
        {
            splashImage = [UIImage imageNamed:images[7]];
            if (splashImage.size.width!=0)
                return images[7];
            else
                return images[8];
        }
        
    }
    else
    {
        if ([self isDeviceRetina])
        {
            splashImage = [UIImage imageNamed:images[9]];
            if (splashImage.size.width!=0)
                return images[9];
            else
                return images[10];
        }
        else
        {
            splashImage = [UIImage imageNamed:images[11]];
            if (splashImage.size.width!=0)
                return images[11];
            else
                return images[12];
        }
    }
}

-(BOOL)isDeviceiPhone
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    {
        return TRUE;
    }
    
    return FALSE;
}

-(BOOL)isDeviceiPhone4
{
    if ([[UIScreen mainScreen] bounds].size.height==480)
        return TRUE;
    
    return FALSE;
}


-(BOOL)isDeviceRetina
{
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] &&
        ([UIScreen mainScreen].scale == 2.0))        // Retina display
    {
        return TRUE;
    }
    else                                          // non-Retina display
    {
        return FALSE;
    }
}


-(BOOL)isDeviceiPhone5
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone && [[UIScreen mainScreen] bounds].size.height==568)
    {
        return TRUE;
    }
    return FALSE;
}

-(BOOL)isDeviceiPhone6
{
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone && [[UIScreen mainScreen] bounds].size.height>568)
    {
        return TRUE;
    }
    return FALSE;
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end
