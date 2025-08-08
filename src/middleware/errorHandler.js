/**
 * Global error handling middleware
 * @param {Error} err - Error object
 * @param {Object} req - Express request object
 * @param {Object} res - Express response object
 * @param {Function} next - Express next middleware function
 */
const errorHandler = (err, req, res, next) => {
  console.error('Error:', err.stack);
  
  // Default to 500 internal server error
  const statusCode = err.statusCode || 500;
  
  res.status(statusCode).json({
    success: false,
    error: {
      message: err.message || 'Internal server error',
      code: err.code || 'INTERNAL_ERROR'
    }
  });
};

module.exports = errorHandler;