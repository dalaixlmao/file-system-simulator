// src/models/Folder.cpp

#include "../../include/models/Folder.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>
using namespace std;

Folder::Folder(string id, string name, string folderId) : id(id), name(name), folderId(folderId) {}

string Folder::getParentId() { return folderId; }

string Folder::getName() { return name; }

string Folder::getId() { return id; }
