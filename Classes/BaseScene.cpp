#include "BaseScene.h"
#include "Pref.h"
#include "MyFileUtils.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;
using namespace network;
using namespace ui;

// =========================================================== 
// Initialization & cleanup
// ===========================================================

BaseScene::BaseScene()
: m_iScreenWidth(0),
  m_iScreenHeight(0),
  m_fTime(0)
{
    m_pTouchListener = EventListenerTouchOneByOne::create();
    m_pTouchListener->setSwallowTouches(true);
    m_pTouchListener->onTouchBegan = CC_CALLBACK_2(BaseScene::onTouchBegan, this);
    m_pTouchListener->onTouchMoved = CC_CALLBACK_2(BaseScene::onTouchMoved, this);
    m_pTouchListener->onTouchEnded = CC_CALLBACK_2(BaseScene::onTouchEnded, this);
    m_pTouchListener->onTouchCancelled = CC_CALLBACK_2(BaseScene::onTouchCancelled, this);
    m_pTouchListener->retain();
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);
}

BaseScene::~BaseScene()
{
    getEventDispatcher()->removeEventListener(m_pTouchListener);
    m_pTouchListener->release();
    m_pTouchListener = nullptr;
}

// ===========================================================
// CCLayer methods
// ===========================================================

bool BaseScene::init()
{
    Color3B c = getFillColor();
	if (!CCLayerColor::initWithColor(ccc4(c.r, c.g, c.b, 255)))
    {
        return false;
    }
	
	setTouchEnabled(true);
	schedule(schedule_selector(BaseScene::update));

    CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_iScreenWidth = size.width;
	m_iScreenHeight = size.height;
    
	handleInit(size.width, size.height);
			
    return true;
}

// =========================================================== 
// Public methods
// ===========================================================

void BaseScene::update(float dt)
{
	m_fTime += dt;	
	handleUpdate(dt);
}

// =========================================================== 
// Common methods
// ===========================================================
	
void BaseScene::handleInit(float width, float height)
{
}

void BaseScene::handleUpdate(float dt)
{
}

// =========================================================== 
// Helper methods
// ===========================================================

int BaseScene::getScreenWidth()
{
	return m_iScreenWidth;
}

int BaseScene::getScreenHeight()
{
	return m_iScreenHeight;
}

bool BaseScene::isClicked(CCNode * pSprite, Touch * pTouch)
{
	if (NULL == pSprite)
	{
		return false;
	}
	CCSize size = pSprite->boundingBox().size;
	CCPoint point = pSprite->convertTouchToNodeSpace(pTouch);
	if (point.x > 0 && point.y > 0 && point.x < size.width && point.y < size.height)
	{
		return true;
	}
	return false;
}

float BaseScene::getTime()
{
	return m_fTime;
}

void BaseScene::switchScene(CCScene * pScene)
{
	CCTransitionScene *transition =  CCTransitionCrossFade::create(0.4f, pScene);
	CCDirector::sharedDirector()->replaceScene(transition);
}

cocos2d::ccColor3B BaseScene::getFillColor()
{
    return ccc3(255, 255, 255);
}

void BaseScene::loadBatch(const char ** batches, const char * postfix)
{
    int batchIndex = 0;
    while (NULL != batches[batchIndex])
    {
        char pvr[30];
        sprintf(pvr, "%s.%s", batches[batchIndex], postfix);
        CCSpriteBatchNode * pGameBatch = CCSpriteBatchNode::create(pvr);
        if (NULL != pGameBatch)
        {
            addChild(pGameBatch);
            
            char plist[30];
            sprintf(plist, "%s.plist", batches[batchIndex]);
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist);
        }
        
        batchIndex++;
    }
}

void BaseScene::playBackgroundMusic()
{
    if (Pref::getInstance()->isSoundEnabled())
    {
        if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        {
        	SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/bg.mp3", true);
        }
    }
}

void BaseScene::stopBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void BaseScene::playSound(const char * name)
{
    if (Pref::getInstance()->isSoundEnabled())
    {
    	SimpleAudioEngine::getInstance()->playEffect(name);
    }
}

