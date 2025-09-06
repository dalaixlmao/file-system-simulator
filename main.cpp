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
    cout << "     pwd" << endl;
    cout << "     ls" << endl;
    cout << "     touch <File Name>" << endl;
    cout << "     write <File Name> <Content>" << endl;
    cout << "     rm <File Name>" << endl;
    cout << "     tree" << endl;
    cout << "     history [number]" << endl;
    cout << "     history clear" << endl;
    cout << "     grep <pattern> [filename]" << endl;
    cout << "     grep -[options] <pattern>" << endl;
    cout << "     grep --help" << endl;
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
        else if (command == "pwd")
        {
            cout << fileSystem->currentPath() << endl;
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
        else if (command == "history")
        {
            string arg;
            if (cin.peek() != '\n')
            {
                cin >> arg;
                if (arg == "clear")
                {
                    fileSystem->clearHistory();
                }
                else
                {
                    try
                    {
                        int count = stoi(arg);
                        fileSystem->showHistory(count);
                    }
                    catch (...)
                    {
                        cout << "Invalid number format. Usage: history [number] or history clear" << endl;
                    }
                }
            }
            else
            {
                fileSystem->showHistory();
            }
        }
        else if (command == "grep")
        {
            string arg;
            if (cin.peek() != '\n')
            {
                cin >> arg;
                if (arg == "--help")
                {
                    fileSystem->showGrepHelp();
                }
                else if (arg.length() > 1 && arg[0] == '-' && arg[1] != '-')
                {
                    // Options provided (e.g., -ir, -c)
                    string options = arg.substr(1);
                    string pattern;
                    cin >> pattern;
                    fileSystem->grepWithOptions(pattern, options);
                }
                else
                {
                    // Pattern provided, check if filename follows
                    string pattern = arg;
                    string fileName;
                    if (cin.peek() != '\n')
                    {
                        cin >> fileName;
                        fileSystem->grepInFile(pattern, fileName);
                    }
                    else
                    {
                        fileSystem->grepPattern(pattern);
                    }
                }
            }
            else
            {
                cout << "Usage: grep <pattern> [filename] or grep --help" << endl;
            }
        }
        else
        {
            cout << "Wrong command!" << endl;
        }
        cout << endl;
    }

    return 0;
}