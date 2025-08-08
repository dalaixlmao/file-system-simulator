/**
 * Folder class represents a directory in the file system
 * Migrated from C++ Folder.h/Folder.cpp
 */
class Folder {
  /**
   * Create a new folder
   * @param {string} id - Unique identifier for the folder
   * @param {string} name - Name of the folder
   * @param {string} folderId - ID of the parent folder
   */
  constructor(id, name, folderId) {
    this.id = id;
    this.name = name;
    this.folderId = folderId;
  }

  /**
   * Get the parent folder ID
   * @returns {string} - Parent folder ID
   */
  getParentId() {
    return this.folderId;
  }

  /**
   * Get the folder name
   * @returns {string} - Folder name
   */
  getName() {
    return this.name;
  }

  /**
   * Get the folder ID
   * @returns {string} - Folder ID
   */
  getId() {
    return this.id;
  }
}

module.exports = Folder;