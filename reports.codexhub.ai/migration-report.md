# C++ to Node.js REST API Migration Report

## Migration Overview

The C++ File System Simulator has been successfully migrated to a Node.js REST API. This migration transformed a command-line interface application into a web service that provides the same functionality through HTTP endpoints while adopting JavaScript and RESTful API best practices.

## Migration Strategy Implemented

1. **Analyzing the Original C++ Structure**: We first studied the C++ implementation to understand its architecture, components, and functionality.

2. **Converting Models**: We created equivalent JavaScript classes for `File`, `Folder`, and `FileSystem`, preserving the object-oriented approach.

3. **Migrating Storage Management**: The Singleton `Storage` pattern was maintained in JavaScript, with maps and object references replacing the C++ STL containers.

4. **Implementing Services**: We migrated the service layer (`FileService`, `FolderService`, `FileSystemService`) to organize the business logic in a similar structure to the original code.

5. **Creating REST API Routes**: We designed RESTful endpoints to map the CLI commands to HTTP operations, following REST best practices.

6. **Adding Middleware**: We implemented error handling and logging middleware specific to a web API.

7. **Writing Tests**: We created comprehensive tests to verify the functionality of both the service layer and the API endpoints.

## Files Created/Modified

### New Files Created:
- **Models**:
  - `src/models/File.js`
  - `src/models/Folder.js`
  - `src/models/FileSystem.js`

- **Services**:
  - `src/services/FileService.js`
  - `src/services/FolderService.js`
  - `src/services/FileSystemService.js`

- **Storage**:
  - `src/storage/Storage.js`

- **API Routes**:
  - `src/routes/fileRoutes.js`
  - `src/routes/folderRoutes.js`
  - `src/routes/navigationRoutes.js`

- **Middleware**:
  - `src/middleware/errorHandler.js`
  - `src/middleware/logger.js`

- **Application**:
  - `src/app.js`

- **Tests**:
  - `tests/api.test.js`
  - `tests/services.test.js`

- **Configuration**:
  - `package.json`
  - `Dockerfile` (updated)

### Modified Files:
- `README.md` - Updated with Node.js API documentation

## Critical Translation Decisions

### 1. Replacing Stack with Array for Path Management

**C++:**
```cpp
// FileSystem.h
private:
    stack<string> path;
```

**JavaScript:**
```javascript
// FileSystem.js
constructor() {
  this.path = []; // Stack to track current path
}
```

JavaScript doesn't have a native stack data structure, so we used an array with push/pop operations to maintain the same functionality.

### 2. Converting Map-Based Tree Structure

**C++:**
```cpp
// Storage.h
map<string, map<string, int>> tree;
```

**JavaScript:**
```javascript
// Storage.js
constructor() {
  this.tree = new Map();
  // ...
  this.tree.set('F0', new Map());
}
```

We used JavaScript's Map object to maintain the same key-value lookup performance and structure as the C++ std::map.

### 3. Error Handling for Web Context

**C++:**
```cpp
// Storage.cpp
cout << "     " << "File name already exist! change the name of the file." << endl;
```

**JavaScript:**
```javascript
// fileRoutes.js
return res.status(400).json({
  success: false,
  error: {
    message: 'Failed to create file, it may already exist',
    code: 'CREATE_FAILED'
  }
});
```

Instead of console output, we implemented proper HTTP status codes and JSON error responses appropriate for a REST API.

### 4. DFS Algorithm for Tree Traversal

**C++:**
```cpp
// Storage.cpp
void Storage::showDFS(string node, string symbols) {
  cout << "     " << symbols + "- " << ((node[0] == 'F') ? folders[node]->getName() : files[node]->getFileName()) << endl;
  
  symbols += "  |";
  for (auto i : tree[node]) {
    showDFS(i.first, symbols);
  }
}
```

**JavaScript:**
```javascript
// Storage.js
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
    // Handle file case...
  }
  
  return result;
}
```

Instead of printing tree nodes directly to the console, we built a nested JSON structure that can be returned via the API.

### 5. Command-Line Interface to RESTful API

**C++:**
```cpp
// main.cpp
if (command == "mkdir") {
  string folderName;
  cin >> folderName;
  fileSystem->createFolder(fileSystem->getCurrentFolder(), folderName);
}
```

**JavaScript:**
```javascript
// folderRoutes.js
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
    
    // Handle response...
  } catch (err) {
    next(err);
  }
});
```

The most significant change was transforming the CLI commands into REST endpoints with appropriate HTTP methods.

## Testing Approach

We implemented a comprehensive testing strategy using Jest:

1. **Unit Testing**: 
   - `services.test.js` verifies the core business logic by testing individual services.
   - Tests include file and folder creation, navigation, content management, and tree structure.

2. **API Testing**: 
   - `api.test.js` tests the HTTP endpoints using Supertest to simulate API requests.
   - Each API route is tested with valid and invalid inputs to ensure proper response handling.

3. **Storage Reset**: 
   - Before each test, the Storage singleton is reset to ensure tests run in isolation.

4. **Test Coverage**:
   - All critical paths and error conditions are tested.
   - Includes validation of both successful operations and error handling.

## Test Results

All tests pass successfully, verifying that the migrated application provides the same functionality as the original C++ implementation, but in a RESTful API format.

## Challenges & Solutions

1. **Challenge**: Maintaining in-memory state in a web server environment
   **Solution**: Used the singleton pattern for Storage to maintain consistent state between requests

2. **Challenge**: Converting recursive tree traversal for API responses
   **Solution**: Modified the DFS algorithm to build nested JSON objects instead of console output

3. **Challenge**: Translating CLI commands to REST endpoints
   **Solution**: Designed intuitive REST routes that map to the original commands:
     - `mkdir` → POST /api/folders
     - `ls` → GET /api/folders/content
     - `cd` → POST /api/navigation/cd

## Limitations & Next Steps

1. **Persistence**: The current implementation is still in-memory only. Adding database integration would be a logical next step.

2. **Authentication**: The API currently has no authentication or authorization. Adding user accounts with proper access controls would be important for production use.

3. **Performance Optimization**: For large file systems, the current recursive tree traversal could be optimized or paginated.

4. **File Content Storage**: Currently, file content is stored as plain strings. Supporting binary content and streaming would enhance functionality.

5. **Client Application**: Developing a frontend web application to interact with this API would complete the migration from CLI to a full web application.

## Conclusion

The migration from C++ to Node.js REST API was successful, maintaining all the functionality of the original application while transforming it into a web service that follows modern API design practices. The code is well-structured, documented, and thoroughly tested, providing a solid foundation for further enhancements.