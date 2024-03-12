---@meta

Dirs = {
    --- Returns a table of entries in the specified directory.
    ---@param luaPath string
    ---@param recursive boolean
    ---@return string[]
    entries = function (luaPath, recursive) end,
    --- Returns whether the specified directory exists.
    ---@param luaPath string
    ---@return boolean
    exists = function (luaPath) end,
    --- Creates a directory at the specified path.
    create = function (luaPath) end,
    --- Removes the directory at the specified path.
    remove = function (luaPath) end,
}