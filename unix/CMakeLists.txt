cmake_minimum_required(VERSION 3.12)

project(UnixCode)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(deps/modernlib)
install(TARGETS modernlib DESTINATION ${CMAKE_BINARY_DIR})

include_directories(AFTER
  ${UnixCode_SOURCE_DIR}/include
  ${modernlib_SOURCE_DIR}/include
)


add_executable(glibc_version src/glibc_version.c)
add_executable(error_handling src/error_handling.c)
add_executable(user_group src/user_group.c)
add_executable(fileio
  src/file_desc.c
  src/fileio.c
  src/test_utils.c
  #src/file_poll.c
)
target_link_libraries(fileio modernlib)

add_executable(fileio_sigasyncio
  src/fileio_sigasyncio.c
  src/test_utils.c
  src/file_desc.c
)
target_link_libraries(fileio_sigasyncio modernlib)
