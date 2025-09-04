// src/storage/storage.cpp

#include "../../include/storage/Storage.h"
#include "../../include/models/File.h"
#include "../../include/models/FileSystem.h"
#include "../../include/models/Folder.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

Storage *Storage::instance = nullptr;

Storage *Storage::getInstance()
{
    if (instance == nullptr)
    {
        return instance = new Storage();
    }
    return instance;
}

Storage::Storage()
{
    fileSystem = new FileSystem();
    fileSystem->addFolderId("F0");
    folders["F0"] = nullptr;
    map<string, int> temp;
    tree["F0"] = temp;
    Folder *f = new Folder("F" + to_string(folders.size()), "BaseFolder", "FX");
    fileSystem->addFolderId("F0");
    fileSystem->addFolderId("F1");
    folders[f->getId()] = f;
}

void Storage::addContent(string fileName, string content)
{
    string currentFolderId = getCurrentFolderId();
    if (validateFile(fileName))
    {
        for (auto i : tree[currentFolderId])
        {
            if (i.first[0] == 'f' && files[i.first]->getFileName() == fileName)
            {
                files[i.first]->setContent(content);
            }
        }
    }
}

string Storage::getNewFileId() { return "f" + to_string(files.size()); }

void Storage::addFile(string name, string folderId)
{
    for (auto i : tree[folderId])
    {
        if (i.first[0] == 'f' && files[i.first]->getFileName() == name)
        {
            cout << "     " << "File name already exist! change the name of the file." << endl;
            return;
        }
    }
    string newFileId = getNewFileId();
    File *f = new File(newFileId, name, folderId);
    files[newFileId] = f;
    tree[folderId][newFileId] = 1;
    cout << "     " << "File created! File name = " + name + ", id =" + f->getId() + ", in folder id - " << folderId << endl;
}

string Storage::getNewFolderId() { return "F" + to_string(folders.size()); }

void Storage::addFolder(string name, string parentFolderId)
{
    for (auto i : tree[parentFolderId])
    {
        if (i.first[0] == 'F' && folders[i.first]->getName() == name)
        {
            cout << "     " << "Folder name already exist! change the name of the folder." << endl;
            return;
        }
    }
    string newFolderId = getNewFolderId();
    Folder *f = new Folder(newFolderId, name, parentFolderId);
    folders[newFolderId] = f;
    tree[parentFolderId][newFolderId] = 1;
    cout << "     " << "New folder created! Name = " << name << " id = " << f->getId() << endl;
}

Folder *Storage::getFolder(string id)
{
    return folders[id];
}

File *Storage::getFile(string id)
{
    return files[id];
}

string Storage::getPath(string id)
{
    Folder *f = folders[id];
    string path = "";
    while (f && f->getParentId() != "F0")
    {
        path = "/" + path;
        path = f->getName() + path;
        f = folders[f->getParentId()];
    }
    return path;
}

void Storage::showFolderPath(string id)
{
    string path = getPath(id);
    cout << path << endl;
}

void Storage::showFilePath(string id)
{
    string path = getPath(id);
    cout << path << endl;
}

string Storage::getCurrentFolderId() { return fileSystem->getCurrentFolder(); }

void Storage::showItemsInFolder(string folderId)
{
    if (folders.count(folderId) && folders[folderId])
    {
        for (auto i : tree[folderId])
        {
            if (i.first[0] == 'f')
                cout << "     " << files[i.first]->getFileName() << endl;
            else
                cout << "     " << folders[i.first]->getName() << endl;
        }
    }
    else
        cout << "     " << "Folder does not exist." << endl;
}

void Storage::getIntoFolder(string name)
{
    string currentFolderId = fileSystem->getCurrentFolder();
    if (name != "..")
    {
        for (auto i : tree[currentFolderId])
        {
            if (i.first[0] == 'F' && folders[i.first]->getName() == name)
            {
                fileSystem->addFolderId(i.first);
                return;
            }
        }
    }
    else if (!fileSystem->checkEmpty())
    {
        fileSystem->removeCurrentFolder();
        return;
    }
    cout << "     " << "Wrong file name, no file exists with name " << name << endl;
}

