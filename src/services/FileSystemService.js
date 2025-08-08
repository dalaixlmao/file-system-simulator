const Storage = require('../storage/Storage');
const FileService = require('./FileService');
const FolderService = require('./FolderService');

/**
 * FileSystemService integrates file and folder services
 * Migrated from C++ FileSystemService.h/FileSystemService.cpp
 */
class FileSystemService {
  constructor() {
    this.store = Storage.getInstance();
    this.fileService = new FileService();
    this.folderService = new FolderService();
  }

  /**
   * Create a file in the specified folder
   * @param {string} folderId - Folder ID where the file will be created
   * @param {string} fileName - Name of the file
   * @returns {Object} - Created file or null if creation failed
   */
  createFile(folderId, fileName) {
    return this.fileService.createFile(folderId, fileName);
  }

  /**
   * Get current folder ID
   * @returns {string} - Current folder ID
   */
  getCurrentFolder() {
    return this.folderService.getCurrentFolder();
  }

  /**
   * Add content to a file
   * @param {string} fileName - Name of the file
   * @param {string} content - Content to add
   */
  addContent(fileName, content) {
    this.fileService.addContent(fileName, content);
  }

  /**
   * Remove a file by name
   * @param {string} fileName - Name of the file to remove
   * @returns {boolean} - True if removal was successful
   */
  removeFile(fileName) {
    return this.fileService.removeFile(fileName);
  }

  /**
   * Get file content
   * @param {string} fileId - ID of the file
   * @returns {string} - Content of the file
   */
  showFileContent(fileId) {
    return this.fileService.showFileContent(fileId);
  }

  /**
   * Create a folder in the specified parent folder
   * @param {string} parentFolderId - Parent folder ID
   * @param {string} folderName - Name of the folder to create
   * @returns {Object} - Created folder or null if creation failed
   */
  createFolder(parentFolderId, folderName) {
    return this.folderService.createFolder(parentFolderId, folderName);
  }

  /**
   * Remove a folder by name
   * @param {string} folderName - Name of the folder to remove
   * @returns {boolean} - True if removal was successful
   */
  removeFolder(folderName) {
    return this.folderService.removeFolder(folderName);
  }

  /**
   * Get folder tree starting from specified folder
   * @param {string} folderId - Starting folder ID
   * @returns {Object} - Tree structure
   */
  showTree(folderId) {
    return this.folderService.showTree(folderId);
  }

  /**
   * List all items in a folder
   * @param {string} folderId - Folder ID
   * @returns {Array} - Array of items in the folder
   */
  listAllItems(folderId) {
    return this.folderService.listAllItems(folderId);
  }

  /**
   * Navigate into a folder
   * @param {string} folderName - Folder name
   * @returns {boolean} - True if navigation was successful
   */
  getIntoFolder(folderName) {
    return this.folderService.getIntoFolder(folderName);
  }

  /**
   * Check if a folder exists in current directory
   * @param {string} name - Name of the folder
   * @returns {boolean} - True if folder exists
   */
  isFolderAvailable(name) {
    return this.folderService.validateFolder(name);
  }

  /**
   * Get current path as a string
   * @returns {string} - Current path
   */
  currentPath() {
    return this.store.getPath(this.folderService.getCurrentFolder());
  }
  
  /**
   * Get file by name
   * @param {string} fileName - Name of the file
   * @returns {Object|null} - File details or null if not found
   */
  getFileByName(fileName) {
    return this.fileService.getFileByName(fileName);
  }
  
  /**
   * Get folder by name
   * @param {string} folderName - Name of the folder
   * @returns {Object|null} - Folder details or null if not found
   */
  getFolderByName(folderName) {
    return this.folderService.getFolderByName(folderName);
  }
}

module.exports = FileSystemService;