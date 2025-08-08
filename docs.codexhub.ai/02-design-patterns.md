# Design Patterns in C++: Best Practices

## Introduction

Design patterns are proven solutions to recurring problems in software design. They represent best practices that evolved over time as developers faced and solved similar problems. Understanding and correctly implementing design patterns can significantly improve your code's structure, maintainability, and extensibility.

In this guide, we'll focus on several design patterns relevant to your File System Simulator project, with a particular emphasis on improving the current implementations.

## Understanding Design Patterns in Your Codebase

Your File System Simulator currently utilizes several design patterns, though some implementations could be improved:

### 1. Singleton Pattern (in Storage class)

The Singleton pattern ensures that a class has only one instance and provides a global point of access to it. Your current implementation:

```cpp
// include/storage/Storage.h
class Storage {
private:
    Storage();
    static Storage* instance;
public:
    static Storage* getInstance();
    // ...
};

// src/storage/Storage.cpp
Storage *Storage::instance = nullptr;

Storage *Storage::getInstance() {
    if (instance == nullptr) {
        return instance = new Storage();
    }
    return instance;
}
```

**Issues with this implementation:**
- Not thread-safe
- Potential memory leak (the singleton is never deleted)
- Non-standard implementation compared to modern C++ practices

### 2. Service Layer Pattern

You've implemented a service layer (FileService, FolderService, FileSystemService) to separate business logic from models, which is good practice:

```cpp
class FileSystemService {
private:
    Storage *store;
    FileService *fileService;
    FolderService *folderService;
public:
    // Operations that delegate to specific services
};
```

**Issues with this implementation:**
- Unclear dependencies between services
- No dependency injection, making testing difficult
- Raw pointer usage without clear ownership semantics

## Modern Design Pattern Implementations

### 1. Improved Singleton Pattern (Meyers Singleton)

```cpp
class Storage {
private:
    Storage(); // Private constructor
    
    // Delete copy/move operations
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;
    
public:
    // Thread-safe in C++11 and later due to static initialization guarantees
    static Storage& getInstance() {
        static Storage instance;
        return instance;
    }
};
```

**Advantages:**
- Thread-safe by default in C++11 and later
- No manual memory management required
- Cleaner syntax with references instead of pointers
- Proper encapsulation of copy/move operations

### 2. Factory Method Pattern

The Factory Method defines an interface for creating objects but lets subclasses decide which classes to instantiate.

```cpp
// Abstract Factory
class FileSystemEntityFactory {
public:
    virtual ~FileSystemEntityFactory() = default;
    virtual std::unique_ptr<File> createFile(const std::string& id, 
                                             const std::string& name, 
                                             const std::string& folderId) = 0;
    virtual std::unique_ptr<Folder> createFolder(const std::string& id, 
                                                const std::string& name, 
                                                const std::string& parentId) = 0;
};

// Concrete Factory
class StandardFileSystemEntityFactory : public FileSystemEntityFactory {
public:
    std::unique_ptr<File> createFile(const std::string& id, 
                                    const std::string& name, 
                                    const std::string& folderId) override {
        return std::make_unique<File>(id, name, folderId);
    }
    
    std::unique_ptr<Folder> createFolder(const std::string& id, 
                                        const std::string& name, 
                                        const std::string& parentId) override {
        return std::make_unique<Folder>(id, name, parentId);
    }
};
```

**Advantages:**
- Decouples object creation from the code that uses the objects
- Makes it easy to extend with new types of files or folders
- Simplifies testing with mock factories

### 3. Composite Pattern

The Composite pattern lets you compose objects into tree structures and work with them as individual objects. It's perfectly suited for a file system:

```cpp
// Common interface for File and Folder
class FileSystemEntity {
public:
    virtual ~FileSystemEntity() = default;
    virtual std::string getName() const = 0;
    virtual std::string getId() const = 0;
    virtual bool isFolder() const = 0;
    virtual void display(int depth = 0) const = 0;
};

// Leaf node
class File : public FileSystemEntity {
    // ...
public:
    bool isFolder() const override { return false; }
    void display(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "- " << getName() << std::endl;
    }
};

// Composite node
class Folder : public FileSystemEntity {
private:
    std::vector<std::unique_ptr<FileSystemEntity>> children;
    
public:
    bool isFolder() const override { return true; }
    
    void addChild(std::unique_ptr<FileSystemEntity> child) {
        children.push_back(std::move(child));
    }
    
    void display(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "+ " << getName() << std::endl;
        
        for (const auto& child : children) {
            child->display(depth + 1);
        }
    }
};
```

**Advantages:**
- Unified interface for files and folders
- Simplifies client code when working with the hierarchy
- Naturally models the tree structure of a file system

