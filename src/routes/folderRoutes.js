const express = require('express');
const router = express.Router();
const FileSystemService = require('../services/FileSystemService');

const fileSystemService = new FileSystemService();

/**
 * @route   POST /api/folders
 * @desc    Create a new folder
 * @access  Public
 */
router.post('/', (req, res, next) => {
  try {
    const { name } = req.body;
    
    if (!name) {
      return res.status(400).json({
        success: false,
        error: {
          message: 'Folder name is required',
          code: 'MISSING_NAME'
        }
      });
    }
    
    const parentFolderId = fileSystemService.getCurrentFolder();
    const folder = fileSystemService.createFolder(parentFolderId, name);
    
    if (!folder) {
      return res.status(400).json({
        success: false,
        error: {
          message: 'Failed to create folder, it may already exist',
          code: 'CREATE_FAILED'
        }
      });
    }
    
    res.status(201).json({
      success: true,
      data: {
        id: folder.getId(),
        name: folder.getName(),
        path: fileSystemService.currentPath() + '/' + folder.getName()
      }
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   GET /api/folders
 * @desc    Get all folders in current directory
 * @access  Public
 */
router.get('/', (req, res, next) => {
  try {
    const folderId = fileSystemService.getCurrentFolder();
    const items = fileSystemService.listAllItems(folderId);
    
    const folders = items.filter(item => item.type === 'folder').map(item => ({
      id: item.id,
      name: item.name,
      path: fileSystemService.currentPath() + '/' + item.name
    }));
    
    res.json({
      success: true,
      data: folders
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   GET /api/folders/content
 * @desc    List all items (files and folders) in current folder
 * @access  Public
 */
router.get('/content', (req, res, next) => {
  try {
    const folderId = fileSystemService.getCurrentFolder();
    const items = fileSystemService.listAllItems(folderId);
    
    const formattedItems = items.map(item => ({
      id: item.id,
      name: item.name,
      type: item.type,
      path: fileSystemService.currentPath() + '/' + item.name
    }));
    
    res.json({
      success: true,
      path: fileSystemService.currentPath(),
      data: formattedItems
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   GET /api/folders/:name
 * @desc    Get folder details
 * @access  Public
 */
router.get('/:name', (req, res, next) => {
  try {
    const folderName = req.params.name;
    const folder = fileSystemService.getFolderByName(folderName);
    
    if (!folder) {
      return res.status(404).json({
        success: false,
        error: {
          message: `Folder '${folderName}' not found in current directory`,
          code: 'FOLDER_NOT_FOUND'
        }
      });
    }
    
    res.json({
      success: true,
      data: {
        id: folder.id,
        name: folder.name,
        path: fileSystemService.currentPath() + '/' + folder.name
      }
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   DELETE /api/folders/:name
 * @desc    Delete a folder
 * @access  Public
 */
router.delete('/:name', (req, res, next) => {
  try {
    const folderName = req.params.name;
    
    if (!fileSystemService.isFolderAvailable(folderName)) {
      return res.status(404).json({
        success: false,
        error: {
          message: `Folder '${folderName}' not found in current directory`,
          code: 'FOLDER_NOT_FOUND'
        }
      });
    }
    
    const success = fileSystemService.removeFolder(folderName);
    
    if (!success) {
      return res.status(500).json({
        success: false,
        error: {
          message: `Failed to delete folder '${folderName}'`,
          code: 'DELETE_FAILED'
        }
      });
    }
    
    res.json({
      success: true,
      data: {
        message: `Folder '${folderName}' successfully deleted`
      }
    });
  } catch (err) {
    next(err);
  }
});

module.exports = router;