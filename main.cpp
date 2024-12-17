// main.cpp

#include "./include/services/FileSystemService.h"
#include <string>

using namespace std;

int main()
{

    FileSystemService *fileSystem = new FileSystemService();
    cout << "     Available commands are: " << endl;
    cout << "     mkdir <Folder Name>" << endl;
    cout << "     rmdir <Folder Name>" << endl;
    cout << "     cd <Change Current Directory>" << endl;
    cout << "     ls" << endl;
    cout << "     touch <File Name>" << endl;
    cout << "     write <File Name> <Content>" << endl;
    cout << "     rm <File Name>" << endl;
    cout << "     tree" << endl;
    while (true)
    {
        string currentPath = fileSystem->currentPath();
        cout << currentPath << ">  ";
        string command;
        cin >> command;
        cout << endl;
        if (command == "mkdir")
        {
            string folderName;
            cin >> folderName;
            fileSystem->createFolder(fileSystem->getCurrentFolder(), folderName);
        }
        else if (command == "rmdir")
        {
            string folderName;
            cin >> folderName;
            fileSystem->removeFolder(folderName);
        }
        else if (command == "cd")
        {
            string folderName;
            cin >> folderName;
            fileSystem->getIntoFolder(folderName);
        }
        else if (command == "ls")
        {
            fileSystem->listAllItems(fileSystem->getCurrentFolder());
        }
        else if (command == "touch")
        {
            string fileName;
            cin >> fileName;
            fileSystem->createFile(fileSystem->getCurrentFolder(), fileName);
        }
        else if (command == "write")
        {
            string fileName;
            cin >> fileName;
            string content;
            getline(cin, content);
            fileSystem->addContent(fileName, content);
        }
        else if (command == "rm")
        {
            string fileName;
            cin >> fileName;
            fileSystem->removeFile(fileName);
        }
        else if (command == "tree")
        {
            fileSystem->showTree(fileSystem->getCurrentFolder());
        }
        else
        {
            cout << "Wrong command!" << endl;
        }
        cout << endl;
    }

    return 0;
}