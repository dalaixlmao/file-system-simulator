const Storage = require('../storage/Storage');

/**
 * FolderService manages all folder-related operations
 * Migrated from C++ FolderService.h/FolderService.cpp
 */
class FolderService {
  constructor() {
    this.store = Storage.getInstance();
  }

  /**
   * Create a folder in the specified parent folder
   * @param {string} parentFolderId - Parent folder ID
   * @param {string} folderName - Name of the folder to create
   * @returns {Object} - Created folder or null if creation failed
   */
  createFolder(parentFolderId, folderName) {
    return this.store.addFolder(folderName, parentFolderId);
  }

  /**
   * Remove a folder by name
   * @param {string} folderName - Name of the folder to remove
   * @returns {boolean} - True if removal was successful
   */
  removeFolder(folderName) {
    return this.store.removeFolder(folderName);
  }

  /**
   * Get folder tree starting from specified folder
   * @param {string} folderId - Starting folder ID
   * @returns {Object} - Tree structure
   */
  showTree(folderId) {
    return this.store.getFolderTree();
  }

  /**
   * List all items in a folder
   * @param {string} folderId - Folder ID
   * @returns {Array} - Array of items in the folder
   */
  listAllItems(folderId) {
    return this.store.showItemsInFolder(folderId);
  }

  /**
   * Show folder path
   * @param {string} folderId - Folder ID
   * @returns {string} - Full path
   */
  showFolderPath(folderId) {
    return this.store.showFolderPath(folderId);
  }

  /**
   * Get current folder ID
   * @returns {string} - Current folder ID
   */
  getCurrentFolder() {
    return this.store.getCurrentFolderId();
  }

  /**
   * Navigate into a folder
   * @param {string} folderName - Folder name
   * @returns {boolean} - True if navigation was successful
   */
  getIntoFolder(folderName) {
    return this.store.getIntoFolder(folderName);
  }
  
  /**
   * Check if a folder exists in current directory
   * @param {string} folderName - Name of the folder
   * @returns {boolean} - True if folder exists
   */
  validateFolder(folderName) {
    return this.store.validateFolder(folderName);
  }
  
  /**
   * Get folder details by name
   * @param {string} folderName - Name of the folder
   * @returns {Object|null} - Folder details or null if not found
   */
  getFolderByName(folderName) {
    const currentFolderId = this.store.getCurrentFolderId();
    const items = this.store.showItemsInFolder(currentFolderId);
    
    const folderItem = items.find(item => 
      item.type === 'folder' && item.name === folderName
    );
    
    if (folderItem) {
      const folder = this.store.getFolder(folderItem.id);
      if (folder) {
        return {
          id: folder.getId(),
          name: folder.getName(),
          parentId: folder.getParentId()
        };
      }
    }
    
    return null;
  }
}

module.exports = FolderService;