#include "SystemUtilsIOS.h"
#include "cocos2d.h"
#include "Pref.h"
#import "AppController.h"
#import "GameCenterIos.h"
#import "IAPHelper.h"
//#import "FacebookAppActivity.h"
//#import "JBWhatsAppActivity.h"
//#import "Flurry.h"
//#import "JBWhatsAppActivity.h"
#include "GameRecording.h"
#include "GameScene.h"

//#import "AppsFlyerTracker.h"
//#import <Parse/Parse.h>
//#import <Chartboost/Chartboost.h>
//#import <FacebookSDK/FacebookSDK.h>
//#import <AdSupport/ASIdentifierManager.h>
//
//#import <Tapjoy/Tapjoy.h>

// ===========================================================
// Constants
// ===========================================================

#define APP_STORE_ID 969915327

static NSString *const iOS7AppStoreURLFormat = @"itms-apps://itunes.apple.com/app/id%d";
static NSString *const iOSAppStoreURLFormat = @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%d";

// ===========================================================
// Initialization & cleanup
// ===========================================================

SystemUtilsIOS::SystemUtilsIOS()
{
    m_bShowingLoading = false;
    m_pLoadingView = NULL;
    m_pLoading = NULL;
    m_bLoginToInvite = false;
    m_iScoreForPostLoginInvite = 0;
    restoreFbUserList();
}

SystemUtilsIOS::~SystemUtilsIOS()
{
    
}

// ===========================================================
// Public methods
// ===========================================================

void SystemUtilsIOS::initGameCenter()
{
    NSUserDefaults* preferences = [NSUserDefaults standardUserDefaults];
    NSString* isLoggedIn = @"isLoggedIn";
    if ([preferences objectForKey:isLoggedIn] != nil)
    {
        const NSInteger currentLevel = [preferences integerForKey:isLoggedIn];
        if (1 == currentLevel)
        {
            [[GameCenterIos shared] login];
        }
    }
}

void SystemUtilsIOS::showAchivments()
{
    CCLOG("showAchivments");
    if ([[GameCenterIos shared] isLoggedIn])
    {
        [[GameCenterIos shared] showAchievements];
    }
    else
    {
        [[GameCenterIos shared] loginAchivments];
    }
}

void SystemUtilsIOS::showLeaderboard()
{
    CCLOG("showLeaderboard");
    if ([[GameCenterIos shared] isLoggedIn])
    {
        [[GameCenterIos shared] showScores];
    }
    else
    {
        [[GameCenterIos shared] loginLeaderboard];
    }
}

void SystemUtilsIOS::sendScore(int score)
{
    CCLOG("sendScore");
    if ([[GameCenterIos shared] isLoggedIn])
    {
        [[GameCenterIos shared] postScore:"CgkIr9fn2_AMEAIQCA" score:[NSNumber numberWithInt:score]];
    }
}

void SystemUtilsIOS::sendAchivment(const char * name)
{
    CCLOG("sendAchivment");
    if ([[GameCenterIos shared] isLoggedIn])
    {
        [[GameCenterIos shared] postAchievement:name percent:[NSNumber numberWithInt:100]];
    }
}

