// testing-example.cpp
// Example test suite for the File System Simulator using Google Test

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>

// Mock versions of our classes for testing
// In a real project, you would include the actual headers

// ===== MOCK CLASSES =====

// Basic File class simplified for testing
class File {
private:
    std::string id;
    std::string name;
    std::string extension;
    std::string content;
    std::string folderId;

public:
    File(std::string id, std::string fileName, std::string folderId) 
        : id(std::move(id)), folderId(std::move(folderId)) 
    {
        size_t dotPos = fileName.find_last_of('.');
        if (dotPos != std::string::npos) {
            name = fileName.substr(0, dotPos);
            extension = fileName.substr(dotPos + 1);
        } else {
            name = fileName;
            extension = "";
        }
    }

    void setContent(const std::string& newContent) { content = newContent; }
    std::string getContent() const { return content; }
    std::string getId() const { return id; }
    std::string getFileName() const { 
        return extension.empty() ? name : name + "." + extension; 
    }
    std::string getFolderId() const { return folderId; }
    std::string getName() const { return name; }
    std::string getExtension() const { return extension; }
};

// Basic Folder class simplified for testing
class Folder {
private:
    std::string id;
    std::string name;
    std::string parentId;

public:
    Folder(std::string id, std::string name, std::string parentId)
        : id(std::move(id)), name(std::move(name)), parentId(std::move(parentId)) {}

    std::string getParentId() const { return parentId; }
    std::string getName() const { return name; }
    std::string getId() const { return id; }
};

// Define interfaces for services to enable mocking
class FileServiceInterface {
public:
    virtual ~FileServiceInterface() = default;
    virtual bool createFile(const std::string& folderId, const std::string& fileName) = 0;
    virtual bool removeFile(const std::string& fileName) = 0;
    virtual std::string getFileContent(const std::string& fileName) = 0;
    virtual bool addContent(const std::string& fileName, const std::string& content) = 0;
};

class FolderServiceInterface {
public:
    virtual ~FolderServiceInterface() = default;
    virtual bool createFolder(const std::string& parentId, const std::string& name) = 0;
    virtual bool removeFolder(const std::string& name) = 0;
    virtual std::string getCurrentFolder() const = 0;
    virtual bool getIntoFolder(const std::string& name) = 0;
};

// Mock implementation of FileServiceInterface using Google Mock
class MockFileService : public FileServiceInterface {
public:
    MOCK_METHOD(bool, createFile, (const std::string& folderId, const std::string& fileName), (override));
    MOCK_METHOD(bool, removeFile, (const std::string& fileName), (override));
    MOCK_METHOD(std::string, getFileContent, (const std::string& fileName), (override));
    MOCK_METHOD(bool, addContent, (const std::string& fileName, const std::string& content), (override));
};

// Mock implementation of FolderServiceInterface using Google Mock
class MockFolderService : public FolderServiceInterface {
public:
    MOCK_METHOD(bool, createFolder, (const std::string& parentId, const std::string& name), (override));
    MOCK_METHOD(bool, removeFolder, (const std::string& name), (override));
    MOCK_METHOD(std::string, getCurrentFolder, (), (const, override));
    MOCK_METHOD(bool, getIntoFolder, (const std::string& name), (override));
};

// FileSystemService class that we'll test with mocks
class FileSystemService {
private:
    std::unique_ptr<FileServiceInterface> fileService;
    std::unique_ptr<FolderServiceInterface> folderService;

public:
    FileSystemService(
        std::unique_ptr<FileServiceInterface> fileService,
        std::unique_ptr<FolderServiceInterface> folderService)
        : fileService(std::move(fileService)),
          folderService(std::move(folderService)) {}

    bool createFile(const std::string& fileName) {
        std::string currentFolder = folderService->getCurrentFolder();
        return fileService->createFile(currentFolder, fileName);
    }

    bool removeFile(const std::string& fileName) {
        return fileService->removeFile(fileName);
    }

    bool addContent(const std::string& fileName, const std::string& content) {
        return fileService->addContent(fileName, content);
    }

