/**
 * File class represents a file in the file system
 * Migrated from C++ File.h/File.cpp
 */
class File {
  /**
   * Create a new file
   * @param {string} id - Unique identifier for the file
   * @param {string} fileName - Name of the file with extension
   * @param {string} folderId - ID of the parent folder
   */
  constructor(id, fileName, folderId) {
    this.id = id;
    this.folderId = folderId;
    this.content = '';
    
    // Parse file name and extension
    const lastDotIndex = fileName.lastIndexOf('.');
    if (lastDotIndex > -1) {
      this.name = fileName.substring(0, lastDotIndex);
      this.extension = fileName.substring(lastDotIndex + 1);
    } else {
      this.name = fileName;
      this.extension = '';
    }
  }

  /**
   * Set the content of the file
   * @param {string} content - Content to be set
   */
  setContent(content) {
    this.content = content;
  }

  /**
   * Get the content of the file
   * @returns {string} - File content
   */
  getContent() {
    return this.content;
  }

  /**
   * Get the file name with extension
   * @returns {string} - File name with extension
   */
  getFileName() {
    if (this.extension) {
      return `${this.name}.${this.extension}`;
    }
    return this.name;
  }

  /**
   * Get the parent folder ID
   * @returns {string} - Parent folder ID
   */
  getFolderId() {
    return this.folderId;
  }

  /**
   * Get the file ID
   * @returns {string} - File ID
   */
  getId() {
    return this.id;
  }
}

module.exports = File;