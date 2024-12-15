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
set_target_properties(cwinapi PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_include_directories(cwinapi AFTER
  PUBLIC ../include/
)
target_compile_definitions(cwinapi
  PRIVATE UNICODE _UNICODE _WIN32_WINNT=0x0501
)
target_link_libraries(cwinapi PUBLIC modernlib)
install(TARGETS cwinapi DESTINATION ${CMAKE_BINARY_DIR})
install(TARGETS cwinapi_static DESTINATION ${CMAKE_BINARY_DIR})

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
