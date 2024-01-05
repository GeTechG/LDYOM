---@meta

Dir = {
    --- Returns a table of entries in the specified directory.
    ---@param luaPath string
    ---@return string[]
    entries = function (luaPath) end,
    --- Returns whether the specified directory exists.
    ---@param luaPath string
    ---@return boolean
    exists = function (luaPath) end
}