void SystemUtilsIOS::shareScore(int score)
{
//    NSMutableArray *sharingItems = [NSMutableArray new];
//    
//    NSString *inviteText = [NSString stringWithFormat:@"OMG! I collected %d coins in Dodgy Wings! #dodgywings http://www.flambostudios.com/dodgywings", score];
//    
//    [sharingItems addObject:inviteText];
//    
//    WhatsAppMessage *whatsappMsg = [[WhatsAppMessage alloc] initWithMessage:inviteText forABID:@""];
//    FacebookMessage *facebookMsg = [[FacebookMessage alloc] initWithMessage:inviteText forLink:@"http://www.flambostudios.com/dodgywings"];
//    
//    NSArray *applicationActivities = @[[[JBWhatsAppActivity alloc] init], [[FacebookAppActivity alloc] init]];
//    NSArray *excludedActivities    = @[UIActivityTypePostToWeibo, UIActivityTypePostToFacebook];
//    //NSArray *activityItems         = @[_messageTextField.text, whatsappMsg];
//    [sharingItems addObject:whatsappMsg];
//    [sharingItems addObject:facebookMsg];
//    
//    UIActivityViewController *activityController = [[UIActivityViewController alloc] initWithActivityItems:sharingItems applicationActivities:applicationActivities];
//    [activityController setValue:@"Dodgy Wings" forKey:@"subject"];
//    activityController.excludedActivityTypes = excludedActivities;
//    
//    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
//    [app->viewController presentViewController:activityController animated:YES completion:nil];
}

void SystemUtilsIOS::invite(int score)
{
//    if (nil != FBSession.activeSession && FBSession.activeSession.state == FBSessionStateOpen)
//    {
//        NSString * msg = [NSString stringWithFormat:@"Hey, I just scored %d in Dodgy Wings! Can you beat it?!", score];
//        if (0 == score)
//        {
//            msg = [NSString stringWithFormat:@"Let's see if you can beat me in Dodgy Wings!"];
//        }
//        
//        [FBWebDialogs
//         presentRequestsDialogModallyWithSession:FBSession.activeSession
//         message:msg
//         title:@"Challange"
//         parameters:nil
//         handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
//             if (error) {
//                 // Error launching the dialog or sending the request.
//                 NSLog(@"Error sending request.");
//             } else {
//                 if (result == FBWebDialogResultDialogNotCompleted) {
//                     // User clicked the "x" icon
//                     NSLog(@"User canceled request.");
//                 } else {
//                     // Handle the send request callback
//                     
//                     NSArray *pairs = [[resultURL query] componentsSeparatedByString:@"&"];
//                     NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
//                     for (NSString *pair in pairs) {
//                         NSArray *kv = [pair componentsSeparatedByString:@"="];
//                         NSString *val =
//                         [[kv objectAtIndex:1]
//                          stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
//                         
//                         [params setObject:val forKey:[kv objectAtIndex:0]];
//                     }
//                     
//                     if (![params valueForKey:@"request"]) {
//                         // User clicked the Cancel button
//                         NSLog(@"User canceled request.");
//                     } else {
//                         // User clicked the Send button
//                         std::vector<std::string> invitedIds;
//                         NSString *requestID = [params valueForKey:@"request"];
//                         for(id key in params)
//                         {
//                             id value = [params objectForKey:key];
//                             if (![key isEqualToString:@"request"])
//                             {
//                                 invitedIds.push_back([value UTF8String]);
//                             }
//                         }
//                         NSLog(@"Request ID: %@", requestID);
//                         
//                         cocos2d::CCScene * pCurrScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
//                         if (NULL != pCurrScene)
//                         {
//                             for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
//                             {
//                                 GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
//                                 if (NULL != pGameScene)
//                                 {
//                                     pGameScene->onInviteDone(invitedIds);
//                                     break;
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }];
//    }
//    else
//    {
//        loginToFacebook();
//        m_bLoginToInvite = true;
//        m_iScoreForPostLoginInvite = score;
//    }
}

void SystemUtilsIOS::purchase(int itemId)
{
    ProductInfo * pInfo = getProductByIndex(itemId);
    if (NULL != pInfo)
    {
        SKProduct *product = (SKProduct *)getProductByIndex(itemId)->getData();
        if (NULL != product)
        {
            NSLog(@"SystemUtilsIOS::Buying %@...", product.productIdentifier);
            [[IAPHelper sharedInstance] buyProduct:product];
        }
    }
}

