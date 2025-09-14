# miniSQLite

miniSQLite is a minimal, educational SQLite-like database engine written in C. It supports basic SQL-like commands for inserting, selecting, and deleting rows from a simple table stored on disk.

## Features

- Simple REPL (Read-Eval-Print Loop) interface
- Persistent storage to a file (`db_file.db`)
- Supports the following commands:
  - `insert <id> <name> <email>`: Insert a new row
  - `select`: Display all rows
  - `delete <id>`: Delete a row by ID
  - `.exit`: Exit the program

## Table Schema

The table managed by miniSQLite has the following columns:

| Column | Type    | Description      |
|--------|---------|------------------|
| id     | int     | Unique row ID    |
| name   | string  | User's name      |
| email  | string  | User's email     |

## Getting Started

### Prerequisites

- C compiler (GCC or Clang)
- CMake (version 3.10 or higher)
- Make

### Build Instructions

1. Clone the repository:
    ```sh
    git clone https://github.com/Nekopan1529/miniSQLite.git
    cd miniSQLite
    ```

2. Build the project using CMake:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

3. Run the program:
    ```sh
    ./main
    ```

## Usage

Once running, you can enter commands at the prompt:

- Insert a row:
    ```
    insert <id> <name> <email>
    ```
- Select all rows(it just prints the entire db at the moment):
    ```
    select
    ```
- Delete a row:
    ```
    delete <row_index>
    ```
- Exit:
    ```
    .exit
    ```

## File Structure

```
miniSQLite/
├── src/                # Source code directory
│   ├── main.c           
│   ├── repl/           # REPL (Read-Eval-Print Loop) logic
│   ├── parser/         # SQL command parsing logic
│   ├── executor/       # Command execution logic
│   └── db/             # Database logic such as insert and persistence of data
├── db_file.db          # Database file (created at runtime)
├── CMakeLists.txt      # build configuration
├── README.md           # Project documentation
└──
```

## Acknowledgements

Inspired by [cstack/db_tutorial](https://cstack.github.io/db_tutorial/). Parts like repl, parser and executor are mainly from here. 