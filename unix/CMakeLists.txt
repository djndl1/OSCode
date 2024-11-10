cmake_minimum_required(VERSION 3.12)

project(UnixCode)

include_directories(AFTER ${UnixCode_SOURCE_DIR}/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(glibc_version src/glibc_version.c)
add_executable(error_handling src/error_handling.c)
add_executable(user_group src/user_group.c)
add_executable(fileio
  src/fileio.c
  src/test_utils.c
  src/file_desc.c
  src/data_buffer.c
  src/allocator.c
)

add_executable(fileio_sigasyncio
  src/fileio_sigasyncio.c
  src/test_utils.c
  src/file_desc.c
  src/data_buffer.c
  src/allocator.c
)
