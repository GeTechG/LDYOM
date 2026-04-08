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
find_package(stduuid REQUIRED)
find_package(watcher REQUIRED)
find_package(glm REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(Tracy REQUIRED)

option(LDYOM_BACKEND_DX11 "Enable DirectX 11 rendering backend" OFF)
option(LDYOM_BACKEND_OPENGL "Enable OpenGL rendering backend" OFF)

add_subdirectory(modules/ImGuiLuaWrapper)
add_subdirectory(modules/OpcodesLuaWrapper)

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

if(NOT LDYOM_BACKEND_DX11)
    list(FILTER SRC_FILES EXCLUDE REGEX "imgui_impl_dx11")
endif()
if(NOT LDYOM_BACKEND_OPENGL)
    list(FILTER SRC_FILES EXCLUDE REGEX "imgui_impl_opengl")
endif()
set(IMGUIZMO_FILES
    thirdparty/ImGuizmo/ImGuizmo.cpp
)
set(IMNODEFLOW_FILES
    thirdparty/ImNodeFlow/src/ImNodeFlow.cpp
)
add_library(${PROJECT_NAME} MODULE ${SRC_FILES} ${IMGUIZMO_FILES} ${IMNODEFLOW_FILES})

# Auto-embed all .lua files from lua/ directory.
# Each file becomes a header: lua/foo.lua -> generated/foo_lua.h -> embedded::foo_lua
set(EMBED_LUA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lua")
file(GLOB EMBED_LUA_FILES "${EMBED_LUA_DIR}/*.lua")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${EMBED_LUA_DIR}")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")

foreach(lua_file ${EMBED_LUA_FILES})
    get_filename_component(name_we "${lua_file}" NAME_WE)
    string(MAKE_C_IDENTIFIER "${name_we}" var_name)
    set(var_name "${var_name}_lua")
    set(output_header "${CMAKE_CURRENT_BINARY_DIR}/generated/${var_name}.h")
    add_custom_command(
        OUTPUT  "${output_header}"
        COMMAND ${CMAKE_COMMAND}
            -DINPUT_FILE=${lua_file}
            -DOUTPUT_FILE=${output_header}
            -DVAR_NAME=${var_name}
            -P "${CMAKE_SOURCE_DIR}/cmake/EmbedFile.cmake"
        DEPENDS "${lua_file}"
        COMMENT "Embedding ${lua_file} -> generated/${var_name}.h"
        VERBATIM
    )
    target_sources(${PROJECT_NAME} PRIVATE "${output_header}")
endforeach()

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
    glm::glm
    SQLiteCpp
    Tracy::TracyClient
    ImGuiLuaWrapper
    OpcodesLuaWrapper
)

if(LDYOM_BACKEND_DX11)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.lib dxgi.lib)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LDYOM_BACKEND_DX11)
endif()

if(LDYOM_BACKEND_OPENGL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LDYOM_BACKEND_OPENGL)
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /FS)
endif()

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/node_editor
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ImGuizmo
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ImNodeFlow/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/ImNodeFlow/src
)

set(GTA_SA_DIR "C:/Games/GTA San Andreas")
set(GTA_SA_CORE_ADDON_DIR "${GTA_SA_DIR}/LDYOM/addons/core")
set(OPCODE_NODES_SRC "${CMAKE_CURRENT_SOURCE_DIR}/modules/OpcodesLuaWrapper/generator/output/nodes/opcode")
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND}
#         -DSRC=${OPCODE_NODES_SRC}
#         -DDST=${GTA_SA_CORE_ADDON_DIR}/nodes/opcode
#         -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/copy_if_exists.cmake"
#     COMMENT "Deploying opcode nodes to skic.core addon"
# )

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

set(LDYOM_VERSION_STRING "0.9.1")
configure_file(src/version.h.in src/version.h @ONLY)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src)
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
    $<$<CONFIG:Debug>:TRACY_ENABLE>
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
