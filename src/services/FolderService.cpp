// src/services/FolderService.cpp

#include "../../include/services/FolderService.h"
#include "../../include/storage/Storage.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>
using namespace std;

void FolderService::createFolder(string parentFolderId, string folderName) { Storage::getInstance()->addFolder(folderName, parentFolderId); }

void FolderService::removeFolder(string folderName) { Storage::getInstance()->removeFolder(folderName); }

void FolderService::showTree(string folderId) { Storage::getInstance()->showFolderTree(); }

void FolderService::listAllItems(string folderId) { Storage::getInstance()->showItemsInFolder(folderId); }

void FolderService::showFolderPath(string folderId) { Storage::getInstance()->showFolderPath(folderId); }

string FolderService::getCurrentFolder() { return Storage::getInstance()->getCurrentFolderId(); }

FolderService::FolderService() {}

void FolderService::getIntoFolder(string folderName) { Storage::getInstance()->getIntoFolder(folderName); }