    bool createFolder(const std::string& folderName) {
        std::string currentFolder = folderService->getCurrentFolder();
        return folderService->createFolder(currentFolder, folderName);
    }

    bool navigateToFolder(const std::string& folderName) {
        return folderService->getIntoFolder(folderName);
    }
};

// ===== UNIT TESTS =====

// Test suite for the File class
class FileTest : public ::testing::Test {
protected:
    // Setup method runs before each test
    void SetUp() override {
        // Create test files
        testFile1 = std::make_unique<File>("f1", "document.txt", "F1");
        testFile2 = std::make_unique<File>("f2", "image.jpg", "F1");
        testFile3 = std::make_unique<File>("f3", "README", "F2"); // No extension
    }

    // Cleanup method runs after each test
    void TearDown() override {
        testFile1.reset();
        testFile2.reset();
        testFile3.reset();
    }

    std::unique_ptr<File> testFile1;
    std::unique_ptr<File> testFile2;
    std::unique_ptr<File> testFile3;
};

// Test the File constructor correctly parses filenames
TEST_F(FileTest, ConstructorParsesFilenamesCorrectly) {
    // Check file with extension
    EXPECT_EQ(testFile1->getName(), "document");
    EXPECT_EQ(testFile1->getExtension(), "txt");
    EXPECT_EQ(testFile1->getFileName(), "document.txt");
    
    // Check another file with different extension
    EXPECT_EQ(testFile2->getName(), "image");
    EXPECT_EQ(testFile2->getExtension(), "jpg");
    EXPECT_EQ(testFile2->getFileName(), "image.jpg");
    
    // Check file without extension
    EXPECT_EQ(testFile3->getName(), "README");
    EXPECT_EQ(testFile3->getExtension(), "");
    EXPECT_EQ(testFile3->getFileName(), "README");
}

// Test content management
TEST_F(FileTest, ContentManagement) {
    // Initial content should be empty
    EXPECT_EQ(testFile1->getContent(), "");
    
    // Set and verify content
    testFile1->setContent("This is a test document.");
    EXPECT_EQ(testFile1->getContent(), "This is a test document.");
    
    // Update content
    testFile1->setContent("Updated content");
    EXPECT_EQ(testFile1->getContent(), "Updated content");
}

// Test folder properties
TEST_F(FileTest, FolderAssociation) {
    EXPECT_EQ(testFile1->getFolderId(), "F1");
    EXPECT_EQ(testFile3->getFolderId(), "F2");
}

// Test suite for the Folder class
class FolderTest : public ::testing::Test {
protected:
    void SetUp() override {
        rootFolder = std::make_unique<Folder>("F0", "root", "");
        documentsFolder = std::make_unique<Folder>("F1", "Documents", "F0");
        picturesFolder = std::make_unique<Folder>("F2", "Pictures", "F0");
    }

    std::unique_ptr<Folder> rootFolder;
    std::unique_ptr<Folder> documentsFolder;
    std::unique_ptr<Folder> picturesFolder;
};

// Test folder properties
TEST_F(FolderTest, FolderProperties) {
    EXPECT_EQ(rootFolder->getName(), "root");
    EXPECT_EQ(rootFolder->getId(), "F0");
    EXPECT_EQ(rootFolder->getParentId(), "");
    
    EXPECT_EQ(documentsFolder->getName(), "Documents");
    EXPECT_EQ(documentsFolder->getId(), "F1");
    EXPECT_EQ(documentsFolder->getParentId(), "F0");
    
    EXPECT_EQ(picturesFolder->getName(), "Pictures");
    EXPECT_EQ(picturesFolder->getId(), "F2");
    EXPECT_EQ(picturesFolder->getParentId(), "F0");
}

// ===== INTEGRATION TESTS WITH MOCKS =====

class FileSystemServiceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create mock services
        mockFileService = std::make_unique<MockFileService>();
        mockFolderService = std::make_unique<MockFolderService>();
        
        // Create file system service with mock dependencies
        fileSystemService = std::make_unique<FileSystemService>(
            std::move(mockFileService),
            std::move(mockFolderService)
        );
        
        // Reset the mocks (need to re-acquire pointers after move)
        mockFileService = std::make_unique<MockFileService>();
        mockFolderService = std::make_unique<MockFolderService>();
    }
    
    std::unique_ptr<MockFileService> mockFileService;
    std::unique_ptr<MockFolderService> mockFolderService;
    std::unique_ptr<FileSystemService> fileSystemService;
};

// Test file creation through FileSystemService
TEST_F(FileSystemServiceTest, CreateFileCallsFileService) {
    // Set up expectations
    EXPECT_CALL(*mockFolderService, getCurrentFolder())
        .WillOnce(testing::Return("F1"));
        
    EXPECT_CALL(*mockFileService, createFile("F1", "test.txt"))
        .WillOnce(testing::Return(true));
    
    // Create FileSystemService with our mocks
    fileSystemService = std::make_unique<FileSystemService>(
        std::unique_ptr<FileServiceInterface>(mockFileService.release()),
        std::unique_ptr<FolderServiceInterface>(mockFolderService.release())
    );
    
    // Call the method under test
    bool result = fileSystemService->createFile("test.txt");
    
    // Verify the result
    EXPECT_TRUE(result);
    
    // The mock expectations are automatically verified when the test exits
}

// Test folder navigation
TEST(FileSystemServiceNavigationTest, NavigateToFolderCallsFolderService) {
    // Create mocks
    auto mockFileService = std::make_unique<MockFileService>();
    auto mockFolderService = std::make_unique<MockFolderService>();
    
    // Set up expectations
    EXPECT_CALL(*mockFolderService, getIntoFolder("documents"))
        .WillOnce(testing::Return(true));
    
    // Create service with mocks
    FileSystemService service(
        std::move(mockFileService),
        std::move(mockFolderService)
    );
    
    // Call the method and verify
    bool result = service.navigateToFolder("documents");
    EXPECT_TRUE(result);
}

// Test file content addition
TEST(FileSystemServiceContentTest, AddContentCallsFileService) {
    // Create mocks
    auto mockFileService = std::make_unique<MockFileService>();
    auto mockFolderService = std::make_unique<MockFolderService>();
    
    // Set up expectations
    EXPECT_CALL(*mockFileService, addContent("notes.txt", "This is a test"))
        .WillOnce(testing::Return(true));
    
    // Create service with mocks
    FileSystemService service(
        std::move(mockFileService),
        std::move(mockFolderService)
    );
    
    // Call the method and verify
    bool result = service.addContent("notes.txt", "This is a test");
    EXPECT_TRUE(result);
}

// ===== PARAMETERIZED TESTS =====

// Example of a parameterized test for File class
class FileNameParsingTest : public ::testing::TestWithParam<std::tuple<std::string, std::string, std::string>> {
    // Test fixture using parameters
};

TEST_P(FileNameParsingTest, ParsesFileNameCorrectly) {
    // Extract test parameters
    std::string fileName = std::get<0>(GetParam());
    std::string expectedName = std::get<1>(GetParam());
    std::string expectedExtension = std::get<2>(GetParam());
    
    // Create file with the test filename
    File file("f1", fileName, "F1");
    
    // Verify parsing
    EXPECT_EQ(file.getName(), expectedName);
    EXPECT_EQ(file.getExtension(), expectedExtension);
}

// Define test cases
INSTANTIATE_TEST_SUITE_P(
    FileNameParsing,
    FileNameParsingTest,
    ::testing::Values(
        std::make_tuple("document.txt", "document", "txt"),
        std::make_tuple("image.jpeg", "image", "jpeg"),
        std::make_tuple("README", "README", ""),
        std::make_tuple("script.js", "script", "js"),
        std::make_tuple(".gitignore", "", "gitignore"),
        std::make_tuple("file.with.dots.txt", "file.with.dots", "txt")
    )
);

// ===== MAIN FUNCTION =====

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}