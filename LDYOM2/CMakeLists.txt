cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_GENERATOR_PLATFORM Win32)
endif()

if (MSVC_TOOLSET_VERSION)
    message("-- MSVC_TOOLSET_VERSION: ${MSVC_TOOLSET_VERSION}")
endif()

project(ldyom C CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(SOL2_LUA_VERSION "libluajit" CACHE STRING "")
set(SOL2_BUILD_LUA OFF CACHE BOOL "")
set(SOL2_PLATFORM "x86" CACHE STRING "")
set(SOL2_TESTS_EXAMPLES OFF CACHE BOOL "")
set(BUILD_LUA_AS_DLL OFF)
set(LUAJIT_FORCE_UTF8_FOPEN ON)
set(LUAJIT_BUILD_TOOL OFF)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(thirdparty/sol2)
add_subdirectory(thirdparty/LuaJIT)

find_package(plugin REQUIRED)
find_package(minhook REQUIRED)
find_package(imgui REQUIRED)
find_package(i18ncpp REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(stduuid)
find_package(watcher)

if(NOT DEFINED ENV{DIRECTX9_SDK_DIR})
    message(FATAL_ERROR "DIRECTX9_SDK_DIR environment variable is not set. Please set it to your DirectX SDK installation path.")
else()
    message(STATUS "Using DirectX SDK from: $ENV{DIRECTX9_SDK_DIR}")
    # Add DirectX SDK Include directory
    include_directories($ENV{DIRECTX9_SDK_DIR}/Include)
    # Add DirectX SDK 32-bit Library directory
    link_directories($ENV{DIRECTX9_SDK_DIR}/Lib/x86)
endif()

file(GLOB_RECURSE SRC_FILES "src/*")
add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    d3d9.lib
    d3dx9.lib
    plugin::plugin_sa
    minhook::minhook
    imgui::imgui
    dinput8
    dxguid
    i18ncpp::i18ncpp
    spdlog::spdlog
    fmt::fmt
    stduuid::stduuid
    watcher::watcher
    sol2::sol2
    libluajit
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gta_data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_hook
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fonts
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua
)

set(GTA_SA_DIR "C:/Games/GTA San Andreas")

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<NOT:$<OR:$<CONFIG:Release>,$<CONFIG:Debug>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    SUFFIX ".asi"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${GTA_SA_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${GTA_SA_DIR}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${GTA_SA_DIR}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${GTA_SA_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${GTA_SA_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GTA_SA_DIR}"
    COMPILE_WARNING_AS_ERROR OFF
    PDB_OUTPUT_DIRECTORY_DEBUG "${GTA_SA_DIR}"
    PDB_OUTPUT_DIRECTORY_RELEASE "${GTA_SA_DIR}"
    COMPILE_PDB_NAME_DEBUG "${PROJECT_NAME}"
    COMPILE_PDB_NAME_RELEASE "${PROJECT_NAME}"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "_CRT_SECURE_NO_WARNINGS;"
    "_CRT_NON_CONFORMING_SWPRINTFS;"
    "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING;"
    "_DX9_SDK_INSTALLED;"
    "GTASA;"
    "PLUGIN_SGV_10US;"
    "RW;"
    "_MBCS;"
    "SOL_LUAJIT=1"  # Enable LuaJIT support in sol2
    SOL_ALL_SAFETIES_ON=1
    SOL_USE_LUAJIT=1
    SOL_NO_NIL=1
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Gm-;
            /Ox;
            /Oi;
            /GF;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        $<$<CONFIG:Debug>:
            /Od;
            /ZI;
            /Oy-; 
            /EHa; 
            /Fa;
        >
        /std:c++latest;
        /W3;
        /wd4073;
        /sdl-;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
endif()

target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:
        /OPT:REF;
        /OPT:ICF
    >
    $<$<CONFIG:Debug>:
        /DEBUG:FULL;
    >
    /MACHINE:X86;
    /SUBSYSTEM:WINDOWS;
    /SAFESEH:NO
)

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_COMMAND "${GTA_SA_DIR}/gta_sa.exe"
        VS_DEBUGGER_WORKING_DIRECTORY "${GTA_SA_DIR}"
    )
endif()
