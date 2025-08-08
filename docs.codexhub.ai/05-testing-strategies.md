# Testing Strategies for C++ Code

## Introduction

Testing is a critical aspect of software development that ensures your code works as expected, remains maintainable, and is resilient to changes. Your File System Simulator project would benefit greatly from a comprehensive testing strategy to catch bugs early and verify functionality.

In this guide, we'll explore different testing approaches, frameworks, and best practices to help you implement effective testing for your C++ codebase.

## Why Testing Matters

Testing provides several key benefits:

1. **Bug Detection**: Identify issues before they reach production
2. **Regression Prevention**: Ensure changes don't break existing functionality
3. **Documentation**: Tests serve as executable documentation
4. **Design Improvement**: Writing testable code leads to better architecture
5. **Refactoring Confidence**: Make changes with confidence that you haven't broken anything

## Testing Levels for Your File System Simulator

### 1. Unit Testing

Unit testing focuses on testing individual components (functions, classes) in isolation.

#### What to Test:
- Individual methods of `File` and `Folder` classes
- Helper functions in services
- Individual operations like file creation, deletion, etc.

#### Example Unit Test for File Class:

```cpp
// Using Google Test framework
#include <gtest/gtest.h>
#include "../../include/models/File.h"

TEST(FileTest, ConstructorParsesFileNameCorrectly) {
    // Create a file with name and extension
    File file("f1", "document.txt", "F1");
    
    // Verify correct parsing
    EXPECT_EQ(file.getFileName(), "document.txt");
    EXPECT_EQ(file.getId(), "f1");
    EXPECT_EQ(file.getFolderId(), "F1");
}

TEST(FileTest, ConstructorHandlesFilesWithoutExtension) {
    // Create a file without extension
    File file("f2", "README", "F1");
    
    // Verify correct handling
    EXPECT_EQ(file.getFileName(), "README");
}

TEST(FileTest, ContentManagement) {
    File file("f3", "notes.txt", "F1");
    
    // Test content setting and retrieval
    file.setContent("Hello, world!");
    EXPECT_EQ(file.getContent(), "Hello, world!");
    
    // Test content update
    file.setContent("Updated content");
    EXPECT_EQ(file.getContent(), "Updated content");
}
```

### 2. Integration Testing

Integration testing verifies that components work together correctly.

#### What to Test:
- Interactions between services (`FileService`, `FolderService`, `FileSystemService`)
- Storage and service interactions
- Command operations that span multiple components

#### Example Integration Test:

```cpp
#include <gtest/gtest.h>
#include "../../include/services/FileSystemService.h"

class FileSystemServiceTest : public ::testing::Test {
protected:
    FileSystemService* service;
    
    void SetUp() override {
        service = new FileSystemService();
    }
    
    void TearDown() override {
        delete service;
    }
};

TEST_F(FileSystemServiceTest, CreateAndRemoveFile) {
    // Get current folder
    std::string currentFolder = service->getCurrentFolder();
    
    // Create a file
    service->createFile(currentFolder, "test.txt");
    
    // Verify file exists (would need a method to check)
    // For example: EXPECT_TRUE(service->fileExists("test.txt"));
    
    // Remove the file
    service->removeFile("test.txt");
    
    // Verify file no longer exists
    // EXPECT_FALSE(service->fileExists("test.txt"));
}
```

### 3. System Testing

System testing evaluates the entire application as a whole.

#### What to Test:
- Complete workflows (create folder → navigate → create file → write content)
- Error handling across the system
- Edge cases like file system limits, long paths, etc.

#### Example System Test:

```cpp
TEST(SystemTest, CompleteWorkflow) {
    FileSystemService service;
    
    // Get root folder
    std::string rootFolder = service.getCurrentFolder();
    
    // Create folder structure
    service.createFolder(rootFolder, "documents");
    service.getIntoFolder("documents");
    
    std::string documentsFolder = service.getCurrentFolder();
    service.createFolder(documentsFolder, "work");
    service.getIntoFolder("work");
    
    std::string workFolder = service.getCurrentFolder();
    
    // Create and manipulate files
    service.createFile(workFolder, "report.txt");
    service.addContent("report.txt", "Quarterly report content");
    
    // Navigate back up
    service.getIntoFolder("..");  // Back to documents
    service.getIntoFolder("..");  // Back to root
    
    // Verify path is correct
    EXPECT_EQ(service.currentPath(), "/");
}
```

## Test-Driven Development (TDD)

Test-Driven Development is a methodology where tests are written before implementing functionality:

1. **Write a failing test** for the functionality you want to implement
2. **Write the minimum code** to make the test pass
3. **Refactor** the code while ensuring tests still pass

### Example TDD Workflow:

