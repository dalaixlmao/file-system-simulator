// include/services/HistoryService.h

#ifndef HISTORYSERVICE_H
#define HISTORYSERVICE_H

#include <vector>
#include <string>
#include "../models/History.h"

using namespace std;

class HistoryService
{
private:
    vector<History*> historyEntries;
    static const int MAX_HISTORY_SIZE = 1000;

public:
    HistoryService();
    
    // Core history operations
    void addEntry(string command, string operationType, string target, string currentPath);
    void showHistory() const;
    void showHistory(int count) const;
    void clearHistory();
    
    // Utility methods
    int getHistoryCount() const;
    History* getLastEntry() const;
    vector<History*> getRecentEntries(int count) const;
    
    ~HistoryService();
};

#endif
