# Project 1: KVStore using Log-structured Merge Tree


### The handout files include two main parts:

- The `KVStoreAPI` class in `kvstore_api.h` that specifies the interface of KVStore.
- Test files including correctness test (`correctness.cc`) and persistence test (`persistence.cc`).

### Explanation of each handout file:

```text
.
├── Makefile       // Makefile if you use GNU Make
├── README.md      // This readme file
├── correctness.cc // Correctness test, do not modify this file
├── correctness.cc // A compiled c-test whose parameter is 8096
├── data           // Data directory used in our test
├── SkipList.hpp   // A simple skip list
├── MemTable.hpp   // A memory table based on skip list
├── SSTable.hpp    // Implementation of sstable
├── SSLevel.hpp    // Level of disk containing several sstables
├── disk.hpp       // Containing several sslevels
├── platGround.cpp // My private field to test my codes (ignore) 
├── performance.cc // To test lsmtree's performanc
├── kvstore.h      // Your implementation
├── kvstore_api.h  // KVStoreAPI, you should not modify this file
├── persistence.cc // Persistence test, do not modify this file
├── persistence.exe// A compiled p-test whose parameter is 8096
└── test.h         // Base class for testing, do not modify this file
```


- First have a look at the `kvstore_api.h` file to check functions you need to implement. 
- Then modify the `kvstore.cc` and `kvstore.h` files and feel free to add new class files.

We will use all files with `.cc`, `.cpp`, `.cxx` suffixes to build correctness and persistence tests. Thus, you can use any IDE to finish this project as long as you ensure that all C++ source files are submitted.

For the test files, of course you could modify it to debug your programs. But remember to change it back when you are testing.

## Good luck :)
