# File System Simulator

## Overview

The File System Simulator is a C++ implementation of a simplified hierarchical file system that mimics the functionality of real-world file systems like NTFS or FAT32. This project provides a command-line interface (CLI) to interact with an in-memory file system, allowing users to perform common file and directory operations.

## Features

- Create and delete files and directories
- Navigate through the file system hierarchy
- List contents of directories
- View file system tree structure
- Supports relative and absolute path navigation
- Implements basic file and folder management operations

## Project Structure

```
file-system-simulator/
│
├── include/
│   ├── models/
│   │   ├── File.h
│   │   ├── FileSystem.h
│   │   └── Folder.h
│   │
│   ├── services/
│   │   ├── FileService.h
│   │   ├── FileSystemService.h
│   │   └── FolderService.h
│   │
│   └── storage/
│       └── Storage.h
│
├── src/
│   ├── models/
│   │   ├── File.cpp
│   │   ├── FileSystem.cpp
│   │   └── Folder.cpp
│   │
│   ├── services/
│   │   ├── FileService.cpp
│   │   ├── FileSystemService.cpp
│   │   └── FolderService.cpp
│   │
│   └── storage/
│       └── Storage.cpp
│
└── main.cpp
```

## Supported Commands

- `mkdir <FolderName>`: Create a new directory
- `rmdir <FolderName>`: Remove a directory
- `cd <FolderName>`: Change current directory
- `ls`: List items in current directory
- `touch <FileName>`: Create a new file
- `write <FileName> <Content>`: Write content to a file
- `rm <FileName>`: Remove a file
- `tree`: Display the file system hierarchy

## Design Principles

### Object-Oriented Programming (OOP)
- **Encapsulation**: Private attributes with public methods
- **Inheritance**: Base `FileSystemEntity` class
- **Polymorphism**: Virtual functions for file and directory actions
- **Abstraction**: Simplified file allocation implementation

### Data Structures and Algorithms
- N-ary tree representation of file system hierarchy
- Hash maps for efficient file and folder management
- Depth-First Search (DFS) for tree traversal and operations

## Installation

### Prerequisites
- C++ compiler (C++11 or later)
- CMake (optional, for build management)

### Compilation
```bash
g++ -std=c++11 main.cpp src/*/*.cpp -I include -o file-system-simulator
```

## Usage Example

```bash
# Start the application
./file-system-simulator

# Create a directory
mkdir documents

# Change directory
cd documents

# Create a file
touch report.txt

# Write content to the file
write report.txt "Project documentation"

# List directory contents
ls

# View file system tree
tree
```

## Key Components

1. **Models**
   - `File`: Represents individual files
   - `Folder`: Represents directories
   - `FileSystem`: Manages current path and navigation

2. **Services**
   - `FileService`: File-related operations
   - `FolderService`: Folder-related operations
   - `FileSystemService`: Integrated file system management

3. **Storage**
   - Singleton `Storage` class for managing file system state
   - In-memory representation using maps and trees