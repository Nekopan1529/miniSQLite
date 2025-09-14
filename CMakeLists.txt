cmake_minimum_required(VERSION 3.10)
project(miniSQLite C)

add_executable(main "")  # Create an empty executable first

# Add sources to the executable
target_sources(main PRIVATE
    src/main.c
    src/repl/repl.c
    src/parser/parser.c
    src/executor/executor.c
    src/db/db.c
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    # Enable AddressSanitizer and related flags
    target_compile_options(main PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(main PRIVATE -fsanitize=address)
endif()

set_target_properties(main PROPERTIES C_STANDARD 23)

set_target_properties(main PROPERTIES C_STANDARD_REQUIRED ON)