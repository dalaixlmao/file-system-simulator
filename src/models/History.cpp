// src/models/History.cpp

#include "../../include/models/History.h"
#include <sstream>
#include <iomanip>

using namespace std;

int History::nextId = 1;

History::History(string command, string operationType, string target, string currentPath)
    : id(nextId++), command(command), operationType(operationType), target(target), 
      currentPath(currentPath), timestamp(time(nullptr))
{
}

int History::getId() const
{
    return id;
}

string History::getCommand() const
{
    return command;
}

string History::getOperationType() const
{
    return operationType;
}

string History::getTarget() const
{
    return target;
}

string History::getCurrentPath() const
{
    return currentPath;
}

time_t History::getTimestamp() const
{
    return timestamp;
}

string History::getFormattedTimestamp() const
{
    char buffer[80];
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

string History::getFormattedEntry() const
{
    stringstream ss;
    ss << setw(4) << right << id << "  ";
    ss << getFormattedTimestamp() << "  ";
    ss << setw(12) << left << operationType << "  ";
    ss << setw(20) << left << target << "  ";
    ss << currentPath << "  ";
    ss << command;
    return ss.str();
}
