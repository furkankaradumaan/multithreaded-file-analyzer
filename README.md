# Parallel File Analyzer

A simple multithreaded file analyzer written in C using the POSIX Threads (`pthread`) library.

This project was built as a learning exercise to understand the fundamentals of multithreaded programming in C. Rather than maximizing performance, its primary goal is to demonstrate the lifecycle of POSIX threads by analyzing multiple files concurrently.

---

## Features

- Analyze all regular files in a directory
- Create **one worker thread per file**
- Count:
  - Number of lines
  - File size (bytes)
- Synchronize threads using `pthread_join()`
- Modular architecture with separated responsibilities
- Basic error handling for file operations and thread creation

---

## Project Structure

```text
.
├── include/
│   ├── analyzer.h
│   └── thread.h
│
├── src/
│   ├── analyzer.c
│   ├── thread.c
│   └── main.c
│
├── Makefile
└── README.md
```

---

## Build

```bash
make
```

---

## Usage

```bash
./parallel-file-analyzer <directory>
```

Example:

```bash
./parallel-file-analyzer ./sample_files
```

---

## Example Output

```text
====================================================
Parallel File Analyzer
====================================================

File: ./sample_files/main.c
Status: OK
Lines: 187
Bytes: 5412

--------------------------------------------

File: ./sample_files/thread.c
Status: OK
Lines: 43
Bytes: 1297

--------------------------------------------

File: ./sample_files/README.md
Status: OK
Lines: 52
Bytes: 1846
```

---

## Design

Each regular file inside the target directory is assigned to its own worker thread.

```
                 Main Thread
                      │
      ┌───────────────┼───────────────┐
      │               │               │
      ▼               ▼               ▼
 file1.txt       file2.txt       file3.txt
      │               │               │
      ▼               ▼               ▼
 Worker 1        Worker 2        Worker 3
      │               │               │
      └───────────────┼───────────────┘
                      ▼
              Analysis Results
```

Each worker thread:

1. Receives a file path.
2. Opens the file.
3. Counts the number of lines.
4. Determines the file size.
5. Stores the analysis result.
6. Terminates.

The main thread waits for all workers using `pthread_join()` before printing the final report.

---

## What I Learned

This project helped me understand several core multithreading concepts:

- POSIX Threads (`pthread`)
- Thread creation with `pthread_create()`
- Thread synchronization with `pthread_join()`
- Thread entry functions
- Passing arguments to worker threads
- Thread lifecycle
- Thread-local execution
- Modular C project organization
- Basic concurrent program design

---

## Why One Thread Per File?

Creating one thread for every file is **not** the most scalable approach.

This design was chosen intentionally because the objective of this project is to understand how threads work rather than building the most efficient implementation.

For directories containing thousands of files, creating thousands of threads introduces significant overhead.

A production-quality implementation would instead use a **Thread Pool**, where a fixed number of worker threads process tasks from a shared work queue.

This project serves as the first step toward implementing such a thread pool.

---

## Possible Improvements

- Thread Pool implementation
- Producer–Consumer work queue
- Recursive directory traversal
- Word counting
- Character counting
- Configurable worker thread count
- Colored terminal output
- Performance benchmarking
- Support for symbolic links and additional file types

---

## Technologies

- C17
- POSIX Threads (`pthread`)
- POSIX Directory API (`dirent`)
- GCC
- Make

---

## Build Requirements

- Linux
- GCC
- POSIX-compliant operating system
- pthread library

---

## License

This project is licensed under the MIT License.
