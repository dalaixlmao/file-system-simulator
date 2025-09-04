// include/services/FileSystemService.h

#ifndef FILESYSTEMSERVICE_H
#define FILESYSTEMSERVICE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "./FileService.h"
#include "./FolderService.h"
#include "./HistoryService.h"
#include "./GrepService.h"
#include "../storage/Storage.h"
using namespace std;

class FileSystemService
{
private:
    Storage *store;
    FileService *fileService;
    FolderService *folderService;
    HistoryService *historyService;
    GrepService *grepService;

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
    
    // History operations
    void showHistory() const;
    void showHistory(int count) const;
    void clearHistory();
    int getHistoryCount() const;
    
    // Grep operations
    void grepPattern(const string& pattern);
    void grepInFile(const string& pattern, const string& fileName);
    void grepRecursive(const string& pattern);
    void grepWithOptions(const string& pattern, const string& options);
    void showGrepHelp();
    
    FileSystemService();
    ~FileSystemService() = default;
};

#endif