cmake_minimum_required(VERSION 3.10)
project(miniSQLite)

add_executable(main "")  # Create an empty executable first

# Add sources to the executable
target_sources(main PRIVATE
    src/main.c
    src/repl/repl.c
    src/parser/parser.c
    src/executor/executor.c
    src/db/db.c
)