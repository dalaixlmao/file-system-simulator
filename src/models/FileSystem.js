/**
 * FileSystem class manages the current path in the file system
 * Migrated from C++ FileSystem.h/FileSystem.cpp
 */
class FileSystem {
  /**
   * Create a new file system instance
   */
  constructor() {
    this.path = []; // Stack to track current path
  }

  /**
   * Add folder ID to the path stack
   * @param {string} id - Folder ID to add to the path
   */
  addFolderId(id) {
    this.path.push(id);
  }

  /**
   * Remove the current folder from the path
   */
  removeCurrentFolder() {
    if (!this.checkEmpty()) {
      this.path.pop();
    }
  }

  /**
   * Get the current folder ID
   * @returns {string} - Current folder ID
   */
  getCurrentFolder() {
    if (!this.checkEmpty()) {
      return this.path[this.path.length - 1];
    }
    return null;
  }

  /**
   * Check if the path stack is empty
   * @returns {boolean} - True if empty, false otherwise
   */
  checkEmpty() {
    return this.path.length === 0;
  }
}

module.exports = FileSystem;