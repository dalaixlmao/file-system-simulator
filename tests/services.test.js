const FileSystemService = require('../src/services/FileSystemService');
const FileService = require('../src/services/FileService');
const FolderService = require('../src/services/FolderService');
const Storage = require('../src/storage/Storage');

describe('File System Services', () => {
  // Reset storage before each test
  beforeEach(() => {
    Storage.resetInstance();
  });

  describe('FileService', () => {
    test('Should create a file', () => {
      const fileService = new FileService();
      const folderId = fileService.store.getCurrentFolderId();
      
      const file = fileService.createFile(folderId, 'test.txt');
      
      expect(file).toBeDefined();
      expect(file.getFileName()).toBe('test.txt');
      expect(file.getFolderId()).toBe(folderId);
    });
    
    test('Should add content to a file', () => {
      const fileService = new FileService();
      const folderId = fileService.store.getCurrentFolderId();
      
      const file = fileService.createFile(folderId, 'test.txt');
      const content = 'Hello, World!';
      
      fileService.addContent('test.txt', content);
      
      expect(fileService.getFileByName('test.txt').content).toBe(content);
    });
    
    test('Should remove a file', () => {
      const fileService = new FileService();
      const folderId = fileService.store.getCurrentFolderId();
      
      fileService.createFile(folderId, 'test.txt');
      
      const result = fileService.removeFile('test.txt');
      
      expect(result).toBe(true);
      expect(fileService.validateFile('test.txt')).toBe(false);
    });
  });

  describe('FolderService', () => {
    test('Should create a folder', () => {
      const folderService = new FolderService();
      const parentFolderId = folderService.store.getCurrentFolderId();
      
      const folder = folderService.createFolder(parentFolderId, 'testFolder');
      
      expect(folder).toBeDefined();
      expect(folder.getName()).toBe('testFolder');
      expect(folder.getParentId()).toBe(parentFolderId);
    });
    
    test('Should navigate into a folder', () => {
      const folderService = new FolderService();
      const parentFolderId = folderService.store.getCurrentFolderId();
      
      folderService.createFolder(parentFolderId, 'testFolder');
      const result = folderService.getIntoFolder('testFolder');
      
      expect(result).toBe(true);
      
      const currentFolder = folderService.getCurrentFolder();
      const folder = folderService.store.getFolder(currentFolder);
      
      expect(folder.getName()).toBe('testFolder');
    });
    
    test('Should navigate back to parent folder', () => {
      const folderService = new FolderService();
      const originalFolderId = folderService.store.getCurrentFolderId();
      
      folderService.createFolder(originalFolderId, 'testFolder');
      folderService.getIntoFolder('testFolder');
      
      // Navigate back
      const result = folderService.getIntoFolder('..');
      
      expect(result).toBe(true);
      expect(folderService.getCurrentFolder()).toBe(originalFolderId);
    });
    
    test('Should remove a folder', () => {
      const folderService = new FolderService();
      const parentFolderId = folderService.store.getCurrentFolderId();
      
      folderService.createFolder(parentFolderId, 'testFolder');
      
      const result = folderService.removeFolder('testFolder');
      
      expect(result).toBe(true);
      expect(folderService.validateFolder('testFolder')).toBe(false);
    });
  });

  describe('FileSystemService', () => {
    test('Should integrate file and folder operations', () => {
      const fileSystemService = new FileSystemService();
      const folderId = fileSystemService.getCurrentFolder();
      
      // Create a folder
      const folder = fileSystemService.createFolder(folderId, 'testFolder');
      expect(folder).toBeDefined();
      
      // Navigate to the folder
      const navResult = fileSystemService.getIntoFolder('testFolder');
      expect(navResult).toBe(true);
      
      // Create a file in the folder
      const newFolderId = fileSystemService.getCurrentFolder();
      const file = fileSystemService.createFile(newFolderId, 'test.txt');
      expect(file).toBeDefined();
      
      // Write content to the file
      fileSystemService.addContent('test.txt', 'Hello from nested folder');
      
      // Get file details
      const fileDetails = fileSystemService.getFileByName('test.txt');
      expect(fileDetails).toBeDefined();
      expect(fileDetails.content).toBe('Hello from nested folder');
      
      // Navigate back to parent
      fileSystemService.getIntoFolder('..');
      
      // Verify current path
      const path = fileSystemService.currentPath();
      expect(path).not.toContain('testFolder');
    });
    
    test('Should build tree structure', () => {
      const fileSystemService = new FileSystemService();
      const folderId = fileSystemService.getCurrentFolder();
      
      // Create files and folders
      fileSystemService.createFile(folderId, 'root.txt');
      fileSystemService.createFolder(folderId, 'folder1');
      fileSystemService.createFolder(folderId, 'folder2');
      
      // Navigate to folder1
      fileSystemService.getIntoFolder('folder1');
      const folder1Id = fileSystemService.getCurrentFolder();
      
      // Create files in folder1
      fileSystemService.createFile(folder1Id, 'file1.txt');
      fileSystemService.createFile(folder1Id, 'file2.txt');
      
      // Navigate back to root
      fileSystemService.getIntoFolder('..');
      
      // Navigate to folder2
      fileSystemService.getIntoFolder('folder2');
      const folder2Id = fileSystemService.getCurrentFolder();
      
      // Create nested folder
      fileSystemService.createFolder(folder2Id, 'nestedFolder');
      
      // Navigate back to root
      fileSystemService.getIntoFolder('..');
      
      // Get tree
      const tree = fileSystemService.showTree(folderId);
      
      // Verify tree structure
      expect(tree).toBeDefined();
      expect(tree.type).toBe('folder');
      expect(tree.children.length).toBe(3); // root.txt, folder1, folder2
      
      const folder1Node = tree.children.find(node => node.name === 'folder1');
      const folder2Node = tree.children.find(node => node.name === 'folder2');
      
      expect(folder1Node).toBeDefined();
      expect(folder2Node).toBeDefined();
      
      expect(folder1Node.children.length).toBe(2); // file1.txt, file2.txt
      expect(folder2Node.children.length).toBe(1); // nestedFolder
      
      const nestedFolderNode = folder2Node.children.find(node => node.name === 'nestedFolder');
      expect(nestedFolderNode).toBeDefined();
    });
  });
});