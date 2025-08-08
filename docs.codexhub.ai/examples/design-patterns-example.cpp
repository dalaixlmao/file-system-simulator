// design-patterns-example.cpp
// Implementation of improved design patterns for the File System Simulator

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <functional>
#include <algorithm>

// Forward declarations
class FileSystemEntity;
class File;
class Folder;

// ========== COMPOSITE PATTERN ==========

// Base class for the composite pattern
class FileSystemEntity {
public:
    virtual ~FileSystemEntity() = default;
    
    virtual std::string getId() const = 0;
    virtual std::string getName() const = 0;
    virtual bool isFolder() const = 0;
    virtual void display(int depth = 0) const = 0;
    
    // Common utility for both files and folders
    virtual std::string getFullPath(const std::function<std::string(const std::string&)>& getParentPath) const = 0;
};

// Leaf node in composite pattern
class File : public FileSystemEntity {
private:
    std::string id;
    std::string name;
    std::string extension;
    std::string content;
    std::string folderId;

public:
    File(std::string id, std::string fileName, std::string folderId) 
        : id(id), folderId(folderId) 
    {
        // Parse file name to separate name and extension
        size_t dotPos = fileName.find_last_of('.');
        if (dotPos != std::string::npos) {
            name = fileName.substr(0, dotPos);
            extension = fileName.substr(dotPos + 1);
        } else {
            name = fileName;
            extension = "";
        }
    }

    // Implementation of FileSystemEntity interface
    std::string getId() const override { return id; }
    
    std::string getName() const override { 
        return extension.empty() ? name : name + "." + extension;
    }
    
    bool isFolder() const override { return false; }
    
    void setContent(const std::string& newContent) { content = newContent; }
    
    std::string getContent() const { return content; }
    
    std::string getFolderId() const { return folderId; }
    
    void display(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "- " << getName() << std::endl;
    }
    
    std::string getFullPath(const std::function<std::string(const std::string&)>& getParentPath) const override {
        return getParentPath(folderId) + "/" + getName();
    }
};

// Composite node in composite pattern
class Folder : public FileSystemEntity {
private:
    std::string id;
    std::string name;
    std::string parentId;

public:
    Folder(std::string id, std::string name, std::string parentId)
        : id(id), name(name), parentId(parentId) {}

    // Implementation of FileSystemEntity interface
    std::string getId() const override { return id; }
    
    std::string getName() const override { return name; }
    
    bool isFolder() const override { return true; }
    
    std::string getParentId() const { return parentId; }
    
    void display(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "+ " << name << std::endl;
        // Child display would be implemented here if we had direct references to children
    }
    
    std::string getFullPath(const std::function<std::string(const std::string&)>& getParentPath) const override {
        if (parentId == "F0") { // Root folder
            return "/" + name;
        }
        return getParentPath(parentId) + "/" + name;
    }
};

// ========== FACTORY METHOD PATTERN ==========

// Abstract Factory for creating file system entities
class FileSystemEntityFactory {
public:
    virtual ~FileSystemEntityFactory() = default;
    
    virtual std::unique_ptr<File> createFile(
        const std::string& id, 
        const std::string& name, 
        const std::string& folderId) = 0;
        
    virtual std::unique_ptr<Folder> createFolder(
        const std::string& id, 
        const std::string& name, 
        const std::string& parentId) = 0;
};

// Concrete factory implementation
class StandardFileSystemEntityFactory : public FileSystemEntityFactory {
public:
    std::unique_ptr<File> createFile(
        const std::string& id, 
        const std::string& name, 
        const std::string& folderId) override 
    {
        return std::make_unique<File>(id, name, folderId);
    }
    
    std::unique_ptr<Folder> createFolder(
        const std::string& id, 
        const std::string& name, 
        const std::string& parentId) override 
    {
        return std::make_unique<Folder>(id, name, parentId);
    }
};

// ========== MODERN SINGLETON PATTERN ==========

// Forward declaration of FileSystem
class FileSystem;

// Modern thread-safe singleton implementation
class Storage {
private:
    // Private constructor
    Storage() {
        // Initialize file system
        fileSystem = std::make_unique<FileSystem>();
        
        // Create root folder
        auto factory = createEntityFactory();
        auto rootFolder = factory->createFolder("F1", "root", "F0");
        folders["F1"] = std::move(rootFolder);
        
        // Set up folder hierarchy
        tree["F0"]["F1"] = 1;
    }
    
    // Deleted copy and move operations
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;
    
    // Storage structures using smart pointers for memory safety
    std::unique_ptr<FileSystem> fileSystem;
    std::map<std::string, std::unique_ptr<Folder>> folders;
    std::map<std::string, std::unique_ptr<File>> files;
    std::map<std::string, std::map<std::string, int>> tree;
    
    // Factory method to create the entity factory
    std::unique_ptr<FileSystemEntityFactory> createEntityFactory() {
        return std::make_unique<StandardFileSystemEntityFactory>();
    }

public:
    // Static method for accessing the singleton instance
    static Storage& getInstance() {
        // Guaranteed to be thread-safe in C++11 and beyond
        static Storage instance;
        return instance;
    }
    
