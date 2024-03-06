cmake_minimum_required(VERSION 3.10)

# Project name
project(markdown-parser)

# Include directories
include_directories(src)

# Add main executable
# add_executable(app hashtable/copystring.c hashtable/hashtable.c)

# Add test executable
add_executable(test_app src/hashtable/test/test.c src/hashtable/copystring.c src/hashtable/hashtable.c)
