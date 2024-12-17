// include/storage/Storage.h

#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "../models/FileSystem.h"
#include "../models/File.h"
#include "../models/Folder.h"

using namespace std;

class Storage
{
private:
    map<string, map<string, int>> tree;
    FileSystem *fileSystem;
    map<string, Folder *> folders;
    map<string, File *> files;
    Storage();
    static Storage* instance;

public:
    static Storage* getInstance();
    void addContent(string fileName, string content);
    string getNewFileId();
    string getNewFolderId();
    void addFile(string name, string folderId);
    void addFolder(string name, string parentFodlerId);
    Folder *getFolder(string id);
    File *getFile(string id);
    void showFolderPath(string id);
    void showFilePath(string id);
    void showItemsInFolder(string folderId);
    void getIntoFolder(string name);
    bool validateFolder(string folderName);
    void removeFile(string fileName);
    bool validateFile(string fileName);
    void removeFolder(string folderName);
    string getPath(string id);
    void removeDFS(string id);
    void showFolderTree();
    void showDFS(string folderId, string symbols);
    string getCurrentFolderId();
    ~Storage() = default;
};

#endif