void SystemUtilsIOS::rateUs()
{
    NSURL * url = [NSURL URLWithString:[NSString stringWithFormat:([[UIDevice currentDevice].systemVersion floatValue] >= 7.0f)? iOS7AppStoreURLFormat: iOSAppStoreURLFormat, APP_STORE_ID]]; // Would contain the right link
    [[UIApplication sharedApplication] openURL:url];
}

void SystemUtilsIOS::trackPage(const char * page)
{
    //[Flurry logEvent:[NSString stringWithUTF8String:page]];
}

void SystemUtilsIOS::trackEvent(const char * event, const char * paramName, const char * paramValue)
{
//    NSDictionary *params = [NSDictionary dictionaryWithObjectsAndKeys:
//                            [NSString stringWithUTF8String:paramValue], [NSString stringWithUTF8String:paramName],
//                            nil];
//    
//    [Flurry logEvent:[NSString stringWithUTF8String:event] withParameters:params];
}

void SystemUtilsIOS::trackTimedEvent(const char * event)
{
    //[Flurry endTimedEvent:[NSString stringWithUTF8String:event] withParameters:nil];
}

bool SystemUtilsIOS::isVideoAdReady()
{
    return false;//[Chartboost hasRewardedVideo:CBLocationLevelComplete];
}

void SystemUtilsIOS::prepareVideoAd()
{
    //[Chartboost cacheRewardedVideo:CBLocationLevelComplete];
}

void SystemUtilsIOS::showVideoAd()
{
    //[Chartboost showRewardedVideo:CBLocationLevelComplete];
}

bool SystemUtilsIOS::isInterstitialAdReady()
{
    return false;//[Chartboost hasInterstitial:CBLocationGameOver];
}

void SystemUtilsIOS::prepareInterstitialAd()
{
    //[Chartboost cacheInterstitial:CBLocationGameOver];
}

void SystemUtilsIOS::showInterstitialAd()
{
    //[Chartboost showInterstitial:CBLocationGameOver];
}

void SystemUtilsIOS::showLoadingScreen()
{
    if (m_bShowingLoading)
    {
        return;
    }
    
    m_bShowingLoading = true;
    
    cocos2d::CCDirector* pDirector = cocos2d::CCDirector::sharedDirector();
    cocos2d::CCEGLView* pEGLView = pDirector->getOpenGLView();
    cocos2d::CCSize frameSize = pEGLView->getFrameSize();
    
    float w = frameSize.width;
    float h = frameSize.height;
    //if (750 == w)
    {
        w /= 2;
        h /= 2;
    }
    CCLOG("loading: %.2f, %.2f", w, h);
    UIView * loadingView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, w, h)];
    loadingView.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0.5];
    UIActivityIndicatorView * loading = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
    [loading setHidesWhenStopped:YES];
    [loadingView addSubview:loading];
    loading.frame = CGRectMake(w/2 - 37, h/2 - 37, 74, 74);
        
    UIApplication* clientApp = [UIApplication sharedApplication];
    UIWindow* topWindow = [clientApp keyWindow];
    if (!topWindow)
    {
        topWindow = [[clientApp windows] objectAtIndex:0];
    }
    [topWindow addSubview:loadingView];
    
    [loading setHidden:NO];
    [loading startAnimating];
    
    m_pLoadingView = (void *)loadingView;
    m_pLoading = (void *)loading;
}

void SystemUtilsIOS::hideLoadingScreen()
{
    if (!m_bShowingLoading)
    {
        return;
    }
    
    UIView * loadingView = (UIView *)m_pLoadingView;
    UIActivityIndicatorView * loading = (UIActivityIndicatorView *)m_pLoading;
    [loading stopAnimating];
    [loadingView removeFromSuperview];
    
    m_bShowingLoading = false;
}

