# Modern C++ Features and Best Practices

## Introduction

C++ has evolved significantly since C++11, with major updates in C++14, C++17, and C++20. These updates have introduced powerful features that make C++ code more expressive, safer, and more maintainable. Your File System Simulator project could benefit greatly from incorporating these modern C++ features.

This guide will introduce you to key modern C++ features and demonstrate how to apply them to improve your codebase.

## Understanding Modern C++ Features

### 1. Smart Pointers

Smart pointers provide automatic memory management, helping prevent memory leaks and dangling pointers.

#### std::unique_ptr

For exclusive ownership of a resource:

```cpp
// Instead of:
FileService *fileService = new FileService();
// ...
delete fileService;  // Must remember to delete!

// Use:
std::unique_ptr<FileService> fileService = std::make_unique<FileService>();
// Automatically deleted when it goes out of scope
```

#### std::shared_ptr

For shared ownership:

```cpp
std::shared_ptr<Storage> storage = std::make_shared<Storage>();
// Multiple pointers can share ownership, object deleted when last reference is gone
```

### 2. Auto Type Deduction

The `auto` keyword simplifies code by deducing types automatically:

```cpp
// Instead of:
std::map<std::string, int>::iterator it = myMap.begin();

// Use:
auto it = myMap.begin();
```

### 3. Range-based For Loops

Range-based for loops make iteration cleaner:

```cpp
// Instead of:
for (auto i : tree[folderId]) {
    if (i.first[0] == 'f' && files[i.first]->getFileName() == name) {
        // ...
    }
}

// Use:
for (const auto& [entityId, value] : tree[folderId]) {
    if (entityId[0] == 'f' && files[entityId]->getFileName() == name) {
        // ...
    }
}
```

### 4. Lambda Expressions

Lambdas enable inline anonymous functions:

```cpp
// Filtering files by extension
std::vector<std::string> getFilesByExtension(const std::string& folderId, const std::string& ext) {
    std::vector<std::string> result;
    
    for (const auto& [entityId, _] : tree[folderId]) {
        if (entityId[0] == 'f') {
            auto file = files[entityId];
            if (file->getExtension() == ext) {
                result.push_back(file->getFileName());
            }
        }
    }
    
    return result;
}

// With lambda and std::copy_if:
std::vector<std::string> getFilesByExtension(const std::string& folderId, const std::string& ext) {
    std::vector<std::string> result;
    
    auto isMatchingFile = [this, &ext](const auto& pair) {
        const auto& [entityId, _] = pair;
        return entityId[0] == 'f' && files[entityId]->getExtension() == ext;
    };
    
    auto extractFileName = [this](const auto& pair) {
        return files[pair.first]->getFileName();
    };
    
    std::transform(
        tree[folderId].begin(), tree[folderId].end(),
        std::back_inserter(result),
        [this, &isMatchingFile, &extractFileName](const auto& pair) {
            if (isMatchingFile(pair)) {
                return extractFileName(pair);
            }
            return std::string();
        }
    );
    
    // Remove empty strings
    result.erase(std::remove_if(result.begin(), result.end(), 
        [](const auto& s) { return s.empty(); }), result.end());
    
    return result;
}
```

### 5. nullptr Instead of NULL

Use `nullptr` for pointer initialization:

```cpp
// Instead of:
File* file = NULL;

// Use:
File* file = nullptr;
```

### 6. Strongly-typed Enums

Enum classes provide type safety and scoping:

```cpp
// Instead of:
enum FileType { Regular, Directory, Symlink };

// Use:
enum class FileType {
    Regular,
    Directory,
    Symlink
};

// Usage:
FileType type = FileType::Regular;
```

### 7. Move Semantics

Move semantics optimize performance when transferring resources:

```cpp
// Move constructor
File::File(File&& other) noexcept
    : id(std::move(other.id)),
      name(std::move(other.name)),
      content(std::move(other.content)),
      extension(std::move(other.extension)),
      folderId(std::move(other.folderId)) {
}

// Move assignment
File& File::operator=(File&& other) noexcept {
    if (this != &other) {
        id = std::move(other.id);
        name = std::move(other.name);
        content = std::move(other.content);
        extension = std::move(other.extension);
        folderId = std::move(other.folderId);
    }
    return *this;
}
```

### 8. std::optional

For values that may or may not exist:

