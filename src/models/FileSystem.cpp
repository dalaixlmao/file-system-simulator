// src/models/FileSystem.cpp

#include "../../include/models/FileSystem.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>
using namespace std;

FileSystem::FileSystem() {}

void FileSystem::addFolderId(string id) { path.push(id); }

void FileSystem::removeCurrentFolder()
{
    if (!path.empty())
        path.pop();
}

string FileSystem::getCurrentFolder()
{
    if (!path.empty())
        return path.top();
    return ""; // Return empty string if path is empty
}

bool FileSystem::checkEmpty() { return path.empty(); }
