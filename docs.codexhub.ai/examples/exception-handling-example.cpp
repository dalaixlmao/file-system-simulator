// exception-handling-example.cpp
// Demonstration of proper exception handling for the File System Simulator

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <sstream>

// ========== CUSTOM EXCEPTION HIERARCHY ==========

// Base exception for all file system errors
class FileSystemException : public std::runtime_error {
private:
    // Timestamp when the exception occurred
    std::string timestamp;
    
public:
    explicit FileSystemException(const std::string& message) 
        : std::runtime_error(message) 
    {
        // Generate timestamp
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        timestamp = ss.str();
    }
    
    // Get the timestamp when the exception occurred
    std::string getTimestamp() const { return timestamp; }
};

// File not found exception
class FileNotFoundException : public FileSystemException {
private:
    std::string filename;
    
public:
    explicit FileNotFoundException(const std::string& filename) 
        : FileSystemException("File not found: " + filename),
          filename(filename) {}
    
    const std::string& getFilename() const { return filename; }
};

// File already exists exception
class FileAlreadyExistsException : public FileSystemException {
private:
    std::string filename;
    
public:
    explicit FileAlreadyExistsException(const std::string& filename) 
        : FileSystemException("File already exists: " + filename),
          filename(filename) {}
    
    const std::string& getFilename() const { return filename; }
};

// Folder not found exception
class FolderNotFoundException : public FileSystemException {
private:
    std::string foldername;
    
public:
    explicit FolderNotFoundException(const std::string& foldername) 
        : FileSystemException("Folder not found: " + foldername),
          foldername(foldername) {}
    
    const std::string& getFoldername() const { return foldername; }
};

// Invalid operation exception
class InvalidOperationException : public FileSystemException {
private:
    std::string operation;
    
public:
    InvalidOperationException(const std::string& operation, const std::string& reason) 
        : FileSystemException("Invalid operation '" + operation + "': " + reason),
          operation(operation) {}
    
    const std::string& getOperation() const { return operation; }
};

// Permission denied exception
class PermissionDeniedException : public FileSystemException {
private:
    std::string entity;
    std::string action;
    
public:
    PermissionDeniedException(const std::string& entity, const std::string& action) 
        : FileSystemException("Permission denied: Cannot " + action + " " + entity),
          entity(entity), action(action) {}
    
    const std::string& getEntity() const { return entity; }
    const std::string& getAction() const { return action; }
};

// ========== FILE SYSTEM MODELS WITH EXCEPTION HANDLING ==========

// Simple models with exception handling
class File {
private:
    std::string id;
    std::string name;
    std::string extension;
    std::string content;
    std::string folderId;

public:
    File(const std::string& id, const std::string& fileName, const std::string& folderId) 
        : id(id), folderId(folderId) 
    {
        // Parse filename with proper validation
        size_t dotPos = fileName.find_last_of('.');
        
        if (dotPos != std::string::npos) {
            name = fileName.substr(0, dotPos);
            extension = fileName.substr(dotPos + 1);
            
            // Validate name part
            if (name.empty()) {
                throw InvalidOperationException("CreateFile", "Filename must have non-empty name part");
            }
        } else {
            name = fileName;
            extension = "";
        }
        
        // Validate filename
        if (fileName.empty()) {
            throw InvalidOperationException("CreateFile", "Filename cannot be empty");
        }
        
        // Check for invalid characters (simplified)
        const std::string invalidChars = "\\/:*?\"<>|";
        for (char c : invalidChars) {
            if (fileName.find(c) != std::string::npos) {
                throw InvalidOperationException("CreateFile", 
                    "Filename contains invalid character: " + std::string(1, c));
            }
        }
    }

    void setContent(const std::string& newContent) { 
        content = newContent; 
    }

    std::string getContent() const { 
        return content; 
    }

    std::string getId() const { 
        return id; 
    }

