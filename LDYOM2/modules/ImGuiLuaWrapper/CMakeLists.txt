cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(ImGuiLuaWrapper CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Все зависимости (imgui::imgui, sol2::sol2, libluajit) определены в родителе

# ── cimgui: только cimgui.cpp, ImGui:: символы из imgui::imgui (Conan) ──
add_library(cimgui STATIC cimgui/cimgui.cpp)
target_include_directories(cimgui PUBLIC cimgui)
target_link_libraries(cimgui PUBLIC imgui::imgui)
target_compile_definitions(cimgui INTERFACE CIMGUI_DEFINE_ENUMS_AND_STRUCTS)
string(CONCAT MSVC_RT_STR
    $<$<CONFIG:Release>:MultiThreaded>
    $<$<CONFIG:Debug>:MultiThreadedDebug>
)
set_target_properties(cimgui PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RT_STR})

# ── Сгенерированные биндинги (коммитятся, пересобирать только при обновлении ImGui) ──
file(GLOB_RECURSE GENERATED_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/generator/output/cpp/enums/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/generator/output/cpp/structs/*.cpp"
)

# ── Библиотека ──
add_library(ImGuiLuaWrapper STATIC
    src/ImGuiLuaWrapper.cpp
    src/customImGuiWrapper.cpp
    src/customCimguiWrapper.cpp
    src/customImColor.cpp
    generator/output/cpp/enums.cpp
    generator/output/cpp/structs.cpp
    ${GENERATED_SOURCES}
)

target_include_directories(ImGuiLuaWrapper
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/cimgui
            ${CMAKE_CURRENT_SOURCE_DIR}/cimgui/imgui/misc/cpp
)

target_link_libraries(ImGuiLuaWrapper PUBLIC
    cimgui
    imgui::imgui
    sol2::sol2
    libluajit
)

target_compile_definitions(ImGuiLuaWrapper PUBLIC
    CIMGUI_DEFINE_ENUMS_AND_STRUCTS
    SOL_LUAJIT=1
    SOL_ALL_SAFETIES_ON=1
    SOL_USE_LUAJIT=1
    SOL_NO_NIL=1
)

set_target_properties(ImGuiLuaWrapper PROPERTIES
    MSVC_RUNTIME_LIBRARY ${MSVC_RT_STR}
)

# Generated files with heavy sol2 template instantiations need /bigobj on MSVC
if(MSVC)
    target_compile_options(ImGuiLuaWrapper PRIVATE /bigobj)
endif()
