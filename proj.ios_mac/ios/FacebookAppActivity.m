//
//  FacebookAppActivity.m
//  Socio
//
//  Created by Mark Ostrovsky on 12/28/14.
//
//

#import "FacebookAppActivity.h"
#import <FacebookSDK/FacebookSDK.h>

@interface FacebookAppActivity ()

@property (nonatomic, strong) FacebookMessage *message;

@end


@implementation FacebookAppActivity

- (NSString *)activityType {
    return @"honestly.FACEBOOK";
}

- (UIImage *)activityImage
{
    return [UIImage imageNamed:@"facebook"];
}

- (NSString *)activityTitle
{
    return @"facebook";
}

- (BOOL)canPerformWithActivityItems:(NSArray *)activityItems
{
    for (id activityItem in activityItems)
    {
        if ([activityItem isKindOfClass:[FacebookMessage class]])
        {
            self.message = activityItem;
         
            return true;
        }
    }
    return NO;
}

+ (UIActivityCategory)activityCategory
{
    return UIActivityCategoryShare;
}

- (void)prepareWithActivityItems:(NSArray *)activityItems
{
    for (id activityItem in activityItems)
    {
        if ([activityItem isKindOfClass:[FacebookMessage class]])
        {
            NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                           _message.text, @"name",
                                           @"", @"description",
                                           _message.link, @"link",
                                           nil];
            
            // Show the feed dialog
            [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                                   parameters:params
                                                      handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                                          if (error) {
                                                              
                                                              NSLog(@"Error publishing story: %@", error.description);
                                                          } else {
                                                              if (result == FBWebDialogResultDialogNotCompleted) {
                                                                  // User cancelled.
                                                                  NSLog(@"User cancelled.");
                                                              } else {
                                                                  
                                                              }
                                                          }
                                                      }];

        }
    }
}



@end


#pragma mark - FacebookMessage Class

@implementation FacebookMessage
@synthesize text = _text;
@synthesize link = _link;

- (id)init
{
    return [self initWithMessage:nil forLink:nil];
}

/** Designated initializer*/
- (id)initWithMessage:(NSString *)message forLink:(NSString *)link
{
    self = [super init];
    
    if (self)
    {
        _text = message && message.length > 0 ? message : nil;
        _link = link && link.length > 0 ? link : nil;
    }
    
    return self;
}

- (NSString *)text
{
    return _text;
}

- (NSString *)link
{
    return _link;
}

- (NSString *)stringByEncodingString:(NSString *)string
{
    CFStringRef encodedString = CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)string, NULL,
                                                                        (CFStringRef)@"!*'();:@&=+$,/?%#[]", kCFStringEncodingUTF8);
    return CFBridgingRelease(encodedString);
}

@end
