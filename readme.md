# File System Simulator

## Overview
The File System Simulator is a C++ implementation of a simplified hierarchical file system that mimics the functionality of real-world file systems like NTFS or FAT32. This project provides a command-line interface (CLI) to interact with an in-memory file system, allowing users to perform common file and directory operations.

## Features
* Create and delete files and directories
* Navigate through the file system hierarchy
* List contents of directories
* View file system tree structure
* Search for patterns in files using grep functionality
* Command history tracking and management
* Supports relative and absolute path navigation
* Implements basic file and folder management operations

## Installation and Usage

### Docker (Recommended)
#### Pull the Docker Image
```bash
docker pull dalaixlmao/file-system-simulator
```

#### Run the Docker Container
```bash
docker run -it dalaixlmao/file-system-simulator
```

### Local Compilation
#### Prerequisites
* C++ compiler (C++11 or later)
* CMake (optional, for build management)

#### Compile Manually
```bash
g++ -std=c++11 main.cpp src/*/*.cpp -I include -o file-system-simulator
```

## Supported Commands
* `mkdir <FolderName>`: Create a new directory
* `rmdir <FolderName>`: Remove a directory
* `cd <FolderName>`: Change current directory
* `ls`: List items in current directory
* `touch <FileName>`: Create a new file
* `write <FileName> <Content>`: Write content to a file
* `rm <FileName>`: Remove a file
* `tree`: Display the file system hierarchy
* `history [number]`: Show command history (optionally limit to number of entries)
* `history clear`: Clear command history
* `grep <pattern>`: Search for pattern in all files in current directory
* `grep <pattern> <filename>`: Search for pattern in specific file
* `grep -[options] <pattern>`: Search with options (i=case-insensitive, r=recursive, c=count, v=invert, n=line numbers)
* `grep --help`: Show grep help and usage information

## Usage Example
```bash
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

# Search for text in files
grep "documentation"

# Search for pattern in specific file
grep "Project" report.txt

# Case-insensitive search
grep -i "project"

# View command history
history

# Get help for grep
grep --help
```

## Grep Functionality

The File System Simulator includes a comprehensive grep implementation that allows you to search for patterns in files. This feature mimics the behavior of the Unix `grep` command.

### Grep Features
* **Pattern Matching**: Search for text patterns in file content
* **Case Sensitivity**: Support for both case-sensitive and case-insensitive searches
* **File-Specific Search**: Target specific files or search across all files
* **Recursive Search**: Search through subdirectories recursively
* **Line Numbers**: Display line numbers with matching results
* **Count Mode**: Show only the count of matches
* **Invert Match**: Show lines that don't match the pattern
* **Regex Support**: Basic regular expression pattern matching

### Grep Command Examples
```bash
# Basic pattern search in current directory
grep "error"

# Search in specific file
grep "TODO" main.cpp

# Case-insensitive search
grep -i "Error"

# Recursive search through all subdirectories
grep -r "function"

# Count matches only
grep -c "include"

# Show non-matching lines
grep -v "comment"

# Combine options (case-insensitive + recursive)
grep -ir "main"

# Get help
grep --help
```

### Grep Options
* `-i`: Case-insensitive search
* `-r`: Recursive search in subdirectories
* `-c`: Count matches only (show number instead of lines)
* `-v`: Invert match (show non-matching lines)
* `-n`: Show line numbers (enabled by default)

### Grep Output Format
```
=== /path/to/file.txt ===
1: First line with matching pattern
3: Another line with the pattern
```

## Project Architecture

### Design Principles
#### Object-Oriented Programming (OOP)
* **Encapsulation**: Private attributes with public methods
* **Inheritance**: Base `FileSystemEntity` class
* **Polymorphism**: Virtual functions for file and directory actions
* **Abstraction**: Simplified file allocation implementation

#### Data Structures and Algorithms
* N-ary tree representation of file system hierarchy
* Hash maps for efficient file and folder management
* Depth-First Search (DFS) for tree traversal and operations

### Project Structure
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
│   │   ├── FolderService.h
│   │   ├── HistoryService.h
│   │   └── GrepService.h
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
│   │   ├── FolderService.cpp
│   │   ├── HistoryService.cpp
│   │   └── GrepService.cpp
│   │
│   └── storage/
│       └── Storage.cpp
│
└── main.cpp
```

## Key Components
1. **Models**
   * `File`: Represents individual files with content storage
   * `Folder`: Represents directories in the hierarchy
   * `FileSystem`: Manages current path and navigation
   * `History`: Tracks command history and metadata
2. **Services**
   * `FileService`: File-related operations (create, write, delete)
   * `FolderService`: Folder-related operations (create, navigate, delete)
   * `HistoryService`: Command history management
   * `GrepService`: Pattern searching and text matching
   * `FileSystemService`: Integrated file system management
3. **Storage**
   * Singleton `Storage` class for managing file system state
   * In-memory representation using maps and trees
   * Supports file content storage and retrieval

## Docker Information
### Docker Hub
* **Image**: dalaixlmao/file-system-simulator
* **Repository**: https://hub.docker.com/r/dalaixlmao/file-system-simulator

### Docker Considerations
* Provides an isolated environment for the File System Simulator
* All operations are in-memory and will reset when the container is closed
* Ideal for demonstration and testing purposes
* No persistent storage between container restarts
