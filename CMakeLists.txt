cmake_minimum_required(VERSION 3.10)
include (FetchContent)

project( simple-image-writer
  LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

find_package(Threads REQUIRED)

add_executable(simple-image-writer
  src/main.c
  vendor/tinyfiledialogs/tinyfiledialogs.c
)

# target_compile_options(simple-image-writer PUBLIC -g)

target_include_directories(simple-image-writer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinyfiledialogs)
target_include_directories(simple-image-writer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

target_link_libraries(simple-image-writer PRIVATE ${GTK_LIBRARIES})

target_include_directories(simple-image-writer PRIVATE ${GTK_INCLUDE_DIRS})
target_link_directories(simple-image-writer PRIVATE ${GTK_LIBRARY_DIRS})

target_link_libraries(simple-image-writer PRIVATE Threads::Threads)
