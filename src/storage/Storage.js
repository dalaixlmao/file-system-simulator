const FileSystem = require('../models/FileSystem');
const File = require('../models/File');
const Folder = require('../models/Folder');

/**
 * Storage class manages the in-memory file system data
 * Migrated from C++ Storage.h/Storage.cpp
 */
class Storage {
  constructor() {
    this.tree = new Map();
    this.fileSystem = new FileSystem();
    this.folders = new Map();
    this.files = new Map();
    
    // Initialize root folder
    this.fileSystem.addFolderId('F0');
    this.folders.set('F0', null);
    this.tree.set('F0', new Map());
    
    // Create base folder
    const baseFolder = new Folder('F1', 'BaseFolder', 'F0');
    this.fileSystem.addFolderId('F1');
    this.folders.set(baseFolder.getId(), baseFolder);
    this.tree.get('F0').set('F1', 1);
  }
  
  /**
   * Add content to a file
   * @param {string} fileName - Name of the file
   * @param {string} content - Content to add
   */
  addContent(fileName, content) {
    const currentFolderId = this.getCurrentFolderId();
    if (this.validateFile(fileName)) {
      const treeFolder = this.tree.get(currentFolderId);
      for (const [key, value] of treeFolder.entries()) {
        if (key[0] === 'f' && this.files.get(key).getFileName() === fileName) {
          this.files.get(key).setContent(content);
        }
      }
    }
  }
  
  /**
   * Generate a new file ID
   * @returns {string} - New file ID
   */
  getNewFileId() {
    return `f${this.files.size}`;
  }
  
  /**
   * Add a file to the file system
   * @param {string} name - File name
   * @param {string} folderId - Parent folder ID
   */
  addFile(name, folderId) {
    const treeFolder = this.tree.get(folderId);
    if (!treeFolder) return;
    
    // Check if file name already exists in folder
    for (const [key, value] of treeFolder.entries()) {
      if (key[0] === 'f' && this.files.get(key).getFileName() === name) {
        console.log("File name already exists! Change the name of the file.");
        return;
      }
    }
    
    // Create new file
    const newFileId = this.getNewFileId();
    const file = new File(newFileId, name, folderId);
    this.files.set(newFileId, file);
    treeFolder.set(newFileId, 1);
    
    return file;
  }
  
  /**
   * Generate a new folder ID
   * @returns {string} - New folder ID
   */
  getNewFolderId() {
    return `F${this.folders.size}`;
  }
  
  /**
   * Add a folder to the file system
   * @param {string} name - Folder name
   * @param {string} parentFolderId - Parent folder ID
   */
  addFolder(name, parentFolderId) {
    const treeFolder = this.tree.get(parentFolderId);
    if (!treeFolder) return;
    
    // Check if folder name already exists in parent folder
    for (const [key, value] of treeFolder.entries()) {
      if (key[0] === 'F' && this.folders.get(key).getName() === name) {
        console.log("Folder name already exists! Change the name of the folder.");
        return;
      }
    }
    
    // Create new folder
    const newFolderId = this.getNewFolderId();
    const folder = new Folder(newFolderId, name, parentFolderId);
    this.folders.set(newFolderId, folder);
    treeFolder.set(newFolderId, 1);
    this.tree.set(newFolderId, new Map());
    
    return folder;
  }
  
  /**
   * Get a folder by ID
   * @param {string} id - Folder ID
   * @returns {Folder} - Folder object
   */
  getFolder(id) {
    return this.folders.get(id);
  }
  
  /**
   * Get a file by ID
   * @param {string} id - File ID
   * @returns {File} - File object
   */
  getFile(id) {
    return this.files.get(id);
  }
  
  /**
   * Get the full path for a folder
   * @param {string} id - Folder ID
   * @returns {string} - Full path
   */
  getPath(id) {
    let folder = this.folders.get(id);
    let path = '';
    
    while (folder && folder.getParentId() !== 'F0') {
      path = '/' + path;
      path = folder.getName() + path;
      folder = this.folders.get(folder.getParentId());
    }
    
    return path || '/';
  }
  
  /**
   * Show folder path
   * @param {string} id - Folder ID
   * @returns {string} - Full path
   */
  showFolderPath(id) {
    return this.getPath(id);
  }
  
  /**
   * Show file path
   * @param {string} id - File ID
   * @returns {string} - Full path
   */
  showFilePath(id) {
    return this.getPath(id);
  }
  
  /**
   * Get the current folder ID
   * @returns {string} - Current folder ID
   */
  getCurrentFolderId() {
    return this.fileSystem.getCurrentFolder();
  }
  
  /**
   * Show all items in a folder
   * @param {string} folderId - Folder ID
   * @returns {Array} - Array of items in the folder
   */
  showItemsInFolder(folderId) {
    const items = [];
    
    if (this.folders.has(folderId)) {
      const treeFolder = this.tree.get(folderId);
      if (treeFolder) {
        for (const [key, value] of treeFolder.entries()) {
          if (key[0] === 'f') {
            items.push({
              type: 'file',
              name: this.files.get(key).getFileName(),
              id: key
            });
          } else {
            items.push({
              type: 'folder',
              name: this.folders.get(key).getName(),
              id: key
            });
          }
        }
      }
    }
    
    return items;
  }
  