void SystemUtilsIOS::multiplayerLogin()
{
//    CCLOG("multiplayerLogin");
//    
//    PFUser *currentUser = [PFUser currentUser];
//    if (!currentUser)
//    {
//        NSString * uid = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
//        
//        PFUser *user = [PFUser user];
//        user.username = uid;
//        user.password = @"my pass";
//        user.email = [NSString stringWithFormat:@"%@@temp.com", uid];
//        
//        [user signUpInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
//            if (!error) {
//                // Hooray! Let them use the app now.
//            } else {
//                //NSString *errorString = [error userInfo][@"error"];
//                
//                [PFUser logInWithUsernameInBackground:user.username password:user.password
//                    block:^(PFUser *user, NSError *error) {
//                        if (user) {
//                            // Do stuff after successful login.
//                            
//                            const char * fbId = Pref::getInstance()->getFbId();
//                            if ('\0' == fbId[0])
//                            {
//                                NSString * fbId = user[@"fbId"];
//                                NSString * name = user[@"name"];
//                                if (nil != fbId && nil != name)
//                                {
//                                    Pref::getInstance()->setUserDetails([fbId UTF8String], [name UTF8String]);
//                                    Pref::getInstance()->setMultiplayerWins([user[@"winCount"] intValue]);
//                                    Pref::getInstance()->save();
//                                }
//                            }
//                        } else {
//                            // The login failed. Check error to see why.
//                        }
//                }];
//            }
//        }];
//    }
//    else // update current user
//    {
//        currentUser[@"winCount"] = [NSNumber numberWithInt:Pref::getInstance()->getMultiplayerWins()];
//        [currentUser incrementKey:@"sessionCount"];
//        currentUser[@"coins"] = [NSNumber numberWithInt:Pref::getInstance()->getCoinsCount()];
//        currentUser[@"bestScore"] = [NSNumber numberWithInt:Pref::getInstance()->getBestScore()];
//        currentUser[@"selectedBird"] = [NSNumber numberWithInt:Pref::getInstance()->getSelectedBird()];
//        currentUser[@"daysSinceInstall"] = [NSNumber numberWithInt:Pref::getInstance()->getDaysSinceInstall()];
//        currentUser[@"videoCount"] = [NSNumber numberWithInt:Pref::getInstance()->getSeenVideoCount()];
//        currentUser[@"didShare"] = [NSNumber numberWithInt:Pref::getInstance()->isMadeAShare() ? 1 : 0];
//        currentUser[@"purchased"] = [NSNumber numberWithInt:Pref::getInstance()->getPurchasedCoins()];
//        
//        char unlockedBirds[200];
//        memset(unlockedBirds, 0, sizeof(unlockedBirds));
//        for (int i = 0; i < 100; i++)
//        {
//            if (Pref::getInstance()->isBirdUnlocked(i))
//            {
//                unlockedBirds[i] = '1';
//            }
//            else
//            {
//                unlockedBirds[i] = '0';
//            }
//        }
//        currentUser[@"unlockedBirds"] = [NSString stringWithUTF8String:unlockedBirds];
//        
//        [currentUser saveInBackground];
//        
//        if ([[currentUser objectForKey:@"sessionCount"] intValue] % 5 == 0)
//        {
//            updateFbUserList();
//        }
//    }
}