    std::string getFileName() const { 
        return name + (extension.empty() ? "" : "." + extension); 
    }

    std::string getFolderId() const { 
        return folderId; 
    }
    
    void setFolderId(const std::string& newFolderId) {
        folderId = newFolderId;
    }
};

// ========== STORAGE CLASS WITH EXCEPTION HANDLING ==========

// Forward declaration
class FileSystem;

class Storage {
private:
    // Storage implementation with exception handling
    Storage() {
        try {
            // Initialize file system
            fileSystem = std::make_unique<FileSystem>();
            
            // Create root folder
            folders["F0"] = nullptr;  // Root sentinel
            
            // Create base folder
            auto baseFolder = std::make_unique<File>("F1", "BaseFolder", "F0");
            folders["F1"] = std::move(baseFolder);
            
            // Initialize tree structure
            tree["F0"]["F1"] = 1;
        } catch (const std::exception& e) {
            std::cerr << "Failed to initialize Storage: " << e.what() << std::endl;
            throw;  // Re-throw to notify caller
        }
    }
    
    // Deleted copy/move operations
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;
    
    // Member variables
    std::unique_ptr<FileSystem> fileSystem;
    std::map<std::string, std::unique_ptr<File>> folders;
    std::map<std::string, std::unique_ptr<File>> files;
    std::map<std::string, std::map<std::string, int>> tree;

public:
    // Singleton instance getter (thread-safe in C++11)
    static Storage& getInstance() {
        static Storage instance;
        return instance;
    }
    
    // Add file with exception handling
    void addFile(const std::string& name, const std::string& folderId) {
        // Check if folder exists
        if (folders.find(folderId) == folders.end()) {
            throw FolderNotFoundException(folderId);
        }
        
        // Check if file already exists
        for (const auto& pair : tree[folderId]) {
            const auto& key = pair.first;
            if (key[0] == 'f') {
                if (files.find(key) != files.end() && 
                    files[key]->getFileName() == name) {
                    throw FileAlreadyExistsException(name);
                }
            }
        }
        
        try {
            // Generate unique ID
            std::string newFileId = "f" + std::to_string(files.size());
            
            // Create file object (may throw for invalid filenames)
            auto newFile = std::make_unique<File>(newFileId, name, folderId);
            
            // Store file (using strong exception guarantee pattern)
            files[newFileId] = std::move(newFile);
            tree[folderId][newFileId] = 1;
            
            std::cout << "File created successfully: " << name << std::endl;
        } catch (const InvalidOperationException& e) {
            // Re-throw with additional context
            throw;
        } catch (const std::exception& e) {
            // Handle unexpected exceptions
            throw FileSystemException("Failed to create file: " + 
                std::string(e.what()));
        }
    }
    
    // Remove file with exception handling
    void removeFile(const std::string& fileName, const std::string& folderId) {
        // Check if folder exists
        if (folders.find(folderId) == folders.end()) {
            throw FolderNotFoundException(folderId);
        }
        
        // Find the file
        std::string fileId;
        bool found = false;
        
        for (const auto& pair : tree[folderId]) {
            const auto& key = pair.first;
            if (key[0] == 'f' && files[key]->getFileName() == fileName) {
                fileId = key;
                found = true;
                break;
            }
        }
        
        if (!found) {
            throw FileNotFoundException(fileName);
        }
        
        // Strong exception guarantee with copy-and-swap idiom
        auto treeCopy = tree;
        
        // Make changes to copy
        treeCopy[folderId].erase(fileId);
        
        // If the folder becomes empty, consider special handling
        if (treeCopy[folderId].empty()) {
            treeCopy.erase(folderId);
        }
        
        // Apply changes (non-throwing operations)
        tree = std::move(treeCopy);
        files.erase(fileId);
        
        std::cout << "File removed successfully: " << fileName << std::endl;
    }
    
