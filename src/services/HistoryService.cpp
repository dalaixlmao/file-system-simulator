// src/services/HistoryService.cpp

#include "../../include/services/HistoryService.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

HistoryService::HistoryService()
{
}

void HistoryService::addEntry(string command, string operationType, string target, string currentPath)
{
    History* newEntry = new History(command, operationType, target, currentPath);
    historyEntries.push_back(newEntry);
    
    // Maintain maximum history size
    if (historyEntries.size() > MAX_HISTORY_SIZE)
    {
        delete historyEntries.front();
        historyEntries.erase(historyEntries.begin());
    }
}

void HistoryService::showHistory() const
{
    if (historyEntries.empty())
    {
        cout << "No history available." << endl;
        return;
    }
    
    cout << endl;
    cout << "Command History:" << endl;
    cout << "----------------" << endl;
    cout << setw(4) << right << "ID" << "  ";
    cout << setw(19) << left << "Timestamp" << "  ";
    cout << setw(12) << left << "Operation" << "  ";
    cout << setw(20) << left << "Target" << "  ";
    cout << setw(15) << left << "Path" << "  ";
    cout << "Command" << endl;
    cout << string(90, '-') << endl;
    
    for (const auto& entry : historyEntries)
    {
        cout << entry->getFormattedEntry() << endl;
    }
    cout << endl;
}

void HistoryService::showHistory(int count) const
{
    if (historyEntries.empty())
    {
        cout << "No history available." << endl;
        return;
    }
    
    if (count <= 0)
    {
        cout << "Invalid count. Please specify a positive number." << endl;
        return;
    }
    
    cout << endl;
    cout << "Recent Command History (last " << count << " commands):" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << setw(4) << right << "ID" << "  ";
    cout << setw(19) << left << "Timestamp" << "  ";
    cout << setw(12) << left << "Operation" << "  ";
    cout << setw(20) << left << "Target" << "  ";
    cout << setw(15) << left << "Path" << "  ";
    cout << "Command" << endl;
    cout << string(90, '-') << endl;
    
    int start = max(0, static_cast<int>(historyEntries.size()) - count);
    for (int i = start; i < static_cast<int>(historyEntries.size()); i++)
    {
        cout << historyEntries[i]->getFormattedEntry() << endl;
    }
    cout << endl;
}

void HistoryService::clearHistory()
{
    for (auto& entry : historyEntries)
    {
        delete entry;
    }
    historyEntries.clear();
    cout << "History cleared successfully." << endl;
}

int HistoryService::getHistoryCount() const
{
    return historyEntries.size();
}

History* HistoryService::getLastEntry() const
{
    if (historyEntries.empty())
    {
        return nullptr;
    }
    return historyEntries.back();
}

vector<History*> HistoryService::getRecentEntries(int count) const
{
    vector<History*> recent;
    if (count <= 0 || historyEntries.empty())
    {
        return recent;
    }
    
    int start = max(0, static_cast<int>(historyEntries.size()) - count);
    for (int i = start; i < static_cast<int>(historyEntries.size()); i++)
    {
        recent.push_back(historyEntries[i]);
    }
    
    return recent;
}

HistoryService::~HistoryService()
{
    for (auto& entry : historyEntries)
    {
        delete entry;
    }
    historyEntries.clear();
}