void SystemUtilsIOS::sendGameRecording(GameRecording * pGameRecording, std::vector<std::string> invitedIds)
{
//    PFUser *currentUser = [PFUser currentUser];
//    if (!currentUser)
//    {
//        return;
//    }
//    
//    showLoadingScreen();
//    
//    int size = 0;
//    const unsigned char * bytes = pGameRecording->getData(&size);
//    NSData *data = [NSData dataWithBytes:bytes length:size];
//    PFFile *file = [PFFile fileWithName:@"game.dat" data:data];
//    [file saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
//        if (!error)
//        {
//            PFObject *gameScore = [PFObject objectWithClassName:@"Game"];
//            gameScore[@"userid"] = [currentUser objectId];
//            gameScore[@"bird"] = [NSNumber numberWithInt:pGameRecording->getBirdType()];
//            gameScore[@"score"] = [NSNumber numberWithInt:pGameRecording->getScore()];
//            gameScore[@"length"] = [NSNumber numberWithFloat:pGameRecording->getLength()];
//            gameScore[@"seed"] = [NSNumber numberWithLong:pGameRecording->getSeed()];
//            gameScore[@"fbId"] = currentUser[@"fbId"];
//            gameScore[@"name"] = currentUser[@"name"];
//            gameScore[@"data"] = file;
//            [gameScore saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
//                
//                if (succeeded)
//                {
//                    NSMutableArray * challenges = [[NSMutableArray alloc] init];
//                    for (int i = 0; i < invitedIds.size(); i++)
//                    {
//                        PFObject *challenge = [PFObject objectWithClassName:@"Challenge"];
//                        challenge[@"gameId"] = [gameScore objectId];
//                        challenge[@"from"] = [currentUser objectId];
//                        challenge[@"to"] = [NSString stringWithUTF8String:(invitedIds[i].c_str())];
//                        challenge[@"fromName"] = currentUser[@"name"];
//                        challenge[@"fromImageURL"] = [NSString stringWithFormat:@"http://graph.facebook.com/%@/picture?width=256&height=256", currentUser[@"fbId"]];
//                        challenge[@"fromLevel"] = [NSNumber numberWithInt:Pref::getInstance()->getMultiplayerLevel()];
//                        [challenges addObject:challenge];
//                    }
//                    [PFObject saveAllInBackground:challenges block:^(BOOL succeeded, NSError *error) {
//                        
//                        hideLoadingScreen();
//                        
//                        cocos2d::CCScene * pCurrScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
//                        if (NULL != pCurrScene)
//                        {
//                            for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
//                            {
//                                GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
//                                if (NULL != pGameScene)
//                                {
//                                    pGameScene->onSendChallengeDone(succeeded);
//                                    break;
//                                }
//                            }
//                        }
//                        
//                    }];
//                }
//                else
//                {
//                    hideLoadingScreen();
//                    
//                    cocos2d::CCScene * pCurrScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
//                    if (NULL != pCurrScene)
//                    {
//                        for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
//                        {
//                            GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
//                            if (NULL != pGameScene)
//                            {
//                                pGameScene->onSendChallengeDone(succeeded);
//                                break;
//                            }
//                        }
//                    }
//                }
//            }];
//        }
//        else
//        {
//            hideLoadingScreen();
//            
//            cocos2d::CCScene * pCurrScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
//            if (NULL != pCurrScene)
//            {
//                for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
//                {
//                    GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
//                    if (NULL != pGameScene)
//                    {
//                        pGameScene->onSendChallengeDone(succeeded);
//                        break;
//                    }
//                }
//            }
//        }
//        
//        delete pGameRecording;
//    }];
}

void SystemUtilsIOS::loadGameRecording(const char * gameId)
{
//    PFQuery *query = [PFQuery queryWithClassName:@"Game"];
//    [query getObjectInBackgroundWithId:[NSString stringWithUTF8String:gameId] block:^(PFObject *game, NSError *error) {
//        if (!game)
//        {
//            onLoadGameRecordingDone(NULL);
//        }
//        else
//        {
//            long seed = [[game objectForKey:@"seed"] longValue];
//            int birdType = [[game objectForKey:@"bird"] intValue];
//            GameRecording * pRecording = new GameRecording(seed, birdType);
//            pRecording->setScore([[game objectForKey:@"score"] intValue]);
//            pRecording->setLength([[game objectForKey:@"length"] intValue]);
//            pRecording->setUserId([[game objectForKey:@"userid"] UTF8String]);
//            pRecording->setFbId([[game objectForKey:@"fbId"] UTF8String]);
//            pRecording->setUsername([[game objectForKey:@"name"] UTF8String]);
//            
//            PFFile *dataFile = [game objectForKey:@"data"];
//            [dataFile getDataInBackgroundWithBlock:^(NSData *data, NSError *error) {
//                
//                if (data)
//                {
//                    unsigned int size = (unsigned int)[data length];
//                    unsigned char * bytes = new unsigned char[size];
//                    memcpy(bytes, [data bytes], size);
//                    pRecording->setData(bytes, size);
//                    
//                    onLoadGameRecordingDone(pRecording);
//                }
//                else
//                {
//                    onLoadGameRecordingDone(NULL);
//                }
//                
//            }];
//        }
//    }];
}

