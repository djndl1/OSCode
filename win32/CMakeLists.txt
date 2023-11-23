cmake_minimum_required(VERSION 3.13)

project(Win32API)

include(ExternalProject)

set(CMAKE_SYSTEM_NAME WINDOWS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  lib/include
  ${CMAKE_BINARY_DIR}/gnulib)

set(GNULIB_BINARY_DIR ${CMAKE_BINARY_DIR}/gnulib)

add_compile_options(
  $<$<COMPILE_LANGUAGE:C>:-std=c17>)
add_compile_options(
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>)

# there's a problem on Windows that Git bash cannot properly redirect
# a header to an output file, resulting in a build failure
ExternalProject_Add(gnulib
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/gnulib/
  BINARY_DIR ${GNULIB_BINARY_DIR}
  CONFIGURE_COMMAND ../../lib/gnulib/configure
  BUILD_COMMAND make
)

add_executable(CreateProcess
  src/CreateProcess.cpp
  lib/src/errors.cpp
  lib/src/processes.cpp)
target_compile_definitions(CreateProcess
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0501)
target_link_options(CreateProcess
  PRIVATE -municode)

add_executable(ProcessIdentity
  src/ProcessIdentity.cpp
  lib/src/errors.cpp
  lib/src/processes.cpp)
target_compile_definitions(ProcessIdentity
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0601)
target_link_options(ProcessIdentity
  PRIVATE -municode)

add_executable(cp src/cp.c)
target_compile_definitions(cp
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0501)
target_link_options(cp PRIVATE -mconsole)

add_custom_target(
  cpw32 ALL
  ${CMAKE_COMMAND} -E copy
  ${CMAKE_BINARY_DIR}/cp.exe
  ${CMAKE_BINARY_DIR}/cpw32.exe
)
add_dependencies(cpw32 cp)
add_custom_target(
  cpc ALL
  ${CMAKE_COMMAND} -E copy
  ${CMAKE_BINARY_DIR}/cp.exe
  ${CMAKE_BINARY_DIR}/cpc.exe
)
add_dependencies(cpc cp)
add_custom_target(
  cpw32s ALL
  ${CMAKE_COMMAND} -E copy
  ${CMAKE_BINARY_DIR}/cp.exe
  ${CMAKE_BINARY_DIR}/cpw32s.exe
)
add_dependencies(cpw32s cp)

add_executable(cat
  src/cat.c
  src/ReportError.c)
target_compile_definitions(cat
  PRIVATE UNICODE _UNICODE)
target_link_directories(cat PRIVATE ${GNULIB_BINARY_DIR}/lib/)
add_dependencies(cat gnulib)
target_link_options(cat PRIVATE -mconsole -lgnu)

add_executable(pwd
  src/pwd.c
  src/ReportError.c
  src/ConsoleUtility.c)
target_compile_definitions(pwd
  PRIVATE UNICODE _UNICODE)
target_link_directories(pwd PRIVATE ${GNULIB_BINARY_DIR}/lib/)
target_link_options(pwd PRIVATE -lgnu -mconsole -municode)
add_dependencies(pwd gnulib)


