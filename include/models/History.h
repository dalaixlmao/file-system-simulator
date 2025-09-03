// include/models/History.h

#ifndef HISTORY_H
#define HISTORY_H

#include <string>
#include <ctime>

using namespace std;

class History
{
private:
    static int nextId;
    int id;
    string command;
    string operationType;
    string target;
    string currentPath;
    time_t timestamp;

public:
    History(string command, string operationType, string target, string currentPath);
    
    // Getters
    int getId() const;
    string getCommand() const;
    string getOperationType() const;
    string getTarget() const;
    string getCurrentPath() const;
    time_t getTimestamp() const;
    string getFormattedTimestamp() const;
    string getFormattedEntry() const;
    
    ~History() = default;
};

#endif
