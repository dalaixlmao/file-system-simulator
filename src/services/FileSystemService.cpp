// src/services/FileSystemService.cpp

#include "../../include/services/FileSystemService.h"
#include "../../include/storage/Storage.h"
#include "../../include/services/FileService.h"
#include "../../include/services/FolderService.h"
#include "../../include/services/HistoryService.h"
#include "../../include/services/GrepService.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>

using namespace std;

void FileSystemService::createFile(string folderId, string fileName) 
{ 
    fileService->createFile(folderId, fileName); 
    historyService->addEntry("touch " + fileName, "CREATE_FILE", fileName, currentPath());
}

string FileSystemService::getCurrentFolder() { return folderService->getCurrentFolder(); }

void FileSystemService::addContent(string fileId, string content) 
{ 
    fileService->addContent(fileId, content); 
    historyService->addEntry("write " + fileId + " " + content, "WRITE_FILE", fileId, currentPath());
}

void FileSystemService::removeFile(string fileName) 
{ 
    fileService->removeFile(fileName); 
    historyService->addEntry("rm " + fileName, "REMOVE_FILE", fileName, currentPath());
}

string FileSystemService::showFileContent(string fileId) { return fileService->showFileContent(fileId); }

void FileSystemService::createFolder(string parentFolderId, string folderName) 
{ 
    folderService->createFolder(parentFolderId, folderName); 
    historyService->addEntry("mkdir " + folderName, "CREATE_FOLDER", folderName, currentPath());
}

void FileSystemService::removeFolder(string folderName) 
{ 
    folderService->removeFolder(folderName); 
    historyService->addEntry("rmdir " + folderName, "REMOVE_FOLDER", folderName, currentPath());
}

void FileSystemService::showTree(string folderId) 
{ 
    folderService->showTree(folderService->getCurrentFolder()); 
    historyService->addEntry("tree", "SHOW_TREE", "", currentPath());
}

void FileSystemService::listAllItems(string folderId) 
{ 
    folderService->listAllItems(folderId); 
    historyService->addEntry("ls", "LIST_ITEMS", "", currentPath());
}

void FileSystemService::getIntoFolder(string folderName) 
{ 
    folderService->getIntoFolder(folderName); 
    historyService->addEntry("cd " + folderName, "CHANGE_DIR", folderName, currentPath());
}

bool FileSystemService::isFolderAvailable(string name) { return Storage::getInstance()->validateFolder(name); }

string FileSystemService::currentPath() { return Storage::getInstance()->getPath(folderService->getCurrentFolder()); }

// History operations
void FileSystemService::showHistory() const
{
    historyService->showHistory();
}

void FileSystemService::showHistory(int count) const
{
    historyService->showHistory(count);
}

void FileSystemService::clearHistory()
{
    historyService->clearHistory();
}

int FileSystemService::getHistoryCount() const
{
    return historyService->getHistoryCount();
}

// Grep operations
void FileSystemService::grepPattern(const string& pattern)
{
    grepService->grep(pattern);
    historyService->addEntry("grep " + pattern, "GREP", pattern, currentPath());
}

void FileSystemService::grepInFile(const string& pattern, const string& fileName)
{
    grepService->grepInFile(pattern, fileName);
    historyService->addEntry("grep " + pattern + " " + fileName, "GREP_FILE", fileName, currentPath());
}

void FileSystemService::grepRecursive(const string& pattern)
{
    GrepOptions options;
    options.recursive = true;
    grepService->grep(pattern, options);
    historyService->addEntry("grep -r " + pattern, "GREP_RECURSIVE", pattern, currentPath());
}

void FileSystemService::grepWithOptions(const string& pattern, const string& options)
{
    GrepOptions grepOpts;
    
    // Parse options
    for (char c : options) {
        switch (c) {
            case 'i': grepOpts.caseInsensitive = true; break;
            case 'r': grepOpts.recursive = true; break;
            case 'c': grepOpts.countOnly = true; break;
            case 'v': grepOpts.invertMatch = true; break;
            case 'n': grepOpts.showLineNumbers = true; break;
        }
    }
    
    grepService->grep(pattern, grepOpts);
    historyService->addEntry("grep -" + options + " " + pattern, "GREP_OPTIONS", pattern, currentPath());
}

void FileSystemService::showGrepHelp()
{
    grepService->showGrepHelp();
    historyService->addEntry("grep --help", "GREP_HELP", "", currentPath());
}

FileSystemService::FileSystemService()
{
    folderService = new FolderService();
    fileService = new FileService();
    historyService = new HistoryService();
    grepService = new GrepService();
}