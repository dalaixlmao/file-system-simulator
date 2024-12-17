// include/services/FileService.h

#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "../storage/Storage.h"

using namespace std;

class FileService
{
private:
    Storage *store;

public:
    void createFile(string folderId, string fileName);
    void addContent(string fileId, string content);
    void removeFile(string filename);
    string showFileContent(string fileId);
    void showFilePath(string fileId);
    FileService();
    ~FileService() = default;
};

#endif