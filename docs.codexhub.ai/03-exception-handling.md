# Exception Handling in C++: Best Practices

## Introduction

Exception handling is a crucial aspect of robust C++ programming. It provides a structured way to detect and respond to runtime anomalies, allowing your program to recover from errors gracefully or fail in a controlled manner. In your File System Simulator project, proper exception handling can significantly improve reliability and user experience.

This guide will explore C++ exception handling principles, demonstrate best practices, and show how to implement effective error handling in your code.

## Current Error Handling in Your Codebase

Currently, your File System Simulator handles errors primarily through console output and function return values. For example:

```cpp
// From Storage::addFile()
for (auto i : tree[folderId]) {
    if (i.first[0] == 'f' && files[i.first]->getFileName() == name) {
        cout << "     " << "File name already exist! change the name of the file." << endl;
        return;
    }
}
```

**Issues with this approach:**
- Error handling is mixed with business logic
- Errors can be easily ignored
- No systematic way to handle different types of errors
- Testing error conditions is difficult
- Impossible to recover programmatically from errors

## Understanding C++ Exceptions

### Basic Exception Concepts

Exceptions in C++ follow this flow:
1. An error condition is detected
2. An exception is thrown using the `throw` keyword
3. The runtime unwinds the stack looking for a matching `catch` block
4. When found, the appropriate `catch` block executes
5. Execution continues after the `try-catch` block

### Exception Handling Syntax

```cpp
try {
    // Code that might throw an exception
    if (someErrorCondition) {
        throw std::runtime_error("Something went wrong");
    }
} catch (const std::runtime_error& e) {
    // Handle the specific exception type
    std::cerr << "Runtime error: " << e.what() << std::endl;
} catch (const std::exception& e) {
    // Handle any standard exception not caught above
    std::cerr << "Standard exception: " << e.what() << std::endl;
} catch (...) {
    // Catch any other exception type
    std::cerr << "Unknown exception occurred" << std::endl;
}
```

## Creating a Custom Exception Hierarchy

For your File System Simulator, a custom exception hierarchy would provide clear and specific error handling:

```cpp
// Base exception class for all file system errors
class FileSystemException : public std::runtime_error {
public:
    explicit FileSystemException(const std::string& message) 
        : std::runtime_error(message) {}
};

// Specific exception types
class FileNotFoundException : public FileSystemException {
public:
    explicit FileNotFoundException(const std::string& filename) 
        : FileSystemException("File not found: " + filename),
          filename(filename) {}
    
    const std::string& getFilename() const { return filename; }
    
private:
    std::string filename;
};

class FileAlreadyExistsException : public FileSystemException {
public:
    explicit FileAlreadyExistsException(const std::string& filename) 
        : FileSystemException("File already exists: " + filename),
          filename(filename) {}
    
    const std::string& getFilename() const { return filename; }
    
private:
    std::string filename;
};

class FolderNotFoundException : public FileSystemException {
public:
    explicit FolderNotFoundException(const std::string& foldername) 
        : FileSystemException("Folder not found: " + foldername),
          foldername(foldername) {}
    
    const std::string& getFoldername() const { return foldername; }
    
private:
    std::string foldername;
};

// And so on for other specific error types...
```

## Implementing Exception Handling in Your Code

Let's refactor parts of your File System Simulator to use exceptions instead of console error messages:

### Example 1: Adding a File

```cpp
// Before
void Storage::addFile(string name, string folderId) {
    for (auto i : tree[folderId]) {
        if (i.first[0] == 'f' && files[i.first]->getFileName() == name) {
            cout << "     " << "File name already exist! change the name of the file." << endl;
            return;
        }
    }
    // Create file...
}

// After
void Storage::addFile(const std::string& name, const std::string& folderId) {
    // First validate the folder exists
    if (folders.find(folderId) == folders.end()) {
        throw FolderNotFoundException(folderId);
    }
    
    // Check if file already exists
    for (const auto& i : tree[folderId]) {
        if (i.first[0] == 'f' && files[i.first]->getFileName() == name) {
            throw FileAlreadyExistsException(name);
        }
    }
    
    // Create file...
    // If anything else goes wrong (like out of memory), let C++ standard exceptions propagate
}
```

### Example 2: Handling Exceptions in the UI Layer