bool Storage::validateFolder(string folderName)
{
    string currentFolderId = fileSystem->getCurrentFolder();
    for (auto i : tree[currentFolderId])
    {
        if (i.first[0] == 'F' && folders[i.first]->getName() == folderName)
        {
            return true;
        }
    }
    return false;
}

void Storage::removeFile(string fileName)
{
    if (validateFile(fileName))
    {
        string currentFolderId = fileSystem->getCurrentFolder();
        for (auto i : tree[currentFolderId])
        {
            if (i.first[0] == 'f' && files[i.first]->getFileName() == fileName)
            {
                string fileId = files[i.first]->getId();
                files.erase(fileId);
                tree[currentFolderId].erase(fileId);
                if (tree[currentFolderId].size() == 0)
                    tree.erase(currentFolderId);
                cout << "File removed successfully!" << endl;
                return;
            }
        }
    }
}

void Storage::removeDFS(string node)
{
    for (auto i : tree[node])
    {
        if (i.first[0] == 'F')
        {
            removeDFS(i.first);
        }
        else
        {
            cout << "     " << "File id - " << files[i.first]->getId() << " and name - " << files[i.first]->getFileName() << " removed successfully!" << endl;
            files[i.first] = nullptr;
        }
    }
    cout << "     " << "Folder id - " << folders[node]->getId() << " and name - " << folders[node]->getName() << " removed successfully!" << endl;
    folders[node] = nullptr;
    tree.erase(node);
}

void Storage::removeFolder(string folderName)
{
    if (validateFolder(folderName))
    {
        string currentFolderId = fileSystem->getCurrentFolder();
        for (auto i : tree[currentFolderId])
        {
            if (i.first[0] == 'F' && folders[i.first]->getName() == folderName)
            {
                string folderId = folders[i.first]->getId();
                string parFolderId = folders[i.first]->getParentId();
                tree[parFolderId].erase(folderId);
                removeDFS(folderId);
                cout << "     Folder removed successfully!" << endl;
                return;
            }
        }
    }
}

void Storage::showDFS(string node, string symbols)
{
    cout << "     " << symbols + "- " << ((node[0] == 'F') ? folders[node]->getName() : files[node]->getFileName()) << endl;

    symbols += "  |";
    for (auto i : tree[node])
    {
        showDFS(i.first, symbols);
    }
}

void Storage::showFolderTree()
{
    string currentFolderId = fileSystem->getCurrentFolder();
    string symbols = "";
    showDFS(currentFolderId, symbols);
}

bool Storage::validateFile(string fileName)
{
    string currentFolderId = fileSystem->getCurrentFolder();
    for (auto i : tree[currentFolderId])
    {
        if (i.first[0] == 'f' && files[i.first]->getFileName() == fileName)
        {
            return true;
        }
    }
    return false;
}

// Grep support methods
vector<string> Storage::getFileIdsInFolder(string folderId)
{
    vector<string> fileIds;
    if (tree.find(folderId) != tree.end())
    {
        for (auto i : tree[folderId])
        {
            if (i.first[0] == 'f')
            {
                fileIds.push_back(i.first);
            }
        }
    }
    return fileIds;
}

vector<string> Storage::getFolderIdsInFolder(string folderId)
{
    vector<string> folderIds;
    if (tree.find(folderId) != tree.end())
    {
        for (auto i : tree[folderId])
        {
            if (i.first[0] == 'F')
            {
                folderIds.push_back(i.first);
            }
        }
    }
    return folderIds;
}

string Storage::getFileIdByName(string fileName, string folderId)
{
    if (tree.find(folderId) != tree.end())
    {
        for (auto i : tree[folderId])
        {
            if (i.first[0] == 'f' && files[i.first] && files[i.first]->getFileName() == fileName)
            {
                return i.first;
            }
        }
    }
    return "";
}

map<string, File*> Storage::getAllFiles()
{
    return files;
}

map<string, Folder*> Storage::getAllFolders()
{
    return folders;
}
