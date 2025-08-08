const express = require('express');
const router = express.Router();
const FileSystemService = require('../services/FileSystemService');

const fileSystemService = new FileSystemService();

/**
 * @route   GET /api/navigation/path
 * @desc    Get current path
 * @access  Public
 */
router.get('/path', (req, res, next) => {
  try {
    const path = fileSystemService.currentPath();
    
    res.json({
      success: true,
      data: {
        path
      }
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   POST /api/navigation/cd
 * @desc    Change directory
 * @access  Public
 */
router.post('/cd', (req, res, next) => {
  try {
    const { folderName } = req.body;
    
    if (!folderName) {
      return res.status(400).json({
        success: false,
        error: {
          message: 'Folder name is required',
          code: 'MISSING_FOLDER_NAME'
        }
      });
    }
    
    const success = fileSystemService.getIntoFolder(folderName);
    
    if (!success) {
      return res.status(404).json({
        success: false,
        error: {
          message: `Folder '${folderName}' not found or cannot navigate to it`,
          code: 'NAVIGATION_FAILED'
        }
      });
    }
    
    const newPath = fileSystemService.currentPath();
    
    res.json({
      success: true,
      data: {
        path: newPath
      }
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   GET /api/navigation/tree
 * @desc    Get file system tree structure
 * @access  Public
 */
router.get('/tree', (req, res, next) => {
  try {
    const folderId = fileSystemService.getCurrentFolder();
    const tree = fileSystemService.showTree(folderId);
    
    res.json({
      success: true,
      data: tree
    });
  } catch (err) {
    next(err);
  }
});

module.exports = router;