```cpp
std::optional<File> findFileByName(const std::string& folderPath, const std::string& fileName) {
    // Search for the file
    for (const auto& [entityId, _] : tree[folderPath]) {
        if (entityId[0] == 'f' && files[entityId]->getFileName() == fileName) {
            return *files[entityId];  // Return the file
        }
    }
    return std::nullopt;  // No file found
}

// Usage:
auto file = findFileByName(currentPath, "document.txt");
if (file) {
    std::cout << "Found file: " << file->getFileName() << std::endl;
} else {
    std::cout << "File not found" << std::endl;
}
```

### 9. String View

Use `std::string_view` for non-owning references to strings:

```cpp
bool isValidFileName(std::string_view filename) {
    // Check if filename contains invalid characters
    return filename.find_first_of("\\/:*?\"<>|") == std::string_view::npos;
}
```

### 10. constexpr for Compile-time Computation

```cpp
constexpr bool isReservedName(std::string_view name) {
    const std::array<std::string_view, 3> reserved = {"CON", "PRN", "AUX"};
    for (auto r : reserved) {
        if (name == r) return true;
    }
    return false;
}
```

## Applying Modern C++ Features to Your File System Simulator

Let's refactor parts of your codebase to use modern C++ features:

### Example 1: Refactoring File Class with Modern C++

```cpp
// Original implementation
class File {
private:
    string id;
    string name;
    string content;
    string extension;
    string folderId;

public:
    File(string id, string fileName, string folderId) : id(id), folderId(folderId) {
        string n;
        int ind;
        for (int i = 0; i < fileName.size(); i++)
            if (fileName[i] == '.') {
                ind = i;
                break;
            }
        n = fileName.substr(0, ind);
        string ex = fileName.substr(ind + 1, n.size() - ind - 1);
        name = n;
        extension = ex;
    }

    void setContent(string content) { content = content; }
    string getContent() { return content; }
    string getId() { return id; }
    string getFileName() { return name + "." + extension; }
    string getFolderId() { return folderId; }
};

// Modern implementation
class File {
private:
    std::string id;
    std::string name;
    std::string content;
    std::string extension;
    std::string folderId;

public:
    File(std::string id, std::string_view fileName, std::string folderId) 
        : id(std::move(id)), folderId(std::move(folderId)) 
    {
        // Use std::string_view and find_last_of for more efficient parsing
        auto dotPos = fileName.find_last_of('.');
        
        if (dotPos != std::string_view::npos) {
            name = fileName.substr(0, dotPos);
            extension = fileName.substr(dotPos + 1);
        } else {
            name = fileName;
            extension = "";
        }
    }

    // Rule of five for proper resource management
    ~File() = default;
    File(const File&) = default;
    File& operator=(const File&) = default;
    File(File&&) noexcept = default;
    File& operator=(File&&) noexcept = default;

    // Use const for methods that don't modify the object
    void setContent(std::string newContent) { content = std::move(newContent); }
    [[nodiscard]] const std::string& getContent() const { return content; }
    [[nodiscard]] const std::string& getId() const { return id; }
    
    [[nodiscard]] std::string getFileName() const { 
        return extension.empty() ? name : name + "." + extension; 
    }
    
    [[nodiscard]] const std::string& getFolderId() const { return folderId; }
    
    // Add useful new methods
    [[nodiscard]] const std::string& getExtension() const { return extension; }
    [[nodiscard]] const std::string& getName() const { return name; }
    void setFolderId(std::string newFolderId) { folderId = std::move(newFolderId); }
};
```

### Example 2: Refactoring Storage Methods with Modern C++

```cpp
// Original implementation
void Storage::addFile(string name, string folderId) {
    for (auto i : tree[folderId]) {
        if (i.first[0] == 'f' && files[i.first]->getFileName() == name) {
            cout << "     " << "File name already exist! change the name of the file." << endl;
            return;
        }
    }
    string newFileId = getNewFileId();
    File *f = new File(newFileId, name, folderId);
    files[newFileId] = f;
    tree[folderId][newFileId] = 1;
    cout << "     " << "File created! File name = " + name + ", id =" + f->getId() + ", in folder id - " << folderId << endl;
}

// Modern implementation
void Storage::addFile(std::string name, const std::string& folderId) {
    // Check if file already exists using range-based for and structured bindings
    for (const auto& [entityId, _] : tree[folderId]) {
        if (entityId[0] == 'f' && files[entityId]->getFileName() == name) {
            throw FileAlreadyExistsException(name);
        }
    }
    
    // Generate unique ID
    std::string newFileId = "f" + std::to_string(files.size());
    
    // Use make_unique for exception safety
    auto newFile = std::make_unique<File>(newFileId, name, folderId);
    
    // Get ID before moving the unique_ptr
    const std::string& id = newFile->getId();
    
    // Store file
    files[newFileId] = std::move(newFile);
    tree[folderId][newFileId] = 1;
    
    std::cout << "File created: " << name 
              << ", ID: " << id
              << ", in folder: " << folderId << std::endl;
}
```

