set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(HOST_TRIPLET aarch64-w64-mingw32)
set(CMAKE_C_COMPILER aarch64-w64-mingw32-clang)
set(CMAKE_C_COMPILER_TARGET ${HOST_TRIPLET})
set(CMAKE_CXX_COMPILER aarch64-w64-mingw32-clang++)
set(CMAKE_CXX_COMPILER_TARGET ${HOST_TRIPLET})
