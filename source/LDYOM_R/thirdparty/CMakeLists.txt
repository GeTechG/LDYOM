project(thirdparty)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

set(CMAKE_C_FLAGS_RELEASE "/MT")
set(CMAKE_C_FLAGS_DEBUG "/MTd")

add_subdirectory(xbyak)
add_subdirectory(ktsignal)
add_subdirectory(kthook)
add_subdirectory(glm)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOURCE_DIR}/lib)
