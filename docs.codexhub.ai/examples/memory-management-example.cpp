// memory-management-example.cpp
// A demonstration of proper memory management for the File System Simulator

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>

// Forward declarations
class File;
class Folder;
class FileSystem;
class FileService;
class FolderService;

// File class with RAII principles
class File {
private:
    std::string id;
    std::string name;
    std::string content;
    std::string extension;
    std::string folderId;

public:
    // Constructor that handles proper initialization
    File(std::string id, std::string fileName, std::string folderId) 
        : id(id), folderId(folderId)
    {
        size_t dotPosition = fileName.find('.');
        if (dotPosition != std::string::npos) {
            name = fileName.substr(0, dotPosition);
            extension = fileName.substr(dotPosition + 1);
        } else {
            // Handle case with no extension
            name = fileName;
            extension = "";
        }
    }

    // No need for custom destructor (Rule of Zero)

    // Method implementations
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
};

// Folder class with RAII principles
class Folder {
private:
    std::string id;
    std::string name;
    std::string parentId;

public:
    Folder(std::string id, std::string name, std::string parentId) 
        : id(id), name(name), parentId(parentId) {}
    
    // No need for custom destructor (Rule of Zero)

    std::string getParentId() const { return parentId; }
    std::string getName() const { return name; }
    std::string getId() const { return id; }
};

// Improved FileSystem class
class FileSystem {
private:
    std::vector<std::string> pathComponents;

public:
    FileSystem() {}

    void addFolderId(const std::string& id) { 
        pathComponents.push_back(id); 
    }

    void removeCurrentFolder() {
        if (!pathComponents.empty()) {
            pathComponents.pop_back();
        }
    }

    std::string getCurrentFolder() const {
        if (!pathComponents.empty()) {
            return pathComponents.back();
        }
        return "";
    }

    bool isEmpty() const { 
        return pathComponents.empty(); 
    }
};

// Improved Singleton implementation for Storage
class Storage {
private:
    // Private constructor for singleton
    Storage() {
        // Initialize with a root folder
        fileSystem = std::make_unique<FileSystem>();
        fileSystem->addFolderId("F0");
        
        // Create root folder
        auto rootFolder = std::make_unique<Folder>("F1", "BaseFolder", "F0");
        fileSystem->addFolderId("F1");
        
        // Store in folders map
        folders["F1"] = std::move(rootFolder);
        
        // Initialize tree structure
        tree["F0"] = std::map<std::string, int>();
        tree["F1"] = std::map<std::string, int>();
        tree["F0"]["F1"] = 1;
    }
    
    // Delete copy/move operations to ensure singleton semantics
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;
    
    // Member variables using smart pointers
    std::unique_ptr<FileSystem> fileSystem;
    std::map<std::string, std::unique_ptr<Folder>> folders;
    std::map<std::string, std::unique_ptr<File>> files;
    std::map<std::string, std::map<std::string, int>> tree;
    
public:
    // Static method to access singleton instance (thread-safe in C++11)
    static Storage& getInstance() {
        static Storage instance;
        return instance;
    }
    
    // Example method for adding a file with proper memory management
    void addFile(const std::string& name, const std::string& folderId) {
        // Check if file exists
        std::string currentFolderId = folderId;
        for (const auto& entry : tree[currentFolderId]) {
            if (entry.first[0] == 'f') {
                const auto& file = files[entry.first];
                if (file && file->getFileName() == name) {
                    std::cout << "File name already exists!" << std::endl;
                    return;
                }
            }
        }
        
        // Create new file with a unique ID
        std::string newFileId = "f" + std::to_string(files.size());
        
        // Use make_unique to safely create the file
        files[newFileId] = std::make_unique<File>(newFileId, name, folderId);
        tree[folderId][newFileId] = 1;
        
        std::cout << "File created! Name: " << name 
                  << ", ID: " << newFileId 
                  << ", Parent: " << folderId << std::endl;
    }
    
    // Other methods...
};

// Improved service classes using smart pointers
class FileService {
private:
    // No need to store Storage pointer as we use the singleton pattern
    
public:
    FileService() {}
    
    void createFile(const std::string& folderId, const std::string& fileName) {
        Storage::getInstance().addFile(fileName, folderId);
    }
    
    // Other methods...
};

class FolderService {
private:
    // No need to store Storage pointer
    
public:
    FolderService() {}
    
    // Methods would call Storage::getInstance() as needed
};

// Improved FileSystemService class with proper memory management
class FileSystemService {
private:
    // Use unique_ptr for exclusive ownership of service objects
    std::unique_ptr<FileService> fileService;
    std::unique_ptr<FolderService> folderService;
    
public:
    // Constructor uses make_unique for exception safety
    FileSystemService() 
        : fileService(std::make_unique<FileService>()),
          folderService(std::make_unique<FolderService>())
    {
        // Nothing else needed - initialization is done in the initializer list
    }
    
    // No explicit destructor needed - smart pointers handle cleanup
    
    // Methods would delegate to fileService and folderService
};

// Example usage
int main() {
    // Create service
    auto fileSystemService = std::make_unique<FileSystemService>();
    
    // No explicit memory management needed
    
    // When fileSystemService goes out of scope, everything is automatically cleaned up
    return 0;
}