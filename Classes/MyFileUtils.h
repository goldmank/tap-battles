//
//  MyFileUtils.h
//  tapfly
//
//  Created by Kfir Goldman on 3/21/15.
//
//

#ifndef __tapfly__MyFileUtils__
#define __tapfly__MyFileUtils__

#include <string>

using namespace std;

class MyFileUtils {
public:
    static string getResourcePath(const string &path);
    static string getDocumentPath(const string &path);
    
    /* Create a director in writable directory */
    static bool createDirectory(const string &dirname);
    
    static bool isFileExist(const string &fileOrDirectory);
    
    static bool createFile(const unsigned char *buff, long size, const string &filepath);
    static bool deleteFile(const string &filepath);
    static bool createFile(const char *buff, const string &filepath);
};


#endif /* defined(__tapfly__MyFileUtils__) */
