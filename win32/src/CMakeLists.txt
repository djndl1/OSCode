set(winapi_src
    winhandle.c
    winfile.c
    winerror.c
    win32_allocators.c
    win32/errors.cpp
    win32/processes.cpp
    win32/utf_cvt.cpp
)

add_library(cwinapi_static STATIC
  ${winapi_src}
)

link_directories(AFTER ${modernlib_BINARY_DIR})
target_include_directories(cwinapi_static AFTER
  PUBLIC ../include/
)
target_compile_definitions(cwinapi_static
  PRIVATE CWINAPI_BUILD_STATIC UNICODE _UNICODE _WIN32_WINNT=0x0501)
target_compile_definitions(cwinapi_static
  PUBLIC MODERNLIB_STATIC_BUILD
)
target_link_libraries(cwinapi_static PUBLIC modernlib_static)
set_target_properties(cwinapi_static
  PROPERTIES
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)

add_library(cwinapi SHARED
  ${winapi_src}
)
target_include_directories(cwinapi AFTER
  PUBLIC ../include/
)
target_compile_definitions(cwinapi
  PRIVATE CWINAPI_BUILD_DLL UNICODE _UNICODE _WIN32_WINNT=0x0501
)
target_link_libraries(cwinapi PUBLIC modernlib)
set_target_properties(cwinapi
  PROPERTIES
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)
install(TARGETS cwinapi DESTINATION ${CMAKE_BINARY_DIR})

add_executable(io_behavior_test
  io_behavior_test.c
)
target_include_directories(io_behavior_test AFTER
  PUBLIC ../include/
)
target_compile_definitions(io_behavior_test
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=_WIN32_WINNT_WINXP
)
target_link_libraries(io_behavior_test PUBLIC cwinapi modernlib)
install(TARGETS io_behavior_test DESTINATION ${CMAKE_BINARY_DIR})
