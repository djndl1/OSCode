set(winapi_src
    winhandle.c
    winfile.c
    winerror.c
)

add_library(cwinapi_static STATIC
  ${winapi_src}
)

link_directories(AFTER ${modernlib_BINARY_DIR})
target_include_directories(cwinapi_static AFTER
  PUBLIC ../include/
)
target_compile_definitions(cwinapi_static
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0501)
target_link_libraries(cwinapi_static PUBLIC modernlib_static)

add_library(cwinapi SHARED
  ${winapi_src}
)
target_include_directories(cwinapi AFTER
  PUBLIC ../include/
)
target_compile_definitions(cwinapi
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0501
)
target_link_options(cwinapi PRIVATE -v)
target_link_libraries(cwinapi PUBLIC modernlib)