```cpp
// Step 1: Write a failing test
TEST(FolderTest, RenameFolder) {
    Folder folder("F1", "Documents", "F0");
    
    folder.rename("Photos");
    
    EXPECT_EQ(folder.getName(), "Photos");
}

// Step 2: Implement the minimum code to pass
void Folder::rename(const std::string& newName) {
    name = newName;
}

// Step 3: Refactor if needed (add validation, error handling)
void Folder::rename(const std::string& newName) {
    if (newName.empty()) {
        throw std::invalid_argument("Folder name cannot be empty");
    }
    name = newName;
}
```

## C++ Testing Frameworks

### 1. Google Test (gtest)

Google Test is a popular C++ testing framework:

```cpp
// Install: apt-get install libgtest-dev

// CMakeLists.txt
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
```

### 2. Catch2

Catch2 is a modern, header-only testing framework:

```cpp
// Just include the header
#include "catch2/catch.hpp"

TEST_CASE("File operations", "[file]") {
    File file("f1", "document.txt", "F1");
    
    SECTION("File name parsing") {
        REQUIRE(file.getFileName() == "document.txt");
    }
    
    SECTION("Content management") {
        file.setContent("Hello");
        REQUIRE(file.getContent() == "Hello");
    }
}
```

### 3. Boost.Test

Boost.Test is part of the Boost libraries:

```cpp
#define BOOST_TEST_MODULE FileSystemTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(FileNameParsing) {
    File file("f1", "document.txt", "F1");
    
    BOOST_CHECK_EQUAL(file.getFileName(), "document.txt");
}
```

## Mock Objects for Testing

Mock objects simulate the behavior of real objects for testing. The Google Mock (gmock) library is commonly used with Google Test:

```cpp
// Define a mock file service
class MockFileService : public FileService {
public:
    MOCK_METHOD(void, createFile, (const std::string&, const std::string&), (override));
    MOCK_METHOD(void, removeFile, (const std::string&), (override));
};

// Test with mock
TEST(FileSystemServiceTest, DelegatesFileCreation) {
    MockFileService mockFileService;
    
    // Set up expectation
    EXPECT_CALL(mockFileService, createFile("F1", "test.txt"))
        .Times(1);  // Expect exactly one call
    
    // Create service with mock
    FileSystemService service(&mockFileService);
    
    // Call the method that should delegate to our mock
    service.createFile("F1", "test.txt");
}
```

## Test Fixtures for Shared Setup

Test fixtures provide a way to reuse setup and teardown code:

```cpp
class StorageTest : public ::testing::Test {
protected:
    Storage* storage;
    std::string rootFolderId;
    std::string testFolderId;
    
    void SetUp() override {
        storage = &Storage::getInstance();
        rootFolderId = storage->getCurrentFolderId();
        
        // Create a test folder for each test
        storage->addFolder("TestFolder", rootFolderId);
        
        // Find the created folder ID
        for (const auto& pair : storage->getTree()[rootFolderId]) {
            if (pair.first[0] == 'F' && storage->getFolder(pair.first)->getName() == "TestFolder") {
                testFolderId = pair.first;
                break;
            }
        }
    }
    
    void TearDown() override {
        // Clean up by removing test folder
        storage->removeFolder("TestFolder");
    }
};

// Test using the fixture
TEST_F(StorageTest, AddFile) {
    storage->addFile("test.txt", testFolderId);
    
    bool fileExists = false;
    for (const auto& pair : storage->getTree()[testFolderId]) {
        if (pair.first[0] == 'f') {
            auto file = storage->getFile(pair.first);
            if (file->getFileName() == "test.txt") {
                fileExists = true;
                break;
            }
        }
    }
    
    EXPECT_TRUE(fileExists);
}
```

## Implementing Testing in Your Project

### 1. Separating Test Code

Create a dedicated test directory:

```
file-system-simulator/
├── include/
├── src/
└── tests/
    ├── models/
    │   ├── FileTest.cpp
    │   └── FolderTest.cpp
    ├── services/
    │   ├── FileServiceTest.cpp
    │   └── FolderServiceTest.cpp
    └── storage/
        └── StorageTest.cpp
```

### 2. CMake Integration

```cmake
# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Create test executable
add_executable(tests
    tests/models/FileTest.cpp
    tests/models/FolderTest.cpp
    tests/services/FileServiceTest.cpp
    tests/services/FolderServiceTest.cpp
    tests/storage/StorageTest.cpp
)

# Link libraries
target_link_libraries(tests
    ${GTEST_LIBRARIES}
    pthread
    file_system_lib  # Your project's library
)

# Add tests
add_test(NAME model_tests COMMAND tests --gtest_filter="FileTest.*:FolderTest.*")
add_test(NAME service_tests COMMAND tests --gtest_filter="*ServiceTest.*")
```

## Making Your Code Testable

To make your File System Simulator more testable:

### 1. Dependency Injection

