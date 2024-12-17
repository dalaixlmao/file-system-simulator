// src/services/FileService.cpp

#include "../../include/services/FileService.h"
#include "../../include/storage/Storage.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>
using namespace std;

void FileService::createFile(string folderId, string fileName) { Storage::getInstance()->addFile(fileName, folderId); }

void FileService::addContent(string fileName, string content) { Storage::getInstance()->addContent(fileName, content); }

void FileService::removeFile(string filename) { Storage::getInstance()->removeFile(filename); }

string FileService::showFileContent(string fileId) { return Storage::getInstance()->getFile(fileId)->getContent(); }

void FileService::showFilePath(string fileId) { return Storage::getInstance()->showFilePath(fileId); }

FileService::FileService() {}