void SystemUtilsIOS::loadChallenges(bool cacheOnly)
{
//    PFUser *currentUser = [PFUser currentUser];
//    if (!currentUser)
//    {
//        std::vector<Challenge> challanges;
//        onLoadChallengesDone(challanges);
//        return;
//    }
//    
//    CCLOG("load challanges for %s", [[currentUser objectId] UTF8String]);
//    
//    PFQuery *query1 = [PFQuery queryWithClassName:@"Challenge"];
//    [query1 whereKey:@"to" equalTo:[currentUser objectId]];
//    
//    PFQuery * query = nil;
//    NSString * fbId = currentUser[@"fbId"];
//    if (fbId)
//    {
//        PFQuery *query2 = [PFQuery queryWithClassName:@"Challenge"];
//        [query2 whereKey:@"to" equalTo:currentUser[@"fbId"]];
//
//        query = [PFQuery orQueryWithSubqueries:@[query1,query2]];
//    }
//    else
//    {
//        query = query1;
//    }
//    
//    [query setLimit:10];
//    
//    if (cacheOnly && [query hasCachedResult])
//    {
//        query.cachePolicy = kPFCachePolicyCacheOnly;
//    }
//    [query findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
//        
//        std::vector<Challenge> challanges;
//        
//        if (!error)
//        {
//            for (PFObject *object in objects)
//            {
//                const char * gameId = [[object objectForKey:@"gameId"] UTF8String];
//                Challenge c(gameId);
//                
//                const char * oppId = [[object objectForKey:@"from"] UTF8String];
//                const char * oppName = [[object objectForKey:@"fromName"] UTF8String];
//                const char * oppImageURL = [[object objectForKey:@"fromImageURL"] UTF8String];
//                int oppLevel = [[object objectForKey:@"fromLevel"] intValue];
//                Opponent o(oppId, oppName, oppImageURL, oppLevel);
//                c.addOpponent(o);
//                
//                challanges.push_back(c);
//            }
//        }
//        
//        onLoadChallengesDone(challanges);
//    }];
}

void SystemUtilsIOS::findQuickMatch()
{
//    int low = Pref::getInstance()->getBestScore() / 2;
//    int high = (int)((1 + Pref::getInstance()->getBestScore()) * 1.25f);
//    if (high - low < 2)
//    {
//        low = 1;
//        high = 10;
//    }
//    PFQuery *query = [PFQuery queryWithClassName:@"Game"];
//    [query whereKey:@"score" greaterThan:[NSNumber numberWithInt:low]];
//    [query whereKey:@"score" lessThan:[NSNumber numberWithInt:high]];
//    [query setLimit:10];
//    [query findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
//       
//        if (!error && [objects count] > 0)
//        {
//            int count = (int)[objects count];
//            int randIndex = arc4random() % count;
//            PFObject *game = objects[randIndex];
//            
//            const char * gameId = [[game objectId] UTF8String];
//            
//            long seed = [[game objectForKey:@"seed"] longValue];
//            int birdType = [[game objectForKey:@"bird"] intValue];
//            const char * fbId = [[game objectForKey:@"fbId"] UTF8String];
//            if (NULL == fbId || 0 == strlen(fbId))
//            {
//                birdType = rand()%40; // only hack bird type on dummy recording
//            }
//            GameRecording * pRecording = new GameRecording(seed, birdType);
//            pRecording->setScore([[game objectForKey:@"score"] intValue]);
//            pRecording->setLength([[game objectForKey:@"length"] intValue]);
//            pRecording->setUserId([[game objectForKey:@"userid"] UTF8String]);
//            pRecording->setUsername([[game objectForKey:@"name"] UTF8String]);
//            pRecording->setFbId(fbId);
//            
//            onQuickMatchFound(gameId, pRecording->getBirdType(), pRecording->getFbId(), pRecording->getUsername());
//            
//            PFFile *dataFile = [game objectForKey:@"data"];
//            [dataFile getDataInBackgroundWithBlock:^(NSData *data, NSError *error) {
//                
//                if (data)
//                {
//                    unsigned int size = (unsigned int)[data length];
//                    unsigned char * bytes = new unsigned char[size];
//                    memcpy(bytes, [data bytes], size);
//                    pRecording->setData(bytes, size);
//                    
//                    onLoadGameRecordingDone(pRecording);
//                }
//                else
//                {
//                    onLoadGameRecordingDone(NULL);
//                }
//                
//            }];
//
//        }
//        else
//        {
//            onQuickMatchFound(NULL, -1, NULL, NULL);
//        }
//    }];
}

