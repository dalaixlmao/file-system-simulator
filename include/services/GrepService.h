// include/services/GrepService.h

#ifndef GREPSERVICE_H
#define GREPSERVICE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <regex>
#include "../storage/Storage.h"

using namespace std;

struct GrepResult {
    string fileName;
    string filePath;
    int lineNumber;
    string matchedLine;
    string fileId;
};

struct GrepOptions {
    bool caseInsensitive = false;
    bool recursive = false;
    bool showLineNumbers = true;
    bool showFilePath = true;
    bool countOnly = false;
    bool invertMatch = false;
    string targetFile = "";
    string targetFolder = "";
};

class GrepService
{
private:
    Storage *store;
    vector<string> splitLines(const string& content);
    bool matchesPattern(const string& line, const string& pattern, bool caseInsensitive, bool invertMatch);
    void searchInFile(const string& fileId, const string& pattern, const GrepOptions& options, vector<GrepResult>& results);
    void searchInFolder(const string& folderId, const string& pattern, const GrepOptions& options, vector<GrepResult>& results);
    void displayResults(const vector<GrepResult>& results, const GrepOptions& options);

public:
    GrepService();
    void grep(const string& pattern, const GrepOptions& options = GrepOptions());
    void grepInFile(const string& pattern, const string& fileName, const GrepOptions& options = GrepOptions());
    void grepRecursive(const string& pattern, const GrepOptions& options = GrepOptions());
    void showGrepHelp();
    ~GrepService() = default;
};

#endif
