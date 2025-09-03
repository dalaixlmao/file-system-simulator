const express = require('express');
const cors = require('cors');
const morgan = require('morgan');

const fileRoutes = require('./routes/fileRoutes');
const folderRoutes = require('./routes/folderRoutes');
const navigationRoutes = require('./routes/navigationRoutes');
const errorHandler = require('./middleware/errorHandler');
const logger = require('./middleware/logger');

// Initialize express app
const app = express();

// Load middleware
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(morgan('dev'));
app.use(logger);

// API routes
app.use('/api/files', fileRoutes);
app.use('/api/folders', folderRoutes);
app.use('/api/navigation', navigationRoutes);

// Welcome route
app.get('/', (req, res) => {
  res.json({
    message: 'Welcome to File System Simulator API',
    version: '1.0.0',
    endpoints: {
      files: '/api/files',
      folders: '/api/folders',
      navigation: '/api/navigation'
    }
  });
});

// 404 handler
app.use((req, res) => {
  res.status(404).json({
    success: false,
    error: {
      message: `Route not found: ${req.originalUrl}`,
      code: 'ROUTE_NOT_FOUND'
    }
  });
});

// Error handler middleware
app.use(errorHandler);

// Server config
const PORT = process.env.PORT || 3000;

// Start server
if (require.main === module) {
  app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
    console.log(`API available at http://localhost:${PORT}`);
  });
}

module.exports = app;