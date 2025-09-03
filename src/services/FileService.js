const Storage = require('../storage/Storage');

/**
 * FileService manages all file-related operations
 * Migrated from C++ FileService.h/FileService.cpp
 */
class FileService {
  constructor() {
    this.store = Storage.getInstance();
  }

  /**
   * Create a file in the specified folder
   * @param {string} folderId - Folder ID where the file will be created
   * @param {string} fileName - Name of the file
   * @returns {Object} - Created file or null if creation failed
   */
  createFile(folderId, fileName) {
    return this.store.addFile(fileName, folderId);
  }

  /**
   * Add content to a file
   * @param {string} fileName - Name of the file
   * @param {string} content - Content to add
   */
  addContent(fileName, content) {
    this.store.addContent(fileName, content);
  }

  /**
   * Remove a file by name
   * @param {string} filename - Name of the file to remove
   * @returns {boolean} - True if removal was successful
   */
  removeFile(filename) {
    return this.store.removeFile(filename);
  }

  /**
   * Get file content
   * @param {string} fileId - ID of the file
   * @returns {string} - Content of the file
   */
  showFileContent(fileId) {
    const file = this.store.getFile(fileId);
    return file ? file.getContent() : '';
  }

  /**
   * Get file path
   * @param {string} fileId - ID of the file
   * @returns {string} - Path to the file
   */
  showFilePath(fileId) {
    return this.store.showFilePath(fileId);
  }
  
  /**
   * Check if a file exists in current folder
   * @param {string} fileName - Name of the file
   * @returns {boolean} - True if file exists
   */
  validateFile(fileName) {
    return this.store.validateFile(fileName);
  }
  
  /**
   * Get file details by name
   * @param {string} fileName - Name of the file
   * @returns {Object|null} - File details or null if not found
   */
  getFileByName(fileName) {
    const currentFolderId = this.store.getCurrentFolderId();
    const items = this.store.showItemsInFolder(currentFolderId);
    
    const fileItem = items.find(item => 
      item.type === 'file' && item.name === fileName
    );
    
    if (fileItem) {
      const file = this.store.getFile(fileItem.id);
      if (file) {
        return {
          id: file.getId(),
          name: file.getFileName(),
          content: file.getContent(),
          folderId: file.getFolderId()
        };
      }
    }
    
    return null;
  }
}

module.exports = FileService;