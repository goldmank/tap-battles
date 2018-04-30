#ifndef __PRODUCT_INFO_H__
#define __PRODUCT_INFO_H__

#include <string>

class ProductInfo
{
public:
    
    // ===========================================================
    // Initialization & cleanup
    // ===========================================================
    
    ProductInfo(const char * productId, const char * name, const char * localPrice, const char * price, const char * currency, const void * pData);
    ~ProductInfo();
    
    // ===========================================================
    // Public methods
    // ===========================================================
    
    std::string & getId() { return m_id; }
    std::string & getName() { return m_name; }
    std::string & getLocalPrice() { return m_localPrice; }
    std::string & getPrice(){ return m_price;}
    std::string & getCurrency(){ return m_currency;}
    const void * getData() { return m_pData; }
    
private:
    
    // ===========================================================
    // Fields
    // ===========================================================
    
    std::string m_id;
    std::string m_name;
    std::string m_localPrice;
    std::string m_price;
    std::string m_currency;
    const void * m_pData;
};

#endif // __PRODUCT_INFO_H__