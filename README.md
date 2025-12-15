# README - Time Travelling File System

## Introduction

This project implements a **Time-Travelling File System** as part of
COL106: Data Structures and Algorithms.\
It is a simplified in-memory version control system inspired by Git. The
system manages versioned files with support for branching, snapshots,
rollback, and system-wide analytics.\
The implementation uses custom-built Trees, HashMaps, and Heaps (without
relying on STL map/unordered_map/priority_queue).

------------------------------------------------------------------------

## Classes and Attributes

### Node

-   `int version_id`
-   `std::string content`
-   `std::string message`
-   `time_t created_timestamp`
-   `time_t snapshot_timestamp`
-   `Node* parent`
-   `std::vector<Node*> children`
-   `bool is_snapshot`
-   Constructor initializes node attributes.

### File

-   `Node* root`
-   `Node* active_version`
-   `HashMap_ID version_map`
-   `int total_versions`
-   `time_t last_modified`
-   `std::string fname`
-   Manages file-specific version history.

### HashNode_ID & HashMap_ID

-   Custom HashMap for mapping **version IDs → Node**\*\
-   Supports insert, find, get operations.\
-   Handles rehashing when load factor \> 0.8.

### HashNode & HashMap

-   Custom HashMap for mapping **filename → File**\*\
-   Supports insert, find, get operations.\
-   Handles rehashing.

------------------------------------------------------------------------

## Functions Implemented

-   `create(filename)` → Creates a file with root snapshot (version 0).\
-   `read(filename)` → Prints active version content.\
-   `insert(filename, content)` → Appends content to active version or creates new version if active is snapshot.
-   `update(filename, content)` →  Replaces content of active version (new node if snapshot).
-   `snapshot(filename, message)` → Marks active version as snapshot.\
-   `rollBack(filename, [versionID])` → Rolls back active version to
    parent or given version ID.\
-   `history(filename)` → Displays chronological snapshots (ID,
    timestamp, message).\
-   `recentFiles(num)` → Lists most recently modified files using heap.\
-   `biggestTrees(num)` → Lists files with most versions using heap.


------------------------------------------------------------------------

## Error Handling

-   Empty filename on **CREATE** → `"File Name cannot be empty"`\
-   Duplicate filename on **CREATE** → `"File Name already exists"`\
-   **READ** on empty content → `"File has no contents to read"`\
-   **INSERT/UPDATE** without content → Warning message printed\
-   **SNAPSHOT** with empty message → Warning but snapshot still saved\
-   **ROLLBACK** invalid ID → Error message shown\
-   Invalid command → `"Error: Unknown Command"`

------------------------------------------------------------------------

## Time Complexity Analysis

-   **HashMap insert/find/get** → O(1) average, O(n) worst-case with
    chaining\
-   **Rehashing** → O(n) when triggered (amortized O(1) per operation)\
-   **Tree operations (insert/update/snapshot/rollback)** → O(1)\
-   **Heapify** → O(log n) per call, building heap O(n)\
-   **recentFiles(num) & biggestTrees(num)** → O(n log n) (heap
    construction + extraction)

------------------------------------------------------------------------

## Input and Output

**Input**: User enters commands at runtime through stdin. Supported
commands:\
- `CREATE <filename>`\
- `READ <filename>`\
- `INSERT <filename> <content>`\
- `UPDATE <filename> <content>`\
- `SNAPSHOT <filename> <message>`\
- `ROLLBACK <filename> [versionID]`\
- `HISTORY <filename>`\
- `RECENT_FILES <num>`\
- `BIGGEST_TREES <num>`\
- `EXIT`

**Output**: Console messages such as file contents, error messages,
version histories, and ordered file lists.

------------------------------------------------------------------------