```cpp
// In main.cpp or UI handler
void handleCreateFile(FileSystemService* fileSystem) {
    string fileName;
    cin >> fileName;
    
    try {
        fileSystem->createFile(fileSystem->getCurrentFolder(), fileName);
        cout << "File created successfully." << endl;
    } catch (const FileAlreadyExistsException& e) {
        cout << "Error: " << e.what() << endl;
        cout << "Please choose a different filename." << endl;
    } catch (const FolderNotFoundException& e) {
        cout << "Error: " << e.what() << endl;
        cout << "Current folder no longer exists." << endl;
    } catch (const std::exception& e) {
        cout << "An unexpected error occurred: " << e.what() << endl;
    }
}
```

## Exception Safety Guarantees

When implementing exception handling, consider these safety guarantees:

### 1. Basic Exception Safety (minimal guarantee)
- If an exception occurs, no resources are leaked
- The program remains in a valid but unspecified state

### 2. Strong Exception Safety (commit-or-rollback)
- If an exception occurs, the state is unchanged from before the operation

### 3. No-Throw Guarantee
- Operation will not throw exceptions
- Typically used for destructors and cleanup operations

Example of providing the strong guarantee:

```cpp
void Storage::moveFile(const std::string& fileId, const std::string& newFolderId) {
    // Validate inputs
    if (files.find(fileId) == files.end()) {
        throw FileNotFoundException("Unknown file ID");
    }
    if (folders.find(newFolderId) == folders.end()) {
        throw FolderNotFoundException("Destination folder not found");
    }
    
    // Get current folder ID
    std::string currentFolderId = files[fileId]->getFolderId();
    
    // Strong exception guarantee using copy-and-swap idiom
    auto tempTree = tree; // Copy current state
    
    // Make changes to the copy
    tempTree[currentFolderId].erase(fileId);
    tempTree[newFolderId][fileId] = 1;
    
    // If we got here, update the actual data (can't throw)
    tree = std::move(tempTree);
    
    // Update file's folder ID (might throw, but won't affect consistency)
    files[fileId]->setFolderId(newFolderId);
}
```

## Exception Handling Best Practices

1. **Throw by value, catch by reference**
   ```cpp
   throw MyException("Error");  // Throw by value
   catch (const MyException& e) { /* ... */ }  // Catch by const reference
   ```

2. **Use specific exception types for specific errors**
   - Create a hierarchy of exceptions
   - Catch exceptions in order from most specific to most general

3. **Keep exception handling separate from normal code flow**
   ```cpp
   try {
       // Only code that might throw exceptions
   } catch (...) {
       // Only exception handling code
   }
   ```

4. **Avoid throwing exceptions in destructors**
   - Destructors should provide the no-throw guarantee
   - Use a boolean flag or std::optional to handle errors in destructors

5. **Document the exceptions your functions can throw**
   ```cpp
   // @throws FileNotFoundException if the file doesn't exist
   // @throws std::runtime_error if the file can't be opened
   void readFile(const std::string& filename);
   ```

6. **Use RAII to ensure resource cleanup**
   - Resource Acquisition Is Initialization ensures resources are properly released
   - Combined with exception handling, prevents resource leaks

## When to Use Exceptions vs. Error Codes

**Use Exceptions When:**
- Error handling is separate from the normal code path
- Errors are exceptional (not part of normal operation)
- Errors need to be propagated up the call stack
- You need rich context about an error

**Use Error Codes When:**
- Errors are expected as part of normal operation
- Performance is absolutely critical (exceptions have overhead)
- Working in low-level code or with C APIs
- Error handling is local to the function

## Practice Exercises

### Exercise 1: Complete the Exception Hierarchy
Extend the `FileSystemException` hierarchy with appropriate exception classes for all error conditions in your File System Simulator.

### Exercise 2: Implement Try-Catch Blocks
Refactor the main command loop to handle exceptions for each command, providing user-friendly error messages.

### Exercise 3: Add Strong Exception Safety
Identify a key method in your code and refactor it to provide the strong exception safety guarantee.

### Exercise 4: Create a Custom Exception Class with Context
Create a custom exception class that captures additional context about an error, such as operation type, file path, and timestamp.

## Real-world Application

Consider updating these specific files in your codebase:

1. `include/storage/Storage.h`: Define exception classes and update method signatures
2. `src/storage/Storage.cpp`: Implement exception throwing instead of console error messages
3. `main.cpp`: Add exception handling for commands in the main loop

## Conclusion

Proper exception handling is essential for building robust C++ applications. By creating a clear exception hierarchy, following best practices, and providing appropriate exception safety guarantees, you can dramatically improve the reliability and maintainability of your File System Simulator.

Remember these key points:
- Use exceptions for exceptional circumstances
- Create a meaningful exception hierarchy
- Throw by value, catch by reference
- Always provide at least the basic exception safety guarantee
- Use RAII to prevent resource leaks
- Document the exceptions your functions can throw