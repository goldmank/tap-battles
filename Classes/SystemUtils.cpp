#include "SystemUtils.h"

// =========================================================== 
// Static memeber
// ===========================================================

SystemUtils * SystemUtils::s_pInstance = NULL;

// =========================================================== 
// Singleton methods
// ===========================================================

void SystemUtils::initInstance(SystemUtils * pSystemUtils)
{
	s_pInstance = pSystemUtils;
}

SystemUtils * SystemUtils::getInstance()
{
	return s_pInstance;
}

// ===========================================================
// Public methods
// ===========================================================

void SystemUtils::initProducts(std::vector<ProductInfo> & products)
{
    m_products = products;
}

void SystemUtils::addProductInfo(const ProductInfo & prodInfo)
{
    m_products.push_back(prodInfo);
}

ProductInfo * SystemUtils::getProductByIndex(int index)
{
    if (index >= 0 && index < m_products.size())
    {
        return &m_products[index];
    }
    return NULL;
}

ProductInfo * SystemUtils::getProductById(const char * itemId)
{
    for (int i = 0; i < m_products.size(); i++)
    {
        if (0 == strcmp(itemId, m_products[i].getId().c_str()))
        {
            return &m_products[i];
        }
    }
    return NULL;
}

// ===========================================================
// Protected Constructor
// ===========================================================

SystemUtils::SystemUtils()
{
    m_pPurchaseListener = NULL;
}
