cmake_minimum_required(VERSION 3.13)

project(Win32API_MSVC)

include(ExternalProject)

set(CMAKE_SYSTEM_NAME WINDOWS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER cl.exe)
set(CMAKE_CXX_COMPILER cl.exe)

add_compile_definitions(UNICODE _UNICODE)

include_directories(
  ../src/
  ../lib/include)

add_executable(toupper
  to_upper.cc
  ../src/ReportError.c)
