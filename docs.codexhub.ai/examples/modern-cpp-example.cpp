// modern-cpp-example.cpp
// Demonstration of modern C++ features for the File System Simulator

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <optional>
#include <variant>
#include <functional>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <tuple>
#include <stdexcept>
#include <format>

namespace fs = std::filesystem;

// Forward declarations
class FileSystemEntity;
class File;
class Folder;

// ========== MODERN ENUMS ==========

enum class EntityType {
    File,
    Folder
};

enum class OperationResult {
    Success,
    FileNotFound,
    FolderNotFound,
    AlreadyExists,
    PermissionDenied,
    InvalidOperation
};

// ========== FILE SYSTEM ENTITY WITH MODERN C++ ==========

// Base class using virtual inheritance
class FileSystemEntity {
public:
    virtual ~FileSystemEntity() = default;
    
    [[nodiscard]] virtual std::string_view getId() const = 0;
    [[nodiscard]] virtual std::string_view getName() const = 0;
    [[nodiscard]] virtual EntityType getType() const = 0;
    
    // Use string_view for non-owning string references
    virtual void display(std::ostream& os, int depth = 0) const = 0;
    
    // Default implementations for copy and move
    FileSystemEntity() = default;
    FileSystemEntity(const FileSystemEntity&) = default;
    FileSystemEntity& operator=(const FileSystemEntity&) = default;
    FileSystemEntity(FileSystemEntity&&) noexcept = default;
    FileSystemEntity& operator=(FileSystemEntity&&) noexcept = default;
};

// Modern File implementation
class File final : public FileSystemEntity {
private:
    std::string id;
    std::string name;
    std::string extension;
    std::string content;
    std::string parentFolderId;
    std::chrono::system_clock::time_point creationTime;
    std::chrono::system_clock::time_point modifiedTime;

public:
    // Constructor using string_view for efficiency and modern parameter passing
    File(std::string id, std::string_view fileName, std::string parentFolderId)
        : id(std::move(id)), 
          parentFolderId(std::move(parentFolderId)),
          creationTime(std::chrono::system_clock::now()),
          modifiedTime(std::chrono::system_clock::now())
    {
        // Efficient string parsing using string_view
        auto dotPos = fileName.find_last_of('.');
        if (dotPos != std::string_view::npos) {
            name = fileName.substr(0, dotPos);
            extension = fileName.substr(dotPos + 1);
        } else {
            name = fileName;
            // Use empty initialization instead of empty string literal
            extension = {};
        }
    }
    
    // Use string_view for efficient string passing
    [[nodiscard]] std::string_view getId() const override { return id; }
    
    [[nodiscard]] std::string_view getName() const override { 
        return name;
    }
    
    [[nodiscard]] EntityType getType() const override { 
        return EntityType::File; 
    }
    
    // Use const& for string parameters that are not modified
    void setContent(const std::string& newContent) { 
        content = newContent; 
        modifiedTime = std::chrono::system_clock::now();
    }
    
    // Use string_view for returning string references
    [[nodiscard]] std::string_view getContent() const { 
        return content; 
    }
    
    [[nodiscard]] std::string getFileName() const { 
        return extension.empty() ? name : name + "." + extension; 
    }
    
    [[nodiscard]] std::string_view getExtension() const { 
        return extension; 
    }
    
    [[nodiscard]] std::string_view getParentFolderId() const { 
        return parentFolderId; 
    }
    
    void setParentFolderId(std::string newParentId) {
        parentFolderId = std::move(newParentId);
    }
    
    // Format and display using modern C++ features
    void display(std::ostream& os, int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        os << indent << "- " << getFileName();
        
        // Format file size
        size_t size = content.size();
        os << " (" << size << " bytes)";
        
        // Format timestamp using modern C++ time utilities
        auto timeNow = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::hours>(
            timeNow - modifiedTime).count();
            
        if (duration < 24) {
            os << " [modified " << duration << " hours ago]";
        } else {
            auto days = duration / 24;
            os << " [modified " << days << " days ago]";
        }
        
        os << std::endl;
    }
    