### 4. Command Pattern

The Command pattern encapsulates a request as an object, allowing parameterization of clients with different requests and queue or log requests:

```cpp
// Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string description() const = 0;
};

// Concrete command for creating files
class CreateFileCommand : public Command {
private:
    FileSystemService& service;
    std::string folderId;
    std::string fileName;
    std::string fileId;  // For undo
    
public:
    CreateFileCommand(FileSystemService& service, 
                     const std::string& folderId, 
                     const std::string& fileName)
        : service(service), folderId(folderId), fileName(fileName) {}
    
    void execute() override {
        fileId = service.createFile(folderId, fileName);
    }
    
    void undo() override {
        service.removeFile(fileId);
    }
    
    std::string description() const override {
        return "Create file: " + fileName;
    }
};

// Command manager for executing commands and handling history
class CommandManager {
private:
    std::vector<std::unique_ptr<Command>> history;
    size_t currentIndex = 0;
    
public:
    void executeCommand(std::unique_ptr<Command> command) {
        command->execute();
        
        // Clear any undone commands
        if (currentIndex < history.size()) {
            history.resize(currentIndex);
        }
        
        history.push_back(std::move(command));
        currentIndex++;
    }
    
    void undo() {
        if (currentIndex > 0) {
            currentIndex--;
            history[currentIndex]->undo();
        }
    }
    
    void redo() {
        if (currentIndex < history.size()) {
            history[currentIndex]->execute();
            currentIndex++;
        }
    }
};
```

**Advantages:**
- Enables undo/redo functionality
- Separates actions from their invokers
- Makes it easy to add new operations without changing existing code

## Applying Design Patterns to Your Project

Let's see how you can improve your File System Simulator with these patterns:

### 1. Refactor Storage as a Modern Singleton

```cpp
// include/storage/Storage.h
class Storage {
private:
    Storage();
    
    // Delete copy/move operations
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;
    
    // Private member variables using smart pointers
    std::unique_ptr<FileSystem> fileSystem;
    std::map<std::string, std::unique_ptr<Folder>> folders;
    std::map<std::string, std::unique_ptr<File>> files;
    
public:
    static Storage& getInstance() {
        static Storage instance;
        return instance;
    }
    
    // Rest of the methods...
};
```

### 2. Implement Dependency Injection for Services

```cpp
// Constructor with dependency injection
FileSystemService::FileSystemService(
    std::unique_ptr<FileService> fileService,
    std::unique_ptr<FolderService> folderService)
    : fileService(std::move(fileService)),
      folderService(std::move(folderService))
{
}

// Factory method for standard configuration
static std::unique_ptr<FileSystemService> createStandardFileSystemService() {
    return std::make_unique<FileSystemService>(
        std::make_unique<FileService>(),
        std::make_unique<FolderService>()
    );
}
```

### 3. Use the Composite Pattern for File and Folder

```cpp
class FileSystemEntity {
public:
    virtual ~FileSystemEntity() = default;
    virtual std::string getName() const = 0;
    virtual std::string getId() const = 0;
    virtual void display(int depth = 0) const = 0;
};

class File : public FileSystemEntity {
    // Implementation...
};

class Folder : public FileSystemEntity {
    // Implementation with children management...
};
```

## Practice Exercises

### Exercise 1: Implement the Observer Pattern
Add an event notification system to your File System Simulator so that observers can be notified when files or folders are created, modified, or deleted.

### Exercise 2: Refactor with the Strategy Pattern
Implement different storage strategies (in-memory, JSON file, database) using the Strategy pattern.

### Exercise 3: Add Command Pattern Support
Implement undo/redo functionality using the Command pattern for all file system operations.

### Exercise 4: Visitor Pattern Implementation
Create a visitor pattern implementation that can perform various operations on the file system tree, such as calculating total size, finding files by extension, etc.

## Real-world Application

Consider updating these specific files in your codebase:

1. `include/storage/Storage.h`: Refactor to use the modern Singleton pattern
2. `include/models/File.h` and `include/models/Folder.h`: Implement a common base class for the Composite pattern
3. `include/services/FileSystemService.h`: Add dependency injection for better testability

## Conclusion

Design patterns are powerful tools that can significantly improve your code's structure and maintainability. By properly implementing patterns like Singleton, Composite, Factory Method, and Command, you can make your File System Simulator more robust, extensible, and easier to test.

Remember these key points:
- Choose the right pattern for the problem at hand
- Understand the pattern's intent before implementing it
- Don't overuse patterns where simpler solutions would suffice
- Combine patterns when appropriate to solve complex problems
- Use modern C++ features to implement patterns more effectively