```cpp
// Before: Hard-coded dependencies
class FileSystemService {
private:
    Storage* store = Storage::getInstance();
    FileService* fileService = new FileService();
    FolderService* folderService = new FolderService();
    
public:
    // Methods...
};

// After: Dependencies injected through constructor
class FileSystemService {
private:
    Storage* store;
    std::unique_ptr<FileService> fileService;
    std::unique_ptr<FolderService> folderService;
    
public:
    FileSystemService(
        Storage* store,
        std::unique_ptr<FileService> fileService,
        std::unique_ptr<FolderService> folderService
    ) : store(store),
        fileService(std::move(fileService)),
        folderService(std::move(folderService)) {}
        
    // Factory method for convenience
    static std::unique_ptr<FileSystemService> createDefault() {
        return std::make_unique<FileSystemService>(
            &Storage::getInstance(),
            std::make_unique<FileService>(),
            std::make_unique<FolderService>()
        );
    }
    
    // Methods...
};
```

### 2. Interface-based Design

```cpp
// Define interfaces
class FileServiceInterface {
public:
    virtual ~FileServiceInterface() = default;
    virtual void createFile(const std::string& folderId, const std::string& fileName) = 0;
    virtual void removeFile(const std::string& fileName) = 0;
    // Other methods...
};

// Concrete implementation
class FileService : public FileServiceInterface {
public:
    void createFile(const std::string& folderId, const std::string& fileName) override {
        // Implementation...
    }
    
    void removeFile(const std::string& fileName) override {
        // Implementation...
    }
    
    // Other methods...
};

// Mock for testing
class MockFileService : public FileServiceInterface {
public:
    MOCK_METHOD(void, createFile, (const std::string&, const std::string&), (override));
    MOCK_METHOD(void, removeFile, (const std::string&), (override));
    // Other methods...
};
```

### 3. Separating Logic from I/O

```cpp
// Before: Mixed logic and I/O
void Storage::addFile(string name, string folderId) {
    // Check if file exists
    for (auto i : tree[folderId]) {
        if (i.first[0] == 'f' && files[i.first]->getFileName() == name) {
            cout << "File name already exists!" << endl;
            return;
        }
    }
    
    // Create file...
    cout << "File created!" << endl;
}

// After: Separate logic and I/O
enum class FileResult {
    Success,
    FileExists,
    FolderNotFound
};

FileResult Storage::addFile(const std::string& name, const std::string& folderId) {
    // Check if folder exists
    if (folders.find(folderId) == folders.end()) {
        return FileResult::FolderNotFound;
    }
    
    // Check if file exists
    for (const auto& pair : tree[folderId]) {
        if (pair.first[0] == 'f' && files[pair.first]->getFileName() == name) {
            return FileResult::FileExists;
        }
    }
    
    // Create file...
    return FileResult::Success;
}

// UI layer handles output
void handleAddFile(FileSystemService& service, const std::string& fileName) {
    FileResult result = service.addFile(fileName);
    
    switch (result) {
        case FileResult::Success:
            cout << "File created!" << endl;
            break;
        case FileResult::FileExists:
            cout << "File name already exists!" << endl;
            break;
        case FileResult::FolderNotFound:
            cout << "Current folder not found!" << endl;
            break;
    }
}
```

## Practice Exercises

### Exercise 1: Write Unit Tests
Write unit tests for the `File` and `Folder` classes, focusing on their core functionality.

### Exercise 2: Create a Test Fixture
Implement a test fixture for testing the `Storage` class with proper setup and teardown.

### Exercise 3: Implement Mock Services
Create mock implementations of `FileService` and `FolderService` to test `FileSystemService` in isolation.

### Exercise 4: Test Edge Cases
Write tests for edge cases, such as:
- Creating files with invalid names
- Navigating beyond the root directory
- Creating files with duplicate names

### Exercise 5: Test-Driven Development
Use TDD to add a new feature, such as:
- File/folder renaming functionality
- File search by name or pattern
- File system statistics (number of files, total size, etc.)

## Real-world Application

Consider updating these specific files in your codebase:

1. `include/services/FileSystemService.h`: Add dependency injection to make testing easier
2. `src/storage/Storage.cpp`: Separate business logic from I/O operations
3. New file `tests/FileTest.cpp`: Create unit tests for the File class

## Conclusion

Testing is an essential practice for ensuring code quality and maintainability. By implementing a comprehensive testing strategy that includes unit, integration, and system tests, you can build confidence in your File System Simulator's correctness and make future improvements easier.

Remember these key points:
- Write tests at multiple levels (unit, integration, system)
- Use appropriate testing frameworks for C++
- Follow Test-Driven Development when possible
- Make code testable through dependency injection and interface-based design
- Separate business logic from I/O for easier testing
- Use mocks and test fixtures to simplify test code