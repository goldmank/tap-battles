#import "IAPHelper.h"
#import <StoreKit/StoreKit.h>
#include "SystemUtils.h"

NSString *const IAPHelperProductPurchasedNotification = @"IAPHelperProductPurchasedNotification";

// 2
@interface IAPHelper () <SKProductsRequestDelegate, SKPaymentTransactionObserver>
@end

// 3
@implementation IAPHelper {
    SKProductsRequest * _productsRequest;
    RequestProductsCompletionHandler _completionHandler;
    
    NSSet * _productIdentifiers;
    NSMutableSet * _purchasedProductIdentifiers;
}

+ (IAPHelper *)sharedInstance {
    static dispatch_once_t once;
    static IAPHelper * sharedInstance;
    dispatch_once(&once, ^{
        NSSet * productIdentifiers = [NSSet setWithObjects:
                                      @"coins_1", @"coins_2", @"coins_3", @"coins_4",
                                      nil];
        sharedInstance = [[self alloc] initWithProductIdentifiers:productIdentifiers];
    });
    return sharedInstance;
}

- (id)initWithProductIdentifiers:(NSSet *)productIdentifiers {
    
    if ((self = [super init])) {
        
        // Store product identifiers
        _productIdentifiers = productIdentifiers;
        
        // Check for previously purchased products
        _purchasedProductIdentifiers = [[NSMutableSet alloc] init]; 
        for (NSString * productIdentifier in _productIdentifiers) {
            BOOL productPurchased = [[NSUserDefaults standardUserDefaults] boolForKey:productIdentifier];
            if (productPurchased) {
                [_purchasedProductIdentifiers addObject:productIdentifier];
                NSLog(@"Previously purchased: %@", productIdentifier);
            } else {
                NSLog(@"Not purchased: %@", productIdentifier);
            }
        }
        
        // Add self as transaction observer
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        
    }
    return self;
    
}

- (void)requestProductsWithCompletionHandler:(RequestProductsCompletionHandler)completionHandler{
    
    // 1
    _completionHandler = [completionHandler copy];
    
    // 2
    _productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers];
    _productsRequest.delegate = self;
    [_productsRequest start];
    
}

- (BOOL)productPurchased:(NSString *)productIdentifier
{
    return [_purchasedProductIdentifiers containsObject:productIdentifier];
}

- (void)buyProduct:(SKProduct *)product
{
    SKPayment * payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

#pragma mark - SKProductsRequestDelegate

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    
    NSLog(@"Loaded list of products...");
    _productsRequest = nil;
    
    NSArray * skProducts = response.products;
    for (SKProduct * skProduct in skProducts) {
        NSLog(@"Found product: %@ %@ %0.2f",
              skProduct.productIdentifier,
              skProduct.localizedTitle,
              skProduct.price.floatValue);
    }
    
    _completionHandler(YES, skProducts);
    _completionHandler = nil;
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"Failed to load list of products.");
    _productsRequest = nil;
    
    _completionHandler(NO, nil);
    _completionHandler = nil;    
}

#pragma mark SKPaymentTransactionOBserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction * transaction in transactions) {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
            break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
            break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
            break;
        }
    };
}

- (NSString*)base64forData:(NSData*)theData {
    
    const uint8_t* input = (const uint8_t*)[theData bytes];
    NSInteger length = [theData length];
    
    static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    
    NSMutableData* data = [NSMutableData dataWithLength:((length + 2) / 3) * 4];
    uint8_t* output = (uint8_t*)data.mutableBytes;
    
    NSInteger i;
    for (i=0; i < length; i += 3) {
        NSInteger value = 0;
        NSInteger j;
        for (j = i; j < (i + 3); j++) {
            value <<= 8;
            
            if (j < length) {
                value |= (0xFF & input[j]);
            }
        }
        
        NSInteger theIndex = (i / 3) * 4;
        output[theIndex + 0] =                    table[(value >> 18) & 0x3F];
        output[theIndex + 1] =                    table[(value >> 12) & 0x3F];
        output[theIndex + 2] = (i + 1) < length ? table[(value >> 6)  & 0x3F] : '=';
        output[theIndex + 3] = (i + 2) < length ? table[(value >> 0)  & 0x3F] : '=';
    }
    
    return [[[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding] autorelease];
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"completeTransaction...%@", transaction);
    
    [self provideContentForProductIdentifier:transaction.payment.productIdentifier];
   
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    const char * p =[transaction.payment.productIdentifier UTF8String];
    
    if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_6_1) {
        // iOS 6.1 or earlier.
        // Use SKPaymentTransaction's transactionReceipt.
        NSLog(@"iOS 6.1 or earlier.");
        NSString * rec = [self base64forData:transaction.transactionReceipt];
        SystemUtils::getInstance()->getPurchaseListener()->onPurchaseSucceeded(p, [rec UTF8String], "");
    } else {
        // iOS 7 or later.
        NSLog(@"iOS 7 or later");
        NSURL *receiptFileURL = nil;
        NSBundle *bundle = [NSBundle mainBundle];
        if ([bundle respondsToSelector:@selector(appStoreReceiptURL)]) {
            
            // Get the transaction receipt file path location in the app bundle.
            receiptFileURL = [bundle appStoreReceiptURL];
            NSData *data = [NSData dataWithContentsOfURL:receiptFileURL];
            NSString * rec = [data base64EncodedStringWithOptions:0];
            NSLog(@"call Purchase succeeded p=%s r=%@", p, rec);
            SystemUtils::getInstance()->getPurchaseListener()->onPurchaseSucceeded(p, [rec UTF8String], "");
            // Read in the contents of the transaction file.
            
        } else {
            NSLog(@"Fall back to deprecated transaction receipt");
            // which is still available in iOS 7.
            
            // Use SKPaymentTransaction's transactionReceipt.
             NSString * rec = [self base64forData:transaction.transactionReceipt];
            SystemUtils::getInstance()->getPurchaseListener()->onPurchaseSucceeded(p, [rec UTF8String], "");

        }
    }
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    NSLog(@"restoreTransaction...");
    
    [self provideContentForProductIdentifier:transaction.originalTransaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    
    NSLog(@"failedTransaction...");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
        SystemUtils::getInstance()->getPurchaseListener()->onPurchaseFailed([transaction.payment.productIdentifier UTF8String],[transaction.error.localizedDescription UTF8String]);
    }
    else
    {
        if (NULL != SystemUtils::getInstance() && NULL != SystemUtils::getInstance()->getPurchaseListener())
        {
            SystemUtils::getInstance()->getPurchaseListener()->onPurchaseCancelled();
        }
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)provideContentForProductIdentifier:(NSString *)productIdentifier {
    
    [_purchasedProductIdentifiers addObject:productIdentifier];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:productIdentifier];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [[NSNotificationCenter defaultCenter] postNotificationName:IAPHelperProductPurchasedNotification object:productIdentifier userInfo:nil];
    
}

- (void)restoreCompletedTransactions {
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

@end