// src/services/FileSystemService.cpp

#include "../../include/services/FileSystemService.h"
#include "../../include/storage/Storage.h"
#include "../../include/services/FileService.h"
#include "../../include/services/FolderService.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>

using namespace std;

void FileSystemService::createFile(string folderId, string fileName) { fileService->createFile(folderId, fileName); }

string FileSystemService::getCurrentFolder() { return folderService->getCurrentFolder(); }

void FileSystemService::addContent(string fileId, string content) { fileService->addContent(fileId, content); }

void FileSystemService::removeFile(string fileName) { fileService->removeFile(fileName); }

string FileSystemService::showFileContent(string fileId) { return fileService->showFileContent(fileId); }

void FileSystemService::createFolder(string parentFolderId, string folderName) { folderService->createFolder(parentFolderId, folderName); }

void FileSystemService::removeFolder(string folderName) { folderService->removeFolder(folderName); }

void FileSystemService::showTree(string folderId) { folderService->showTree(folderService->getCurrentFolder()); }

void FileSystemService::listAllItems(string folderId) { folderService->listAllItems(folderId); }

void FileSystemService::getIntoFolder(string folderName) { folderService->getIntoFolder(folderName); }

bool FileSystemService::isFolderAvailable(string name) { return Storage::getInstance()->validateFolder(name); }

string FileSystemService::currentPath() { return Storage::getInstance()->getPath(folderService->getCurrentFolder()); }

FileSystemService::FileSystemService()
{
    folderService = new FolderService();
    fileService = new FileService();
}