    // Example storage operation
    void addFile(const std::string& name, const std::string& folderId) {
        // Validate if file exists
        for (const auto& pair : tree[folderId]) {
            const auto& entityId = pair.first;
            if (entityId[0] == 'f' && files[entityId]->getName() == name) {
                std::cout << "File already exists with name: " << name << std::endl;
                return;
            }
        }
        
        // Create new file
        std::string newFileId = "f" + std::to_string(files.size());
        auto factory = createEntityFactory();
        auto newFile = factory->createFile(newFileId, name, folderId);
        
        // Store file and update tree
        files[newFileId] = std::move(newFile);
        tree[folderId][newFileId] = 1;
        
        std::cout << "File created: " << name << " (ID: " << newFileId << ")" << std::endl;
    }
    
    // Other storage operations would be implemented here...
};

// ========== DEPENDENCY INJECTION PATTERN ==========

// Simple FileSystem implementation
class FileSystem {
private:
    std::vector<std::string> pathComponents;

public:
    void addFolderId(const std::string& id) {
        pathComponents.push_back(id);
    }
    
    void removeCurrentFolder() {
        if (!pathComponents.empty()) {
            pathComponents.pop_back();
        }
    }
    
    std::string getCurrentFolder() const {
        return pathComponents.empty() ? "F0" : pathComponents.back();
    }
    
    bool isEmpty() const {
        return pathComponents.empty();
    }
};

// Service interfaces
class FileServiceInterface {
public:
    virtual ~FileServiceInterface() = default;
    virtual void createFile(const std::string& folderId, const std::string& fileName) = 0;
    // Other file operations...
};

class FolderServiceInterface {
public:
    virtual ~FolderServiceInterface() = default;
    virtual void createFolder(const std::string& parentId, const std::string& name) = 0;
    virtual std::string getCurrentFolder() const = 0;
    // Other folder operations...
};

// Concrete service implementations
class FileService : public FileServiceInterface {
public:
    void createFile(const std::string& folderId, const std::string& fileName) override {
        Storage::getInstance().addFile(fileName, folderId);
    }
    // Other implementations...
};

class FolderService : public FolderServiceInterface {
public:
    void createFolder(const std::string& parentId, const std::string& name) override {
        // Would call Storage::getInstance().addFolder(...)
    }
    
    std::string getCurrentFolder() const override {
        // Would call Storage::getInstance().getCurrentFolderId()
        return "F1"; // Placeholder
    }
    // Other implementations...
};

// Main service with dependency injection
class FileSystemService {
private:
    // Dependencies injected and owned through smart pointers
    std::unique_ptr<FileServiceInterface> fileService;
    std::unique_ptr<FolderServiceInterface> folderService;

public:
    // Constructor with dependency injection
    FileSystemService(
        std::unique_ptr<FileServiceInterface> fileService,
        std::unique_ptr<FolderServiceInterface> folderService)
        : fileService(std::move(fileService)),
          folderService(std::move(folderService)) {}
    
    // Factory method for creating standard service
    static std::unique_ptr<FileSystemService> createStandard() {
        return std::make_unique<FileSystemService>(
            std::make_unique<FileService>(),
            std::make_unique<FolderService>()
        );
    }
    
    // Delegating methods
    void createFile(const std::string& fileName) {
        fileService->createFile(folderService->getCurrentFolder(), fileName);
    }
    
    // Other methods would be implemented similarly...
};

// ========== COMMAND PATTERN ==========

// Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string description() const = 0;
};

// Example command implementation
class CreateFileCommand : public Command {
private:
    FileSystemService& service;
    std::string fileName;
    // Would store state for undo operation in a real implementation
    
public:
    CreateFileCommand(FileSystemService& service, const std::string& fileName)
        : service(service), fileName(fileName) {}
        
    void execute() override {
        service.createFile(fileName);
    }
    
    void undo() override {
        // Would implement undo logic
    }
    
    std::string description() const override {
        return "Create file: " + fileName;
    }
};

// Command history manager
class CommandManager {
private:
    std::vector<std::unique_ptr<Command>> history;
    size_t currentIndex = 0;
    
public:
    void executeCommand(std::unique_ptr<Command> command) {
        // Execute the command
        command->execute();
        
        // Clear any undone commands
        if (currentIndex < history.size()) {
            history.resize(currentIndex);
        }
        
        // Add to history
        history.push_back(std::move(command));
        currentIndex++;
    }
    
    bool canUndo() const {
        return currentIndex > 0;
    }
    
    bool canRedo() const {
        return currentIndex < history.size();
    }
    
    void undo() {
        if (canUndo()) {
            currentIndex--;
            history[currentIndex]->undo();
        }
    }
    
    void redo() {
        if (canRedo()) {
            history[currentIndex]->execute();
            currentIndex++;
        }
    }
};

// ========== EXAMPLE USAGE ==========

int main() {
    // Create service with dependency injection
    auto fileSystemService = FileSystemService::createStandard();
    
    // Create command manager
    CommandManager commandManager;
    
    // Execute commands through the command manager
    commandManager.executeCommand(
        std::make_unique<CreateFileCommand>(*fileSystemService, "example.txt")
    );
    
    // Undo operation
    if (commandManager.canUndo()) {
        commandManager.undo();
    }
    
    // Redo operation
    if (commandManager.canRedo()) {
        commandManager.redo();
    }
    
    return 0;
}