---@meta

---@class bitser
bitser = {
    ---Basic serialization of value into a Lua string.
    ---@param value any
    ---@return string
    dumps = function(value) end,
    ---Deserializes value from string.
    ---@param string string
    ---@return any
    loads = function (string) end,
    ---Registers the class class, so that bitser can correctly serialize and deserialize instances of class.
    ---@param class any
    registerClass = function(class) end
}