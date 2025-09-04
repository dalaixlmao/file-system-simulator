// src/services/GrepService.cpp

#include "../../include/services/GrepService.h"
#include "../../include/storage/Storage.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <cctype>

using namespace std;

GrepService::GrepService() {
    store = Storage::getInstance();
}

vector<string> GrepService::splitLines(const string& content) {
    vector<string> lines;
    istringstream stream(content);
    string line;
    
    while (getline(stream, line)) {
        lines.push_back(line);
    }
    
    // If content doesn't end with newline, the last line might be missed
    if (!content.empty() && content.back() != '\n' && lines.empty()) {
        lines.push_back(content);
    }
    
    return lines;
}

bool GrepService::matchesPattern(const string& line, const string& pattern, bool caseInsensitive, bool invertMatch) {
    string searchLine = line;
    string searchPattern = pattern;
    
    if (caseInsensitive) {
        transform(searchLine.begin(), searchLine.end(), searchLine.begin(), ::tolower);
        transform(searchPattern.begin(), searchPattern.end(), searchPattern.begin(), ::tolower);
    }
    
    bool found = false;
    
    try {
        // Try regex matching first
        regex regexPattern(searchPattern, caseInsensitive ? regex_constants::icase : regex_constants::ECMAScript);
        found = regex_search(line, regexPattern);
    } catch (const regex_error&) {
        // If regex fails, fall back to simple string matching
        found = searchLine.find(searchPattern) != string::npos;
    }
    
    return invertMatch ? !found : found;
}

void GrepService::searchInFile(const string& fileId, const string& pattern, const GrepOptions& options, vector<GrepResult>& results) {
    File* file = store->getFile(fileId);
    if (!file) return;
    
    string content = file->getContent();
    if (content.empty()) return;
    
    vector<string> lines = splitLines(content);
    
    for (size_t i = 0; i < lines.size(); i++) {
        if (matchesPattern(lines[i], pattern, options.caseInsensitive, options.invertMatch)) {
            GrepResult result;
            result.fileName = file->getFileName();
            result.filePath = store->getPath(file->getFolderId()) + "/" + file->getFileName();
            result.lineNumber = i + 1;
            result.matchedLine = lines[i];
            result.fileId = fileId;
            results.push_back(result);
        }
    }
}

void GrepService::searchInFolder(const string& folderId, const string& pattern, const GrepOptions& options, vector<GrepResult>& results) {
    // Get all files in the current folder
    vector<string> fileIds = store->getFileIdsInFolder(folderId);
    
    // Search in each file
    for (const string& fileId : fileIds) {
        searchInFile(fileId, pattern, options, results);
    }
    
    // If recursive search is enabled, search in subfolders
    if (options.recursive) {
        vector<string> folderIds = store->getFolderIdsInFolder(folderId);
        for (const string& subFolderId : folderIds) {
            searchInFolder(subFolderId, pattern, options, results);
        }
    }
}

void GrepService::displayResults(const vector<GrepResult>& results, const GrepOptions& options) {
    if (results.empty()) {
        cout << "     No matches found." << endl;
        return;
    }
    
    if (options.countOnly) {
        cout << "     Total matches: " << results.size() << endl;
        return;
    }
    
    string currentFile = "";
    for (const auto& result : results) {
        if (options.showFilePath && result.fileName != currentFile) {
            if (!currentFile.empty()) cout << endl;
            cout << "     === " << result.filePath << " ===" << endl;
            currentFile = result.fileName;
        }
        
        cout << "     ";
        if (options.showLineNumbers) {
            cout << result.lineNumber << ": ";
        }
        cout << result.matchedLine << endl;
    }
}

void GrepService::grep(const string& pattern, const GrepOptions& options) {
    vector<GrepResult> results;
    
    if (!options.targetFile.empty()) {
        // Search in specific file
        grepInFile(pattern, options.targetFile, options);
        return;
    }
    
    // Search in current directory
    string currentFolderId = store->getCurrentFolderId();
    cout << "     Searching for pattern: \"" << pattern << "\" in current directory..." << endl;
    
    searchInFolder(currentFolderId, pattern, options, results);
    displayResults(results, options);
}

void GrepService::grepInFile(const string& pattern, const string& fileName, const GrepOptions& options) {
    vector<GrepResult> results;
    
    // Find the file in current directory
    string currentFolderId = store->getCurrentFolderId();
    string fileId = store->getFileIdByName(fileName, currentFolderId);
    
    if (!fileId.empty()) {
        cout << "     Searching for pattern: \"" << pattern << "\" in file: " << fileName << endl;
        searchInFile(fileId, pattern, options, results);
        displayResults(results, options);
    } else {
        cout << "     File not found: " << fileName << endl;
    }
}

void GrepService::grepRecursive(const string& pattern, const GrepOptions& options) {
    GrepOptions recursiveOptions = options;
    recursiveOptions.recursive = true;
    grep(pattern, recursiveOptions);
}

void GrepService::showGrepHelp() {
    cout << "     GREP - Search for patterns in files" << endl;
    cout << "     Usage:" << endl;
    cout << "       grep <pattern>                    - Search pattern in current directory" << endl;
    cout << "       grep <pattern> <filename>         - Search pattern in specific file" << endl;
    cout << "       grep -i <pattern>                 - Case-insensitive search" << endl;
    cout << "       grep -r <pattern>                 - Recursive search in subdirectories" << endl;
    cout << "       grep -c <pattern>                 - Count matches only" << endl;
    cout << "       grep -v <pattern>                 - Invert match (show non-matching lines)" << endl;
    cout << "       grep -n <pattern>                 - Show line numbers (default)" << endl;
    cout << "       grep --help                       - Show this help" << endl;
    cout << endl;
    cout << "     Options can be combined: grep -ir <pattern>" << endl;
    cout << "     Pattern supports basic regex syntax" << endl;
}
