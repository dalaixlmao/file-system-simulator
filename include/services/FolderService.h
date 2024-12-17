// include/services/FolderService.h

#ifndef FOLDERSERVICE_H
#define FOLDERSERVICE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "../storage/Storage.h"
using namespace std;

class FolderService
{
private:
    Storage *store;

public:
    void createFolder(string parentFolderId, string folderName);
    void removeFolder(string folderName);
    void showTree(string folderId);
    void listAllItems(string folderId);
    string getCurrentFolder();
    void showFolderPath(string folderId);
    void getIntoFolder(string folderName);
    FolderService();
    ~FolderService() = default;
};

#endif