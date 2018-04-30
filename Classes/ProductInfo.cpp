//
//  ProductInfo.cpp
//  Socio
//
//  Created by Kfir Goldman on 10/14/14.
//
//

#include "ProductInfo.h"

// ===========================================================
// Initialization & cleanup
// ===========================================================

ProductInfo::ProductInfo(const char * productId, const char * name, const char * localPrice, const char * price, const char * currency, const void * pData)
: m_id(productId)
, m_name(name)
, m_localPrice(localPrice)
, m_price(price)
, m_currency(currency)
, m_pData(pData)
{
}

ProductInfo::~ProductInfo()
{
}