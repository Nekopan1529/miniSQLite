cmake_minimum_required(VERSION 3.10)
project(sqlite-light)

add_executable(main src/main.c src/repl/repl.c src/parser/parser.c)
