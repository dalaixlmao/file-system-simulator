# C++ Memory Management Best Practices

## Introduction

Memory management is one of the most critical aspects of C++ programming. Unlike languages with automatic garbage collection, C++ gives developers complete control over memory allocation and deallocation. This power comes with responsibility—improper memory management leads to bugs that are notoriously difficult to diagnose: memory leaks, dangling pointers, and undefined behavior.

In this guide, we'll explore best practices for memory management in C++ with practical examples from your File System Simulator project.

## Understanding the Problem

Let's first identify common memory management issues in your codebase:

### 1. Memory Leaks

In your `FileSystemService` constructor:

```cpp
FileSystemService::FileSystemService()
{
    folderService = new FolderService();
    fileService = new FileService();
}
```

There's dynamic memory allocation, but no corresponding deallocation. Without a proper destructor, these allocations are never freed, causing memory leaks.

### 2. Ownership Ambiguity

The `Storage` class manages `FileSystem`, `File`, and `Folder` objects, but ownership responsibilities are unclear:

```cpp
Storage::Storage()
{
    fileSystem = new FileSystem();
    fileSystem->addFolderId("F0");
    folders["F0"] = nullptr;
    // ...
}
```

### 3. Raw Pointers Throughout the Codebase

Raw pointers are used extensively without clear ownership semantics:

```cpp
Storage *store;
FileService *fileService;
FolderService *folderService;
```

## Modern C++ Memory Management Solutions

### 1. RAII (Resource Acquisition Is Initialization)

RAII is a fundamental C++ principle: tie resource management to object lifetime.

**Key Concept:** When you acquire a resource (like memory), immediately put it in a managing object whose destructor will release it.

### 2. Smart Pointers

Smart pointers automate memory management by tying deallocation to scope:

#### std::unique_ptr

For exclusive ownership:

```cpp
std::unique_ptr<FileService> fileService;
```

#### std::shared_ptr

For shared ownership:

```cpp
std::shared_ptr<Storage> store;
```

### 3. Rule of Five (or Zero)

Modern C++ classes should implement either:
- **Rule of Zero**: Use standard library components for all resource management
- **Rule of Five**: Define all of: destructor, copy constructor, move constructor, copy assignment operator, and move assignment operator

## Practical Improvements for Your Code

Let's refactor parts of your File System Simulator to use modern memory management:

### Example 1: Using RAII in FileSystemService

```cpp
class FileSystemService {
private:
    // Smart pointers manage memory automatically
    std::unique_ptr<FileService> fileService;
    std::unique_ptr<FolderService> folderService;
    // Storage is a singleton with its own lifetime management
    Storage* store;

public:
    FileSystemService() 
        : fileService(std::make_unique<FileService>()),
          folderService(std::make_unique<FolderService>()),
          store(Storage::getInstance())
    {
    }
    
    // No explicit destructor needed - smart pointers clean up automatically
};
```

### Example 2: Improved Singleton Pattern with Modern C++

```cpp
class Storage {
private:
    // Private constructor
    Storage();
    
    // Delete copy/move operations to ensure singleton semantics
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;
    
    // Static instance using Meyer's Singleton (thread-safe in C++11)
    static Storage& getInstance() {
        static Storage instance;
        return instance;
    }
    
    // Use smart pointers for owned resources
    std::unique_ptr<FileSystem> fileSystem;
    std::map<std::string, std::unique_ptr<Folder>> folders;
    std::map<std::string, std::unique_ptr<File>> files;
};
```

### Example 3: Safe Resource Management in Destructors

```cpp
// Original code with memory leak potential
~FileSystemService() = default; // Does nothing!

// Fixed version with proper cleanup
~FileSystemService() {
    // Clean up any resources that aren't managed by smart pointers
    // In this case, we don't need to do anything if using smart pointers
}
```

## Advanced Memory Management Techniques

### 1. Custom Deleters for Smart Pointers

When resources need special cleanup beyond `delete`:

```cpp
auto customDeleter = [](FileSystem* fs) {
    // Special cleanup logic here
    delete fs;
};
std::unique_ptr<FileSystem, decltype(customDeleter)> fileSystem(new FileSystem(), customDeleter);
```

### 2. Object Pools for Performance-Critical Code

When allocation/deallocation overhead is a concern:

```cpp
class FilePool {
    std::vector<std::unique_ptr<File>> pool;
public:
    File* acquireFile() {
        // Reuse or create files
    }
    void releaseFile(File* file) {
        // Return to pool instead of deleting
    }
};
```

### 3. Placement New for Advanced Control

For special allocation needs:

```cpp
// Allocate memory without constructing an object
void* memory = ::operator new(sizeof(File));

// Construct object at specific memory location
File* file = new(memory) File("id", "name", "folderId");

// Later, destroy without deallocating
file->~File();

// Finally deallocate memory
::operator delete(memory);
```

## Memory Management Debugging Tools

1. **Valgrind**: Detects memory leaks and other memory-related issues
2. **AddressSanitizer**: Fast memory error detector
3. **Visual Studio Memory Profiler**: For Windows development
4. **Clang Static Analyzer**: Identifies potential bugs at compile time

## Practice Exercises

### Exercise 1: Fix Memory Leaks
Identify and fix all memory leaks in the `Storage` class by properly managing the lifecycle of `File` and `Folder` objects.

### Exercise 2: Implement Smart Pointers
Refactor the `FileSystemService` class to use smart pointers for managing its dependencies.

### Exercise 3: RAII File Handler
Create a RAII-compliant wrapper for file operations that automatically closes file handles when objects go out of scope.

### Exercise 4: Implement Move Semantics
Add move constructors and move assignment operators to the `File` and `Folder` classes to optimize resource transfers.

## Real-world Application

Consider updating these specific files in your codebase:

1. `include/services/FileSystemService.h`: Replace raw pointers with smart pointers
2. `src/storage/Storage.cpp`: Implement proper memory cleanup in the singleton
3. `include/models/File.h` and `include/models/Folder.h`: Add proper destructors and consider move semantics

## Conclusion

Memory management is fundamental to writing robust C++ programs. By adopting modern C++ techniques like RAII and smart pointers, you can eliminate entire categories of memory-related bugs while making your code more maintainable.

Always remember:
- Every resource acquisition should be paired with a corresponding release
- Use smart pointers to automate memory management where possible
- Follow the Rule of Zero (or Five) for consistent resource management
- Clearly define ownership semantics for each pointer in your code