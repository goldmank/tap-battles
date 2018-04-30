@interface GameCenterIos : NSObject
{
}

+ (GameCenterIos*)shared;

- (void)showLoginFailedAlert:(NSError*)error;
- (void)login;
- (void)loginLeaderboard;
- (void)loginAchivments;

- (BOOL)isLoggedIn;
- (BOOL)showAchievements;
- (void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete;
- (void)clearAllAchivements;

- (BOOL)showScores;
- (void)postScore:(const char*)idName score:(NSNumber*)score;
- (void)clearAllScores;

@end
