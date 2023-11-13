cmake_minimum_required(VERSION 3.12)

project(Win32API)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_SYSTEM_NAME WINDOWS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(lib/include)

add_executable(CreateProcess
  src/CreateProcess.cpp
  lib/src/errors.cpp
  lib/src/processes.cpp)
target_compile_definitions(CreateProcess
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x6000)
target_link_options(CreateProcess
  PRIVATE -municode)

add_executable(ProcessIdentity
  src/ProcessIdentity.cpp
  lib/src/errors.cpp
  lib/src/processes.cpp)
target_compile_definitions(ProcessIdentity
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x6000)
target_link_options(ProcessIdentity
  PRIVATE -municode)

add_executable(cp src/cp.c)
target_compile_definitions(cp
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x6000)
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
