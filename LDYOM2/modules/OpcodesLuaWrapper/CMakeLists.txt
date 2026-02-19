cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(OpcodesLuaWrapper CXX)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE GENERATED_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/generator/output/cpp/opcodes/*.cpp"
)

add_library(OpcodesLuaWrapper STATIC
    src/OpcodesLuaWrapper.cpp
    generator/output/cpp/OpcodesLuaWrapper.cpp
    ${GENERATED_SOURCES}
)

target_include_directories(OpcodesLuaWrapper PUBLIC include)

target_link_libraries(OpcodesLuaWrapper PUBLIC
    plugin::plugin_sa
    sol2::sol2
    libluajit
)

target_compile_definitions(OpcodesLuaWrapper PUBLIC
    SOL_LUAJIT=1 SOL_ALL_SAFETIES_ON=1 SOL_USE_LUAJIT=1 SOL_NO_NIL=1
    GTASA PLUGIN_SGV_10US RW _MBCS _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS
)

string(CONCAT MSVC_RT_STR
    $<$<CONFIG:Release>:MultiThreaded>
    $<$<CONFIG:Debug>:MultiThreadedDebug>
)
set_target_properties(OpcodesLuaWrapper PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RT_STR})

if(MSVC)
    target_compile_options(OpcodesLuaWrapper PRIVATE /bigobj)
endif()