    // Move file between folders with exception handling and strong guarantee
    void moveFile(const std::string& fileId, const std::string& targetFolderId) {
        // Validate file exists
        if (files.find(fileId) == files.end()) {
            throw FileNotFoundException("File with ID " + fileId);
        }
        
        // Validate target folder exists
        if (folders.find(targetFolderId) == folders.end()) {
            throw FolderNotFoundException("Target folder with ID " + targetFolderId);
        }
        
        // Get current folder
        std::string sourceFolderId = files[fileId]->getFolderId();
        
        // Verify file is actually in the source folder
        if (tree[sourceFolderId].find(fileId) == tree[sourceFolderId].end()) {
            throw FileSystemException("File system integrity error: File tree inconsistent");
        }
        
        // Check for duplicate filename in target folder
        std::string fileName = files[fileId]->getFileName();
        for (const auto& pair : tree[targetFolderId]) {
            const auto& key = pair.first;
            if (key[0] == 'f' && files[key]->getFileName() == fileName) {
                throw FileAlreadyExistsException(fileName + " already exists in destination folder");
            }
        }
        
        // Strong exception guarantee with copy-and-swap idiom
        auto treeCopy = tree;
        
        // Update tree structure in the copy
        treeCopy[sourceFolderId].erase(fileId);
        treeCopy[targetFolderId][fileId] = 1;
        
        // If source folder is empty after removal, clean it up
        if (treeCopy[sourceFolderId].empty()) {
            treeCopy.erase(sourceFolderId);
        }
        
        // Apply changes (non-throwing operations)
        tree = std::move(treeCopy);
        files[fileId]->setFolderId(targetFolderId);
        
        std::cout << "File moved successfully: " << fileName << std::endl;
    }
};

// ========== EXAMPLE USAGE ==========

class FileSystem {
    // Simplified placeholder
public:
    std::string getCurrentFolder() { return "F1"; }
};

int main() {
    try {
        // Get storage instance
        auto& storage = Storage::getInstance();
        
        // Create a file - should succeed
        try {
            storage.addFile("document.txt", "F1");
            std::cout << "File creation succeeded" << std::endl;
        } catch (const FileSystemException& e) {
            std::cout << "Error at " << e.getTimestamp() << ": " << e.what() << std::endl;
        }
        
        // Create a file with the same name - should fail
        try {
            storage.addFile("document.txt", "F1");
            std::cout << "File creation succeeded" << std::endl;
        } catch (const FileAlreadyExistsException& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Suggested action: Choose a different name" << std::endl;
        } catch (const FileSystemException& e) {
            std::cout << "Error at " << e.getTimestamp() << ": " << e.what() << std::endl;
        }
        
        // Create a file with invalid characters - should fail
        try {
            storage.addFile("invalid:file.txt", "F1");
            std::cout << "File creation succeeded" << std::endl;
        } catch (const InvalidOperationException& e) {
            std::cout << "Error: " << e.what() << std::endl;
        } catch (const FileSystemException& e) {
            std::cout << "Error at " << e.getTimestamp() << ": " << e.what() << std::endl;
        }
        
        // Remove a non-existent file - should fail
        try {
            storage.removeFile("nonexistent.txt", "F1");
            std::cout << "File removal succeeded" << std::endl;
        } catch (const FileNotFoundException& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Suggested action: Check filename spelling" << std::endl;
        } catch (const FileSystemException& e) {
            std::cout << "Error at " << e.getTimestamp() << ": " << e.what() << std::endl;
        }
        
        // Access a non-existent folder - should fail
        try {
            storage.addFile("test.txt", "INVALID_FOLDER");
            std::cout << "File creation succeeded" << std::endl;
        } catch (const FolderNotFoundException& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Suggested action: Create the folder first" << std::endl;
        } catch (const FileSystemException& e) {
            std::cout << "Error at " << e.getTimestamp() << ": " << e.what() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 2;
    }
    
    return 0;
}