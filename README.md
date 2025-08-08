# File System Simulator REST API

## Overview
This is a Node.js REST API implementation of the File System Simulator, migrated from the original C++ command-line interface. It provides a web API to interact with an in-memory file system, allowing users to perform common file and directory operations.

## Features
* Create and delete files and directories
* Navigate through the file system hierarchy
* List contents of directories
* View file system tree structure
* Supports relative and absolute path navigation
* Implements basic file and folder management operations

## Installation and Usage

### Prerequisites
* Node.js (v14 or later)
* npm (Node Package Manager)

### Local Installation
1. Clone the repository
```bash
git clone <repository-url>
cd file-system-simulator-api
```

2. Install dependencies
```bash
npm install
```

3. Start the server
```bash
npm start
```

The API will be available at http://localhost:3000.

### Docker (Optional)
1. Build the Docker image
```bash
docker build -t file-system-simulator-api .
```

2. Run the Docker container
```bash
docker run -p 3000:3000 file-system-simulator-api
```

## API Endpoints

### Files
* `POST /api/files` - Create a new file
  * Request Body: `{ "name": "filename.txt" }`

* `GET /api/files` - List all files in current directory

* `GET /api/files/:name` - Get file details

* `PUT /api/files/:name/content` - Write content to a file
  * Request Body: `{ "content": "File content goes here" }`

* `DELETE /api/files/:name` - Delete a file

### Folders
* `POST /api/folders` - Create a new folder
  * Request Body: `{ "name": "foldername" }`

* `GET /api/folders` - List all folders in current directory

* `GET /api/folders/content` - List all items (files and folders) in current directory

* `GET /api/folders/:name` - Get folder details

* `DELETE /api/folders/:name` - Delete a folder

### Navigation
* `GET /api/navigation/path` - Get current path

* `POST /api/navigation/cd` - Change directory
  * Request Body: `{ "folderName": "foldername" }` or `{ "folderName": ".." }` for parent

* `GET /api/navigation/tree` - Get file system tree structure

## Example Usage

### Create a Folder
```bash
curl -X POST http://localhost:3000/api/folders \
  -H "Content-Type: application/json" \
  -d '{"name": "documents"}'
```

### Navigate to Folder
```bash
curl -X POST http://localhost:3000/api/navigation/cd \
  -H "Content-Type: application/json" \
  -d '{"folderName": "documents"}'
```

### Create a File
```bash
curl -X POST http://localhost:3000/api/files \
  -H "Content-Type: application/json" \
  -d '{"name": "report.txt"}'
```

### Write to File
```bash
curl -X PUT http://localhost:3000/api/files/report.txt/content \
  -H "Content-Type: application/json" \
  -d '{"content": "Project documentation"}'
```

### List Directory Contents
```bash
curl -X GET http://localhost:3000/api/folders/content
```

### Show File System Tree
```bash
curl -X GET http://localhost:3000/api/navigation/tree
```

## Development

### Running Tests
```bash
npm test
```

### Project Structure
```
file-system-simulator-api/
│
├── src/
│   ├── models/          # Data models
│   │   ├── File.js
│   │   ├── Folder.js
│   │   └── FileSystem.js
│   ├── services/        # Business logic
│   │   ├── FileService.js
│   │   ├── FolderService.js
│   │   └── FileSystemService.js
│   ├── storage/         # Data storage
│   │   └── Storage.js
│   ├── routes/          # API endpoints
│   │   ├── fileRoutes.js
│   │   ├── folderRoutes.js
│   │   └── navigationRoutes.js
│   ├── middleware/      # Express middleware
│   │   ├── errorHandler.js
│   │   └── logger.js
│   └── app.js           # Express application
├── tests/               # Test files
│   ├── api.test.js
│   └── services.test.js
└── package.json         # Project metadata and dependencies
```