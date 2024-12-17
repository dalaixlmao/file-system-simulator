// include/models/FileSystem.h

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>

using namespace std;

class FileSystem
{
private:
    stack<string> path;

public:
    FileSystem();
    string getCurrentFolder();
    void addFolderId(string id);
    void removeCurrentFolder();
    bool checkEmpty();
    ~FileSystem() = default;
};

#endif