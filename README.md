# ⏳ Time Travelling File System

A simplified **in-memory version control system** inspired by Git, implemented as part of **COL106: Data Structures and Algorithms**. The system supports versioned files with branching, snapshots, rollback, and system-wide analytics—built using **custom data structures** (Trees, HashMaps, Heaps) without relying on `std::map`, `std::unordered_map`, or `std::priority_queue`.

---

## ✨ Features

- Versioned files with parent–child relationships
- Snapshots with messages and timestamps
- Rollback to parent or a specific version
- File history visualization
- Analytics across files:
  - Most recently modified files
  - Files with the largest version trees

---

## 🧱 Architecture

### Core Classes

#### `Node`
Represents a version of a file.

**Attributes**
- `int version_id`
- `std::string content`
- `std::string message` (non-empty for snapshots)
- `time_t created_timestamp`
- `time_t snapshot_timestamp`
- `Node* parent`
- `std::vector<Node*> children`
- `bool is_snapshot`

**Notes**
- Constructor initializes all attributes.
- Forms a tree of versions via `parent` and `children`.

---

#### `File`
Manages the version history of a single file.

**Attributes**
- `Node* root`
- `Node* active_version`
- `HashMap_ID version_map` (version ID → `Node*`)
- `int total_versions`
- `time_t last_modified`
- `std::string fname`

---

### Custom Hash Maps

#### `HashNode_ID` & `HashMap_ID`
Maps **version IDs → Node\***.

- Operations: insert, find, get
- Collision handling: chaining
- Rehashing when load factor > 0.8

#### `HashNode` & `HashMap`
Maps **filename → File\***.

- Operations: insert, find, get
- Collision handling: chaining
- Rehashing supported

---

## ⚙️ Supported Operations

| Command | Description |
|---|---|
| `CREATE <filename>` | Create a new file with a root snapshot (version 0) |
| `READ <filename>` | Print the content of the active version |
| `INSERT <filename> <content>` | Append content to active version (or create a new version if active is a snapshot) |
| `UPDATE <filename> <content>` | Replace content of active version (new node if snapshot) |
| `SNAPSHOT <filename> <message>` | Mark active version as a snapshot |
| `ROLLBACK <filename> [versionID]` | Roll back to parent or a specific version |
| `HISTORY <filename>` | Display snapshot history (ID, timestamp, message) |
| `RECENT_FILES <num>` | List most recently modified files |
| `BIGGEST_TREES <num>` | List files with the most versions |
| `EXIT` | Terminate the program |

---

## 🛡️ Error Handling

- **CREATE** with empty filename → `"File Name cannot be empty"`
- **CREATE** with duplicate filename → `"File Name already exists"`
- **READ** on empty content → `"File has no contents to read"`
- **INSERT / UPDATE** without content → Warning message
- **SNAPSHOT** with empty message → Warning (snapshot still saved)
- **ROLLBACK** with invalid version ID → Error message
- Invalid command → `"Error: Unknown Command"`

---

## ⏱️ Time Complexity

| Operation | Complexity |
|---|---|
| HashMap insert/find/get | **O(1)** average, **O(n)** worst-case |
| Rehashing | **O(n)** when triggered (amortized **O(1)**) |
| Tree operations (insert/update/snapshot/rollback) | **O(1)** |
| Heapify | **O(log n)** per operation; **O(n)** build |
| `RECENT_FILES`, `BIGGEST_TREES` | **O(n log n)** |

---

## 🧪 Input & Output

### Input
Commands are provided via **stdin** at runtime. See the command table above for supported operations and formats.

### Output
Console output includes:
- File contents
- Error and warning messages
- Snapshot histories
- Ordered lists for analytics commands

---

## 📌 Notes

- Entire system operates **in-memory**.
- Designed to emphasize **data-structure implementation and analysis**.
- Suitable for extension to persistent storage or concurrency control.

