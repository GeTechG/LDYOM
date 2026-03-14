---
name: ldyom-definitions
description: Generate Lua declaration files (LuaLS/EmmyLua ---@meta annotations for IDE/LSP) from C++ sol2 bindings in the LDYOM2 GTA San Andreas mission editor project. Use this skill whenever the user wants to create, update, or regenerate Lua definition files from C++ sol2 binding code (files in src/lua/), or when they say "generate declarations", "create definitions", "update lua definitions", "add IDE support for binding", or ask why autocomplete is missing for a Lua API they just added in C++. Always trigger for any LDYOM Lua definition/declaration tasks.
---

# LDYOM Lua Definition Generator

Generates Lua declaration files (`.lua` with `---@meta`) from C++ sol2 binding files in the LDYOM2 project. These give IDE/LSP (LuaLS / EmmyLua) autocomplete and type checking to Lua addon scripts.

## Project layout

```
LDYOM2/src/lua/          ← C++ binding files (*_binding.cpp / *_binding.h)
ingame/definitions/core/ ← Output .lua declarations (sibling of LDYOM2/)
```

Reference existing declarations (`fs.lua`, `logger.lua`, `localization.lua`) as style examples.

## Workflow

1. **Read** the C++ binding file(s) the user points to (or all `*_binding.cpp` if they say "all")
2. **Parse** sol2 patterns (see below) to discover what the binding exposes to Lua
3. **Generate** the `.lua` declaration file using LuaLS annotation format
4. **Write** to `../ingame/definitions/core/<tablename>.lua` relative to `LDYOM2/`
5. **Report** what was generated: table/class name, function count, any type-mapping assumptions

If the binding is not specified, ask — or infer from context (e.g., user just wrote a new binding file).

---

## Parsing sol2 patterns

### Global Lua table

```cpp
lua.create_table("tableName")
state.create_named_table("tableName")
auto t = state.create_table("tableName")
lua["tableName"] = lua.create_table()
```
→ `tableName = {}`

### Function registration

```cpp
// Assignment lambda
table["funcName"] = [](const std::string& msg) { ... };

// set_function
table.set_function("funcName", [](const char* path, bool recursive) -> sol::optional<std::string> { ... });
```
→ `function tableName.funcName(params) end` with annotations

### sol2 usertype

```cpp
auto utype = lua.new_usertype<MyClass>("MyClass");
utype["method"] = &MyClass::method;
utype["prop"]   = sol::property(&MyClass::get, &MyClass::set);
```
→ `---@class MyClass` with `---@field` entries and method stubs

### Two-layer pattern (embedded Lua script)

Sometimes a binding executes a Lua script via `lua.script(R"(...)")` to expose a clean public API on top of raw C++ functions. Parse **the embedded Lua string** for the actual public API — those are the functions to declare, not the underscore-prefixed raw ones.

### Static `sol_lua_register` method

Some types self-register: `Entity::sol_lua_register(lua)`. Find and read the class header to locate the `new_usertype<T>` call and extract all registered methods/fields.

---

## C++ → Lua type mapping

| C++ type | LuaLS annotation |
|---|---|
| `std::string`, `const std::string&`, `const char*` | `string` |
| `bool` | `boolean` |
| `int`, `size_t`, `uint32_t`, `uint64_t` | `integer` |
| `float`, `double` | `number` |
| `void` (return) | omit `---@return` |
| `sol::optional<T>` | `T\|nil` |
| `sol::table` | `table` |
| `sol::variadic_args` (all variadic) | `---@param ... any` and `...` in signature |
| `sol::variadic_args` where `va[0]` is format string | `---@param format string` + `---@param ... any` |
| `std::vector<T>` / `sol::as_table(vector<T>)` | `T[]` |
| Custom class `T*` / `T&` | the class name, e.g. `Entity` |

**Nullable returns**: when the function returns `sol::nullopt` or early-returns a default, annotate `T|nil` and note "Returns nil on error/invalid path" in the doc comment.

---

## LuaLS annotation format

Use these annotations (LuaLS / EmmyLua compatible):

| Annotation | Purpose |
|---|---|
| `---@meta` | Marks file as a declaration-only file (no runtime code) |
| `---@class Name[:Parent]` | Defines a class / usertype |
| `---@field [scope] name type [desc]` | Field on a class |
| `---@param name[?] type [desc]` | Function parameter (`?` = optional) |
| `---@param ... type` | Variadic parameter (replaces deprecated `---@vararg`) |
| `---@return type [name] [desc]` | Return value |
| `---@overload fun(...):<ret>` | Additional overload signature |
| `---@alias Name type` | Type alias |
| `---@enum Name` | Enum table |
| `---@type type` | Variable type annotation |
| `---@nodiscard` | Warn if return value is ignored |
| `---@deprecated` | Mark as deprecated |

> **Note**: `---@vararg` is deprecated in LuaLS — always use `---@param ... type` instead.

---

## Output file structure

```lua
---@meta

TableName = {}

---Brief description of what this function does.
---Additional detail if needed (e.g. path sandboxing, error behavior).
---@param paramName type Description
---@param paramName2 type Description
---@return type
function TableName.funcName(paramName, paramName2) end

---Next function...
---@param ... any Format arguments
---@return boolean success
function TableName.nextFunc(...) end
```

Rules:
- `---@meta` is always the very first line, no blank line before it
- One blank line between the global `TableName = {}` declaration and first function
- One blank line between consecutive function blocks
- Derive doc comments from C++ comments above the lambda; if absent, infer from name + body behavior
- Never include implementation bodies — all function bodies are just `end`
- For variadic format functions (e.g. `log.tracef`): signature is `function log.tracef(format, ...) end`
- For table returns with known shape, describe inline: `---@return {index: integer, name: string, type: string}[]`

---

## Example

**Input** (`log_bindings.cpp`):
```cpp
sol::table log = lua.create_table("log");
log["trace"] = [](const std::string& msg) { LDYOM_TRACE(msg); };
log["tracef"] = [args_to_strings](const sol::variadic_args& va) {
    // va[0] = format string, rest = substitution values
    std::string msg = fmt::vformat(va.get<std::string>(0), args_to_strings(va, 1));
    LDYOM_TRACE(msg);
};
```

**Output** (`logger.lua`):
```lua
---@meta

log = {}

---Log a trace message
---@param msg string The message to log
function log.trace(msg) end

---Log a formatted trace message using fmt-style placeholders
---@param format string The format string (e.g. "hello {}")
---@param ... string Substitution values
function log.tracef(format, ...) end
```

---

## After writing the file

Tell the user:
- The output file path
- List of declared functions/classes
- Any type mappings you weren't certain about (so they can correct them)
