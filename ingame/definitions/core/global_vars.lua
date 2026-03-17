---@meta
---@class global_vars
global_vars = {}
---@param name string
---@return number|string|boolean|nil
function global_vars.get(name) end
---@param name string
---@param value number|string|boolean
function global_vars.set(name, value) end
---@param name string
---@return boolean
function global_vars.exists(name) end
function global_vars.reset() end
---@param name string
---@return number|string|boolean|nil
function global_vars.getDefault(name) end
---@param name string
---@param value number|string|boolean
function global_vars.setDefault(name, value) end
