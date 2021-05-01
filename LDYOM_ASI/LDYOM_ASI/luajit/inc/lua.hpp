#pragma once
// C++ wrapper for LuaJIT header files.

#define SOL_LUAJIT 1
#define SOL_USING_CXX_LUAJIT 1
#define SOL_USE_CXX_LUAJIT 1

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

