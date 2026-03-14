---@meta

Dirs = {}

---Returns a list of entry names in the specified directory.
---All paths are relative to the LDYOM plugin root.
---@param luaPath string Path relative to the LDYOM root
---@param recursive boolean Whether to list entries recursively
---@return string[]
function Dirs.entries(luaPath, recursive) end

---Returns whether a path exists (file or directory).
---@param luaPath string Path relative to the LDYOM root
---@return boolean
function Dirs.exists(luaPath) end

---Returns whether the path points to a regular file.
---@param luaPath string Path relative to the LDYOM root
---@return boolean
function Dirs.isFile(luaPath) end

---Returns whether the path points to a directory.
---@param luaPath string Path relative to the LDYOM root
---@return boolean
function Dirs.isDir(luaPath) end

---Creates a directory (and all intermediate directories) at the given path.
---@param luaPath string Path relative to the LDYOM root
---@return boolean success
function Dirs.create(luaPath) end

---Removes a file or directory (recursively) at the given path.
---@param luaPath string Path relative to the LDYOM root
---@return boolean success
function Dirs.remove(luaPath) end

---Renames or moves a file or directory.
---@param fromPath string Source path relative to the LDYOM root
---@param toPath string Destination path relative to the LDYOM root
---@return boolean success
function Dirs.rename(fromPath, toPath) end

---Reads the entire contents of a file as a string.
---Returns nil if the file does not exist or cannot be read.
---@param luaPath string Path relative to the LDYOM root
---@return string|nil
function Dirs.readFile(luaPath) end

---Writes (overwrites) content to a file, creating parent directories as needed.
---@param luaPath string Path relative to the LDYOM root
---@param content string Content to write
---@return boolean success
function Dirs.writeFile(luaPath, content) end

---Appends content to a file.
---@param luaPath string Path relative to the LDYOM root
---@param content string Content to append
---@return boolean success
function Dirs.appendFile(luaPath, content) end
