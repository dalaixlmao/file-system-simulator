// include/models/File.h

#ifndef FILE_H
#define FILE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

class File
{
private:
    string id;
    string name;
    string content;
    string extension;
    string folderId;

public:
    File(string id, string name, string folderId);
    void setContent(string content);
    string getContent();
    string getFileName();
    string getFolderId();
    string getId();
    ~File() = default;
};

#endif