    // Add factory method for creating files with unique IDs
    static std::unique_ptr<File> create(
        std::string_view fileName, 
        std::string_view parentFolderId,
        size_t nextId
    ) {
        std::string id = "f" + std::to_string(nextId);
        return std::make_unique<File>(std::move(id), fileName, std::string(parentFolderId));
    }
};

// Modern Folder implementation
class Folder final : public FileSystemEntity {
private:
    std::string id;
    std::string name;
    std::string parentId;
    std::chrono::system_clock::time_point creationTime;
    
    // Store child entities directly using variant and smart pointers
    std::vector<std::unique_ptr<FileSystemEntity>> children;

public:
    Folder(std::string id, std::string name, std::string parentId)
        : id(std::move(id)), 
          name(std::move(name)), 
          parentId(std::move(parentId)),
          creationTime(std::chrono::system_clock::now())
    {}
    
    [[nodiscard]] std::string_view getId() const override { return id; }
    [[nodiscard]] std::string_view getName() const override { return name; }
    [[nodiscard]] EntityType getType() const override { return EntityType::Folder; }
    [[nodiscard]] std::string_view getParentId() const { return parentId; }
    
    void addChild(std::unique_ptr<FileSystemEntity> entity) {
        children.push_back(std::move(entity));
    }
    
    // Get child by name with optional
    [[nodiscard]] std::optional<FileSystemEntity*> findChildByName(std::string_view childName) const {
        auto it = std::find_if(children.begin(), children.end(),
            [childName](const auto& child) {
                return child->getName() == childName;
            });
            
        if (it != children.end()) {
            return it->get();
        }
        return std::nullopt;
    }
    
    // Display folder and its children with modern formatting
    void display(std::ostream& os, int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        os << indent << "+ " << name;
        
        // Show child count
        os << " (" << children.size() << " items)";
        
        os << std::endl;
        
        // Sort children: folders first, then files, alphabetically within each group
        std::vector<std::reference_wrapper<const std::unique_ptr<FileSystemEntity>>> sortedChildren;
        for (const auto& child : children) {
            sortedChildren.push_back(std::cref(child));
        }
        
        std::sort(sortedChildren.begin(), sortedChildren.end(),
            [](const auto& a, const auto& b) {
                bool aIsFolder = a.get()->get()->getType() == EntityType::Folder;
                bool bIsFolder = b.get()->get()->getType() == EntityType::Folder;
                
                if (aIsFolder != bIsFolder) {
                    return aIsFolder; // Folders come first
                }
                
                return a.get()->get()->getName() < b.get()->get()->getName();
            });
            
        for (const auto& child : sortedChildren) {
            child.get()->get()->display(os, depth + 1);
        }
    }
    
    // Factory method for creating folders
    static std::unique_ptr<Folder> create(
        std::string_view name, 
        std::string_view parentId,
        size_t nextId
    ) {
        std::string id = "F" + std::to_string(nextId);
        return std::make_unique<Folder>(
            std::move(id), 
            std::string(name), 
            std::string(parentId)
        );
    }
};

// ========== MODERN FILESYSTEM IMPLEMENTATION ==========

// Result type using std::variant
using OperationResultVariant = std::variant<
    std::monostate,                // Success with no return value
    std::unique_ptr<FileSystemEntity>,  // Success with entity
    std::string                    // Error message
>;

class ModernFileSystem {
private:
    // Root folder
    std::unique_ptr<Folder> rootFolder;
    
    // Current path using std::filesystem
    fs::path currentPath;
    
    // ID generators with atomic for thread safety
    std::atomic<size_t> nextFileId{0};
    std::atomic<size_t> nextFolderId{0};
    
    // Current folder reference
    Folder* currentFolder;
    
    // Helper to find folder by path
    std::optional<Folder*> findFolder(const fs::path& path) {
        // Implementation would traverse the folder structure
        // This is a placeholder
        return currentFolder;
    }

public:
    // Constructor with modern initialization
    ModernFileSystem() 
        : nextFileId(0), 
          nextFolderId(0)
    {
        // Create root folder
        rootFolder = Folder::create("root", "", nextFolderId++);
        currentFolder = rootFolder.get();
        currentPath = "/";
    }
    