  /**
   * Navigate into a folder
   * @param {string} name - Folder name
   * @returns {boolean} - True if successful, false otherwise
   */
  getIntoFolder(name) {
    const currentFolderId = this.fileSystem.getCurrentFolder();
    
    if (name !== '..') {
      const treeFolder = this.tree.get(currentFolderId);
      if (!treeFolder) return false;
      
      for (const [key, value] of treeFolder.entries()) {
        if (key[0] === 'F' && this.folders.get(key).getName() === name) {
          this.fileSystem.addFolderId(key);
          return true;
        }
      }
    } else if (!this.fileSystem.checkEmpty()) {
      this.fileSystem.removeCurrentFolder();
      return true;
    }
    
    return false;
  }
  
  /**
   * Validate if a folder exists in the current directory
   * @param {string} folderName - Folder name
   * @returns {boolean} - True if folder exists, false otherwise
   */
  validateFolder(folderName) {
    const currentFolderId = this.fileSystem.getCurrentFolder();
    const treeFolder = this.tree.get(currentFolderId);
    if (!treeFolder) return false;
    
    for (const [key, value] of treeFolder.entries()) {
      if (key[0] === 'F' && this.folders.get(key).getName() === folderName) {
        return true;
      }
    }
    
    return false;
  }
  
  /**
   * Remove a file from the file system
   * @param {string} fileName - File name
   * @returns {boolean} - True if successful, false otherwise
   */
  removeFile(fileName) {
    if (this.validateFile(fileName)) {
      const currentFolderId = this.fileSystem.getCurrentFolder();
      const treeFolder = this.tree.get(currentFolderId);
      if (!treeFolder) return false;
      
      for (const [key, value] of treeFolder.entries()) {
        if (key[0] === 'f' && this.files.get(key).getFileName() === fileName) {
          const fileId = this.files.get(key).getId();
          this.files.delete(fileId);
          treeFolder.delete(fileId);
          
          if (treeFolder.size === 0) {
            this.tree.delete(currentFolderId);
          }
          
          return true;
        }
      }
    }
    
    return false;
  }
  
  /**
   * Recursively remove a folder and its contents
   * @param {string} node - Folder ID
   */
  removeDFS(node) {
    const treeFolder = this.tree.get(node);
    if (!treeFolder) return;
    
    const itemsToDelete = Array.from(treeFolder.keys());
    
    for (const key of itemsToDelete) {
      if (key[0] === 'F') {
        this.removeDFS(key);
      } else {
        this.files.delete(key);
      }
    }
    
    this.folders.delete(node);
    this.tree.delete(node);
  }
  
  /**
   * Remove a folder from the file system
   * @param {string} folderName - Folder name
   * @returns {boolean} - True if successful, false otherwise
   */
  removeFolder(folderName) {
    if (this.validateFolder(folderName)) {
      const currentFolderId = this.fileSystem.getCurrentFolder();
      const treeFolder = this.tree.get(currentFolderId);
      if (!treeFolder) return false;
      
      for (const [key, value] of treeFolder.entries()) {
        if (key[0] === 'F' && this.folders.get(key).getName() === folderName) {
          const folderId = this.folders.get(key).getId();
          const parFolderId = this.folders.get(key).getParentId();
          
          const parentFolder = this.tree.get(parFolderId);
          if (parentFolder) {
            parentFolder.delete(folderId);
          }
          
          this.removeDFS(folderId);
          return true;
        }
      }
    }
    
    return false;
  }
  
  /**
   * Recursively build folder tree structure
   * @param {string} node - Starting node
   * @param {Object} result - Result object to build
   * @returns {Object} - Tree structure
   */
  buildDFS(node) {
    let result;
    
    if (node[0] === 'F') {
      const folder = this.folders.get(node);
      if (!folder) return null;
      
      result = {
        id: node,
        name: folder.getName(),
        type: 'folder',
        children: []
      };
      
      const treeFolder = this.tree.get(node);
      if (treeFolder) {
        for (const [key, value] of treeFolder.entries()) {
          const childNode = this.buildDFS(key);
          if (childNode) {
            result.children.push(childNode);
          }
        }
      }
    } else {
      const file = this.files.get(node);
      if (!file) return null;
      
      result = {
        id: node,
        name: file.getFileName(),
        type: 'file'
      };
    }
    
    return result;
  }
  
  /**
   * Get folder tree structure
   * @returns {Object} - Tree structure
   */
  getFolderTree() {
    const currentFolderId = this.fileSystem.getCurrentFolder();
    return this.buildDFS(currentFolderId);
  }
  
  /**
   * Validate if a file exists in the current directory
   * @param {string} fileName - File name
   * @returns {boolean} - True if file exists, false otherwise
   */
  validateFile(fileName) {
    const currentFolderId = this.fileSystem.getCurrentFolder();
    const treeFolder = this.tree.get(currentFolderId);
    if (!treeFolder) return false;
    
    for (const [key, value] of treeFolder.entries()) {
      if (key[0] === 'f' && this.files.get(key).getFileName() === fileName) {
        return true;
      }
    }
    
    return false;
  }
}

// Singleton pattern
let instance = null;

module.exports = {
  getInstance: () => {
    if (instance === null) {
      instance = new Storage();
    }
    return instance;
  },
  resetInstance: () => {
    instance = null;
    return module.exports.getInstance();
  }
};