### Example 3: Using Algorithms and Lambdas for Tree Traversal

```cpp
// Original implementation
void Storage::showDFS(string node, string symbols) {
    cout << "     " << symbols + "- " << ((node[0] == 'F') ? folders[node]->getName() : files[node]->getFileName()) << endl;

    symbols += "  |";
    for (auto i : tree[node]) {
        showDFS(i.first, symbols);
    }
}

// Modern implementation with lambda and recursion
void Storage::showTree(const std::string& rootId) {
    // Define recursive lambda for tree traversal
    std::function<void(const std::string&, int)> traverseTree;
    
    traverseTree = [this, &traverseTree](const std::string& nodeId, int depth) {
        // Create indentation based on depth
        std::string indent(depth * 2, ' ');
        
        // Get node name based on type
        std::string nodeName;
        if (nodeId[0] == 'F') {
            nodeName = folders[nodeId]->getName();
            std::cout << indent << "+ " << nodeName << std::endl;
        } else {
            nodeName = files[nodeId]->getFileName();
            std::cout << indent << "- " << nodeName << std::endl;
        }
        
        // Recursively process children if this is a folder
        if (nodeId[0] == 'F' && tree.contains(nodeId)) {
            // Sort children for consistent display
            std::vector<std::string> childIds;
            for (const auto& [childId, _] : tree[nodeId]) {
                childIds.push_back(childId);
            }
            
            // Sort folders first, then files
            std::sort(childIds.begin(), childIds.end(), 
                [this](const std::string& a, const std::string& b) {
                    bool aIsFolder = a[0] == 'F';
                    bool bIsFolder = b[0] == 'F';
                    
                    if (aIsFolder != bIsFolder) {
                        return aIsFolder; // Folders come before files
                    }
                    
                    // Both are folders or both are files, sort by name
                    std::string aName = a[0] == 'F' ? folders[a]->getName() 
                                                   : files[a]->getFileName();
                    std::string bName = b[0] == 'F' ? folders[b]->getName() 
                                                   : files[b]->getFileName();
                    return aName < bName;
                });
            
            for (const auto& childId : childIds) {
                traverseTree(childId, depth + 1);
            }
        }
    };
    
    // Start traversal from root
    traverseTree(rootId, 0);
}
```

## Practice Exercises

### Exercise 1: Implement Move Semantics
Add move constructors and move assignment operators to the `File` and `Folder` classes.

### Exercise 2: Use Smart Pointers
Refactor the `Storage` class to use `std::unique_ptr` for managing `File` and `Folder` objects.

### Exercise 3: Implement std::optional
Create a method that returns an `std::optional<File>` when looking up a file that might not exist.

### Exercise 4: Refactor with Range-based For Loops
Identify and refactor at least three methods in your codebase to use range-based for loops with structured bindings.

### Exercise 5: Create a Utility Function with Lambdas
Implement a search function that uses lambdas to find files matching certain criteria (e.g., by extension, size, or name pattern).

## Real-world Application

Consider updating these specific files in your codebase:

1. `include/models/File.h`: Add modern C++ features like `const` methods, move semantics, and proper constructors
2. `src/storage/Storage.cpp`: Use range-based for loops, structured bindings, and smart pointers
3. `src/models/FileSystem.cpp`: Refactor to use STL containers more effectively

## Conclusion

Modern C++ features provide powerful tools to make your code more efficient, safer, and easier to maintain. By incorporating features like smart pointers, lambdas, range-based for loops, and move semantics, you can significantly improve your File System Simulator project.

Remember these key points:
- Use smart pointers for automatic memory management
- Leverage range-based for loops and structured bindings for cleaner iteration
- Use `auto` for type deduction, but be mindful of readability
- Apply move semantics for optimal performance with large objects
- Take advantage of STL algorithms and containers
- Mark methods as `const` when they don't modify object state
- Use `std::optional` for functions that may or may not return a value