void SystemUtilsIOS::clearChallange(std::string gameId)
{
//    PFQuery *query = [PFQuery queryWithClassName:@"Challenge"];
//    [query whereKey:@"gameId" equalTo:[NSString stringWithUTF8String:gameId.c_str()]];
//    [query findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
//        
//        if (!error && [objects count] > 0)
//        {
//            [objects[0] deleteInBackground];
//        }
//        
//    }];
}

void SystemUtilsIOS::notifyUserMinActionComplete()
{
    //[Tapjoy actionComplete:@"a3255332-1572-4890-a5fa-c6f0a50de25e"];
}

void SystemUtilsIOS::restoreFacebookSession()
{
//    if (FBSession.activeSession.state == FBSessionStateOpen)
//    {
//    }
//    else if (FBSession.activeSession.state == FBSessionStateCreatedTokenLoaded)
//    {
//        [FBSession.activeSession openWithBehavior:FBSessionLoginBehaviorUseSystemAccountIfPresent completionHandler:^(FBSession *session, FBSessionState state, NSError *error) {
//            
//        }];
//    }
}

void SystemUtilsIOS::loginToFacebook()
{
//    m_bLoginToInvite = false; // to clear previous flags that we might have
//    
//    trackEvent("fb", "connect", "start");
//    
//    showLoadingScreen();
//    
//    [FBSession openActiveSessionWithReadPermissions:@[@"public_profile,email"] allowLoginUI:TRUE
//          completionHandler:^(FBSession *session, FBSessionState state, NSError *error) {
//              if (!error)
//              {
//                  BOOL isLoggedIn = FBSession.activeSession.state == FBSessionStateOpen || FBSession.activeSession.state == FBSessionStateOpenTokenExtended;
//                  if (isLoggedIn)
//                  {
//                      trackEvent("fb", "connect", "done");
//                      getUserDetails();
//                  }
//              }
//              else
//              {
//                  hideLoadingScreen();
//                  trackEvent("fb", "connect", "failed");
//              }
//          }
//     ];
}

// ===========================================================
// Private methods
// ===========================================================

void SystemUtilsIOS::onLoadGameRecordingDone(GameRecording * pGameRecording)
{
    cocos2d::Scene * pCurrScene = cocos2d::Director::getInstance()->getRunningScene();
    if (NULL != pCurrScene)
    {
        for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
        {
            GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
            if (NULL != pGameScene)
            {
                pGameScene->onGameRecordingLoaded(pGameRecording);
                break;
            }
        }
    }
}

