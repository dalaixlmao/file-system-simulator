// src/models/File.cpp

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "../../include/models/File.h"
using namespace std;

File::File(string id, string fileName, string folderId) : id(id), folderId(folderId)
{
    string n;
    int ind;
    for (int i = 0; i < fileName.size(); i++)
        if (fileName[i] == '.')
        {
            ind = i;
            break;
        }
    n = fileName.substr(0, ind);
    string ex = fileName.substr(ind + 1, n.size() - ind - 1);
    name = n;
    extension = ex;
}

void File::setContent(string content) { this->content = content; }

string File::getContent() { return content; }

string File::getId() { return id; }

string File::getFileName() { return name + "." + extension; }

string File::getFolderId() { return folderId; }
