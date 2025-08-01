cmake_minimum_required(VERSION 3.10)
include (FetchContent)

project( simple-image-writer
  LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

add_executable(simple-image-writer
  src/main.c
)

target_link_libraries(simple-image-writer PRIVATE ${GTK_LIBRARIES})

target_include_directories(simple-image-writer PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(simple-image-writer PRIVATE ${GTK_LIBRARY_DIRS})
