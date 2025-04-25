#pragma once
#include <plugin.h>
#include <string>

inline constexpr const char* LDYOM_ROOT_DIR = "LDYOM";

#define LDYOM_PATH(path) std::string(PLUGIN_PATH((char*)"")) + LDYOM_ROOT_DIR + "/" + std::string(path)