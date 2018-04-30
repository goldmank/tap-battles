//
//  FacebookAppActivity.h
//  Socio
//
//  Created by Mark Ostrovsky on 12/28/14.
//
//

#import <UIKit/UIKit.h>

@interface FacebookAppActivity : UIActivity

@end

@interface FacebookMessage : NSObject

@property (nonatomic, strong, readonly) NSString *text;
@property (nonatomic, strong, readonly) NSString *link;

- (id)initWithMessage:(NSString *)message forLink:(NSString *)link;

@end