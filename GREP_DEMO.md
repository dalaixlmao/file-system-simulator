# Grep Feature Demo

## Overview
This demo showcases the grep functionality implemented in the File System Simulator. The grep feature allows searching for text patterns in files with various options and filters.

## Demo Script
Run the file system simulator and execute the following commands to test grep functionality:

```bash
# Start the simulator
./file_system_simulator

# Create test files with content
touch file1.txt
write file1.txt Hello World! This is a test file.
write file1.txt Second line contains the word TEST.
write file1.txt Third line has different content.
write file1.txt Fourth line mentions Hello again.

touch file2.txt
write file2.txt This is another file.
write file2.txt It contains some different words.
write file2.txt Including test and sample data.

touch config.conf
write config.conf
write config.conf server_name=localhost
write config.conf port=8080
write config.conf debug=true

# Create a subdirectory with files
mkdir docs
cd docs
touch readme.md
write readme.md # Project Documentation
write readme.md This project implements a file system simulator.
write readme.md Features include file creation and search.
cd ..

# Test grep functionality
grep --help                     # Show help
grep Hello                      # Search for "Hello" in all files
grep Hello file1.txt           # Search in specific file
grep -i hello                  # Case-insensitive search
grep -c test                   # Count matches
grep -v line                   # Invert match (non-matching lines)
grep -r project                # Recursive search
grep server_name config.conf   # Search in configuration file

# View results
ls
tree
history
```

## Expected Features
1. **Pattern Matching**: Successfully finds text patterns in file content
2. **File-Specific Search**: Can target individual files
3. **Case Insensitive**: `-i` option works for case-insensitive matching
4. **Count Mode**: `-c` option shows match counts
5. **Invert Match**: `-v` option shows non-matching lines
6. **Recursive Search**: `-r` option searches through subdirectories
7. **Line Numbers**: Shows line numbers with matches
8. **Help System**: `--help` displays usage information

## Implementation Highlights
- **GrepService**: Dedicated service class for grep operations
- **Regex Support**: Basic regular expression matching
- **Options Parsing**: Command-line style option handling
- **Integration**: Seamless integration with existing file system
- **History Tracking**: Grep commands are tracked in history