BaseScene::LoadingImageDetails BaseScene::getLoadingImageDetails(const char * url)
{
    return m_mapUrlToSprite[url];
}

void BaseScene::addLoadingImageDetails(const char * url, cocos2d::CCNode * pNode, cocos2d::CCPoint pos)
{
    LoadingImageDetails & data = m_mapUrlToSprite[url];
    
    LoadingImageDetailsItem item;
    item.pContainer = pNode;
    item.pos = pos;
    
    data.items.push_back(item);
}

void BaseScene::removeLoadingImageDetails(const char * url)
{
    LoadingImageDetails & data = m_mapUrlToSprite[url];
    data.items.clear();
}

void BaseScene::clearAllLoadingImageDetails()
{
    m_mapUrlToSprite.clear();
}

bool BaseScene::isImageExist(const char * url)
{
    char cacheFileName[400];
    getImageCacheFileName(cacheFileName, url);
    
    return MyFileUtils::isFileExist(MyFileUtils::getDocumentPath(cacheFileName));
}

void BaseScene::downloadImage(const char * url)
{
    if (m_downloadingImages.find(url) != m_downloadingImages.end())
    {
        return;
    }
    m_downloadingImages.insert(url);
    
    CCLOG("download: %s", url);
    
    HttpRequest* request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(BaseScene::imageLoadedCallback));
    request->setTag(url);
    HttpClient::getInstance()->send(request);
    request->release();
}

const char * BaseScene::getImageCacheFileName(char * dest, const char * url)
{
    int len = (int)strlen(url);
    int startIndex = 0;
    if (len > 50)
    {
        startIndex = len - 30;
    }
    
    int index = 0;
    for (int i = startIndex; i < len; i++)
    {
        char ch = url[i];
        if (ch == '/' || ch == ':' || ch == '.' || ch == '?' || ch == '&' || ch == '=')
        {
            ch = '_';
        }
        dest[index++] = ch;
    }
    dest[index++] = '.';
    dest[index++] = 'j';
    dest[index++] = 'p';
    dest[index++] = 'g';
    dest[index] = '\0';
    
    return dest;
}

cocos2d::CCTexture2D * BaseScene::loadTexture(const char * url)
{
    char cacheFileName[200];
    getImageCacheFileName(cacheFileName, url);
        
    string path = MyFileUtils::getDocumentPath(cacheFileName);
    
    Image *image = new Image();
    
    bool imageLoaded = false;
    if (MyFileUtils::isFileExist(path))
    {
        imageLoaded = image->initWithImageFile(path.c_str());
    }
    if (!imageLoaded)
    {
        CCLOG("use default profile image");
        imageLoaded = image->initWithImageFile("defimage.png");
    }
    
    if (imageLoaded)
    {
        CCTexture2D *texture = new CCTexture2D();
        if (texture->initWithImage(image))
        {
            texture->autorelease();
            image->release();
            return texture;
        }
        texture->release();
    }
    else
    {
        CCLOG("initWithImageFile FAILED: %s", path.c_str());
    }
    
    image->release();
    
    return NULL;
}

// ===========================================================
// Private methods
// ===========================================================

bool BaseScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    return handleOnTouchBegan(touch, event);
}

void BaseScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    handleOnTouchMoved(touch, event);
}

void BaseScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    handleOnTouchEnded(touch, event);
}

void BaseScene::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event)
{
    handleOnTouchCancelled(touch, event);
}

void BaseScene::imageLoadedCallback(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    const char * url = response->getHttpRequest()->getTag();
    m_downloadingImages.erase(url);
    
    if (200 == response->getResponseCode())
    {
        char cacheFileName[200];
        getImageCacheFileName(cacheFileName, url);
        
        std::string imageData(response->getResponseData()->begin(), response->getResponseData()->end());
        
        MyFileUtils::createFile((unsigned char *)imageData.c_str(), response->getResponseData()->size(), MyFileUtils::getDocumentPath(cacheFileName));
        
        onImageDownloaded(url);
    }
    else
    {
        CCLOG("failed to download: %s", url);
    }
}
