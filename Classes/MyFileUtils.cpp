#include "MyFileUtils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>

#include "cocos2d.h"
using namespace cocos2d;

string MyFileUtils::getResourcePath(const string &path)
{
    return CCFileUtils::sharedFileUtils()->fullPathForFilename(path.c_str());
}

string MyFileUtils::getDocumentPath(const string &path)
{
    return CCFileUtils::sharedFileUtils()->getWritablePath() + "/" + path;
}

bool MyFileUtils::createDirectory(const string &dirname)
{
    string path = CCFileUtils::sharedFileUtils()->getWritablePath() + string("/") + dirname;
    int ret = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    return ret == 0;
}

bool MyFileUtils::isFileExist(const string &fileOrDirectory)
{
    return CCFileUtils::sharedFileUtils()->isFileExist(fileOrDirectory);
}

bool MyFileUtils::createFile(const unsigned char *buff, long size, const string &filepath) {
    std::ofstream ofs(filepath.c_str(), std::ofstream::binary);
    if (ofs.fail()) {
        CCLOG("FileUtils::createFile - failed to create %s", filepath.c_str());
        return false;
    }
    
    ofs.write((const char *)buff, size);
    ofs.close();
    
    if (ofs.fail()) {
        CCLOG("FileUtils::createFile - something wrong when write %s", filepath.c_str());
    }
    
    return true;
}

bool MyFileUtils::deleteFile(const string &filepath)
{
    return remove(filepath.c_str());
}

bool MyFileUtils::createFile(const char *buff, const string &filepath)
{
    std::ofstream ofs(filepath.c_str(), std::ofstream::binary);
    if (ofs.fail()) {
        CCLOG("FileUtils::createFile - failed to create %s", filepath.c_str());
        return false;
    }
    
    ofs.write(buff, strlen(buff));
    ofs.close();
    
    return true;
}