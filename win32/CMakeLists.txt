cmake_minimum_required(VERSION 3.13)

project(Win32API)

include(ExternalProject)

set(NO_SONAME ON)
set(CMAKE_PLATFORM_NO_VERSIONED_SONAME ON)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  lib/include
  include
  deps/modernlib/include
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps/modernlib)
install(TARGETS modernlib DESTINATION ${CMAKE_BINARY_DIR})
add_subdirectory(src)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

add_executable(CreateProcess
  src/CreateProcess.cpp
  lib/src/errors.cpp
  lib/src/processes.cpp)
target_compile_definitions(CreateProcess
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0501)
target_link_options(CreateProcess PRIVATE -mconsole -municode)

add_executable(ProcessIdentity
  src/ProcessIdentity.cpp
  lib/src/errors.cpp
  lib/src/processes.cpp)
target_compile_definitions(ProcessIdentity
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0601)
target_link_options(ProcessIdentity PRIVATE -mconsole -municode)

add_executable(cp src/cp.c)
target_compile_definitions(cp
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0501)
target_link_libraries(cp cwinapi_static)
target_link_options(cp PRIVATE -mconsole)

add_custom_target(
  cpw32 ALL
  ${CMAKE_COMMAND} -E copy
  $<TARGET_FILE:cp>
  $<TARGET_FILE_DIR:cp>/cpw32.exe
)
add_dependencies(cpw32 cp)
add_custom_target(
  cpc ALL
  ${CMAKE_COMMAND} -E copy
  $<TARGET_FILE:cp>
  $<TARGET_FILE_DIR:cp>/cpc.exe
)
add_dependencies(cpc cp)
add_custom_target(
  cpw32s ALL
  ${CMAKE_COMMAND} -E copy
  $<TARGET_FILE:cp>
  $<TARGET_FILE_DIR:cp>/cpw32s.exe
)
add_dependencies(cpw32s cp)

#add_executable(cat
#  src/cat.c
 # src/ReportError.c)
#target_compile_definitions(cat
#  PRIVATE UNICODE _UNICODE)

add_executable(pwd
  src/pwd.c
  src/ReportError.c
  src/ConsoleUtility.c)
target_compile_definitions(pwd
  PRIVATE UNICODE _UNICODE)
target_link_options(pwd PRIVATE -municode -mconsole)


add_executable(console_test
  src/console_test.c)
target_compile_definitions(console_test
  PRIVATE UNICODE _UNICODE)
target_link_options(console_test PRIVATE -municode -mconsole)

add_executable(console_ctrl
  src/console_ctrl.c)
target_compile_definitions(console_ctrl
  PRIVATE UNICODE _UNICODE)
target_link_options(console_ctrl PRIVATE -municode -mconsole)

add_executable(system_info
  src/system_info.c)
target_compile_definitions(system_info
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0600)
target_link_options(system_info PRIVATE -municode -mconsole)