    // Create a file with modern error handling
    OperationResult createFile(std::string_view fileName) {
        // Validate filename
        if (fileName.empty()) {
            return OperationResult::InvalidOperation;
        }
        
        // Check if file exists
        if (currentFolder->findChildByName(fileName)) {
            return OperationResult::AlreadyExists;
        }
        
        // Create file with unique ID
        auto newFile = File::create(fileName, currentFolder->getId(), nextFileId++);
        currentFolder->addChild(std::move(newFile));
        
        return OperationResult::Success;
    }
    
    // Create a folder with modern C++ features
    OperationResult createFolder(std::string_view folderName) {
        if (folderName.empty()) {
            return OperationResult::InvalidOperation;
        }
        
        if (currentFolder->findChildByName(folderName)) {
            return OperationResult::AlreadyExists;
        }
        
        auto newFolder = Folder::create(folderName, currentFolder->getId(), nextFolderId++);
        currentFolder->addChild(std::move(newFolder));
        
        return OperationResult::Success;
    }
    
    // Change directory with std::optional
    OperationResult changeDirectory(std::string_view dirName) {
        if (dirName == "..") {
            // Go to parent directory if not root
            if (currentFolder != rootFolder.get()) {
                auto parentId = currentFolder->getParentId();
                // This is simplified - would need to search for parent
                currentFolder = rootFolder.get();
                currentPath = currentPath.parent_path();
                return OperationResult::Success;
            }
            return OperationResult::InvalidOperation;
        }
        
        // Find child folder
        auto childOpt = currentFolder->findChildByName(dirName);
        if (!childOpt) {
            return OperationResult::FolderNotFound;
        }
        
        auto* child = *childOpt;
        if (child->getType() != EntityType::Folder) {
            return OperationResult::InvalidOperation;
        }
        
        // Update current folder
        currentFolder = static_cast<Folder*>(child);
        currentPath /= std::string(dirName);
        return OperationResult::Success;
    }
    
    // Get current path with string_view
    [[nodiscard]] std::string_view getCurrentPath() const {
        static std::string pathStr;
        pathStr = currentPath.string();
        return pathStr;
    }
    
    // List items with modern output formatting
    void listItems(std::ostream& os = std::cout) const {
        os << "Contents of " << getCurrentPath() << ":\n";
        currentFolder->display(os);
    }
};

// ========== EXAMPLE USAGE ==========

// Helper for displaying operation results
void printResult(OperationResult result) {
    switch (result) {
        case OperationResult::Success:
            std::cout << "Operation successful\n";
            break;
        case OperationResult::FileNotFound:
            std::cout << "Error: File not found\n";
            break;
        case OperationResult::FolderNotFound:
            std::cout << "Error: Folder not found\n";
            break;
        case OperationResult::AlreadyExists:
            std::cout << "Error: Item already exists\n";
            break;
        case OperationResult::PermissionDenied:
            std::cout << "Error: Permission denied\n";
            break;
        case OperationResult::InvalidOperation:
            std::cout << "Error: Invalid operation\n";
            break;
    }
}

int main() {
    // Create a file system
    ModernFileSystem fileSystem;
    
    // Create some folders
    printResult(fileSystem.createFolder("documents"));
    printResult(fileSystem.createFolder("pictures"));
    
    // Change into documents folder
    printResult(fileSystem.changeDirectory("documents"));
    
    // Create some files
    printResult(fileSystem.createFile("report.txt"));
    printResult(fileSystem.createFile("notes.md"));
    
    // Try to create a duplicate file
    printResult(fileSystem.createFile("report.txt"));
    
    // List the current directory
    fileSystem.listItems();
    
    // Go back to root
    printResult(fileSystem.changeDirectory(".."));
    
    // Change to pictures
    printResult(fileSystem.changeDirectory("pictures"));
    
    // Create files in pictures
    printResult(fileSystem.createFile("vacation.jpg"));
    printResult(fileSystem.createFile("family.png"));
    
    // List pictures directory
    fileSystem.listItems();
    
    // Go back to root and list everything
    printResult(fileSystem.changeDirectory(".."));
    std::cout << "\nFull directory listing:\n";
    fileSystem.listItems();
    
    return 0;
}