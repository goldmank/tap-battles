#ifndef __PURCHASE_LISTENER_H__
#define __PURCHASE_LISTENER_H__

class PurchaseListener
{
public:
    
    virtual ~PurchaseListener() {}
    
    virtual void onPurchaseSucceeded(const char * prodId, const char * receipt, const char * purchase) = 0;
    virtual void onPurchaseFailed(const char * prodId, const char * msg) = 0;
    virtual void onPurchaseCancelled() = 0;
};

#endif // __PURCHASE_LISTENER_H__