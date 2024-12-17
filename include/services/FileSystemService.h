// include/services/FileSystemService.h

#ifndef FILESYSTEMSERVICE_H
#define FILESYSTEMSERVICE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "./FileService.h"
#include "./FolderService.h"
#include "../storage/Storage.h"
using namespace std;

class FileSystemService
{
private:
    Storage *store;
    FileService *fileService;
    FolderService *folderService;

public:
    void createFile(string folderId, string fileName);
    string getCurrentFolder();
    void addContent(string fileName, string content);
    void removeFile(string fileName);
    string showFileContent(string fileId);
    void createFolder(string parentFolderId, string folderName);
    void removeFolder(string folderName);
    void showTree(string folderId);
    void listAllItems(string folderId);
    void getIntoFolder(string folderName);
    bool isFolderAvailable(string name);
    string currentPath();
    FileSystemService();
    ~FileSystemService() = default;
};

#endif