void SystemUtilsIOS::onLoadChallengesDone(std::vector<Challenge> challenges)
{
    cocos2d::Scene * pCurrScene = cocos2d::Director::getInstance()->getRunningScene();
    if (NULL != pCurrScene)
    {
        for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
        {
            GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
            if (NULL != pGameScene)
            {
                pGameScene->onLoadChallengesDone(challenges);
                break;
            }
        }
    }
}

void SystemUtilsIOS::onQuickMatchFound(const char * gameId, int birdType, const char * fbId, const char * name)
{
    cocos2d::Scene * pCurrScene = cocos2d::Director::getInstance()->getRunningScene();
    if (NULL != pCurrScene)
    {
        for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
        {
            GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
            if (NULL != pGameScene)
            {
                pGameScene->onQuickMatchFound(gameId, birdType, fbId, name);
                break;
            }
        }
    }
}

void SystemUtilsIOS::getUserDetails()
{
//    trackEvent("fb", "connect-details", "start");
//    
//    [FBRequestConnection startForMeWithCompletionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
//        
//        if (!error)
//        {
//            NSString * fbId = (NSString *)[result valueForKey:@"id"];
//            NSString * name = (NSString *)[result valueForKey:@"first_name"];
//            NSString * email = (NSString *)[result valueForKey:@"email"];
//            NSString * gender = (NSString *)[result valueForKey:@"gender"];
//            if (nil == gender)
//            {
//                gender = @"unknown";
//            }
//            
//            PFUser *currentUser = [PFUser currentUser];
//            currentUser[@"gender"] = gender;
//            currentUser[@"fbId"] = fbId;
//            currentUser[@"name"] = name;
//            currentUser[@"fbEmail"] = email;
//            currentUser[@"winCount"] = [NSNumber numberWithInt:Pref::getInstance()->getMultiplayerWins()];
//            [currentUser saveInBackgroundWithBlock:^(BOOL succeeded, NSError *error) {
//                
//                hideLoadingScreen();
//                
//                if (succeeded)
//                {
//                    Pref::getInstance()->setUserDetails([fbId UTF8String], [name UTF8String]);
//                    Pref::getInstance()->save();
//                    
//                    trackEvent("fb", "connect-details", "done");
//                    
//                    cocos2d::Scene * pCurrScene = cocos2d::Director::getInstance()->getRunningScene();
//                    if (NULL != pCurrScene)
//                    {
//                        for (int i = 0; i < pCurrScene->getChildrenCount(); i++)
//                        {
//                            GameScene * pGameScene = dynamic_cast<GameScene*>(pCurrScene->getChildren().at(i));
//                            if (NULL != pGameScene)
//                            {
//                                pGameScene->onLoginDone();
//                                break;
//                            }
//                        }
//                    }
//                    
//                    if (m_bLoginToInvite)
//                    {
//                        m_bLoginToInvite = false;
//                        invite(m_iScoreForPostLoginInvite);
//                    }
//                }
//                
//            }];
//        }
//        else
//        {
//            trackEvent("fb", "connect-details", "failed");
//            hideLoadingScreen();
//        }
//        
//    }];
}

void SystemUtilsIOS::updateFbUserList()
{
//    CCLOG("updateFbUserList");
//    
//    PFQuery * userQuery = [PFUser query];
//    [userQuery whereKey:@"fbId" notEqualTo:NSNull.null];
//    [userQuery setLimit:1000];
//    [userQuery findObjectsInBackgroundWithBlock:^(NSArray *objects, NSError *error) {
//        
//        if (!error && [objects count] > 0)
//        {
//            m_fbIds.clear();
//            
//            for (int i = 0; i < [objects count]; i++)
//            {
//                PFUser *user = objects[i];
//                
//                //TODO: need to save facebook id and name
//                
//                m_fbIds.push_back([[user objectForKey:@"fbId"] UTF8String]);
//            }
//            
//            //TODO: save to cache
//        }
//    }];
}

void SystemUtilsIOS::restoreFbUserList()
{
    //TODO: load from cache
}