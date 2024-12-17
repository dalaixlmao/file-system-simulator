// include/models/Folder.h

#ifndef FOLDER_H
#define FOLDER_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

class Folder
{
private:
    string id;
    string name;
    string folderId;

public:
    Folder(string id, string name, string folderId);
    string getParentId();
    string getName();
    string getId();
    ~Folder() = default;
};

#endif