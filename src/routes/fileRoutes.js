const express = require('express');
const router = express.Router();
const FileSystemService = require('../services/FileSystemService');

const fileSystemService = new FileSystemService();

/**
 * @route   POST /api/files
 * @desc    Create a new file
 * @access  Public
 */
router.post('/', (req, res, next) => {
  try {
    const { name } = req.body;
    
    if (!name) {
      return res.status(400).json({
        success: false,
        error: {
          message: 'File name is required',
          code: 'MISSING_NAME'
        }
      });
    }
    
    const folderId = fileSystemService.getCurrentFolder();
    const file = fileSystemService.createFile(folderId, name);
    
    if (!file) {
      return res.status(400).json({
        success: false,
        error: {
          message: 'Failed to create file, it may already exist',
          code: 'CREATE_FAILED'
        }
      });
    }
    
    res.status(201).json({
      success: true,
      data: {
        id: file.getId(),
        name: file.getFileName(),
        path: fileSystemService.currentPath() + '/' + file.getFileName()
      }
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   GET /api/files
 * @desc    Get all files in current folder
 * @access  Public
 */
router.get('/', (req, res, next) => {
  try {
    const folderId = fileSystemService.getCurrentFolder();
    const items = fileSystemService.listAllItems(folderId);
    
    const files = items.filter(item => item.type === 'file').map(item => ({
      id: item.id,
      name: item.name,
      path: fileSystemService.currentPath() + '/' + item.name
    }));
    
    res.json({
      success: true,
      data: files
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   GET /api/files/:name
 * @desc    Get file details
 * @access  Public
 */
router.get('/:name', (req, res, next) => {
  try {
    const fileName = req.params.name;
    const file = fileSystemService.getFileByName(fileName);
    
    if (!file) {
      return res.status(404).json({
        success: false,
        error: {
          message: `File '${fileName}' not found in current directory`,
          code: 'FILE_NOT_FOUND'
        }
      });
    }
    
    res.json({
      success: true,
      data: {
        id: file.id,
        name: file.name,
        content: file.content,
        path: fileSystemService.currentPath() + '/' + file.name
      }
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   PUT /api/files/:name/content
 * @desc    Write content to a file
 * @access  Public
 */
router.put('/:name/content', (req, res, next) => {
  try {
    const { content } = req.body;
    const fileName = req.params.name;
    
    if (content === undefined) {
      return res.status(400).json({
        success: false,
        error: {
          message: 'Content is required',
          code: 'MISSING_CONTENT'
        }
      });
    }
    
    if (!fileSystemService.fileService.validateFile(fileName)) {
      return res.status(404).json({
        success: false,
        error: {
          message: `File '${fileName}' not found in current directory`,
          code: 'FILE_NOT_FOUND'
        }
      });
    }
    
    fileSystemService.addContent(fileName, content);
    
    const updatedFile = fileSystemService.getFileByName(fileName);
    
    res.json({
      success: true,
      data: {
        id: updatedFile.id,
        name: updatedFile.name,
        content: updatedFile.content,
        path: fileSystemService.currentPath() + '/' + updatedFile.name
      }
    });
  } catch (err) {
    next(err);
  }
});

/**
 * @route   DELETE /api/files/:name
 * @desc    Delete a file
 * @access  Public
 */
router.delete('/:name', (req, res, next) => {
  try {
    const fileName = req.params.name;
    
    if (!fileSystemService.fileService.validateFile(fileName)) {
      return res.status(404).json({
        success: false,
        error: {
          message: `File '${fileName}' not found in current directory`,
          code: 'FILE_NOT_FOUND'
        }
      });
    }
    
    const success = fileSystemService.removeFile(fileName);
    
    if (!success) {
      return res.status(500).json({
        success: false,
        error: {
          message: `Failed to delete file '${fileName}'`,
          code: 'DELETE_FAILED'
        }
      });
    }
    
    res.json({
      success: true,
      data: {
        message: `File '${fileName}' successfully deleted`
      }
    });
  } catch (err) {
    next(err);
  }
});

module.exports = router;