---@meta

---@class ImVec2
---@field x number
---@field y number

---@class ImVec4
---@field x number
---@field y number
---@field z number
---@field w number

ImGui = {
    ---@param text string
    text = function (text) end,
    sameLine = function() end,
    ---@param offset float
    sameLine = function(offset) end,
    ---@return number x
    ---@return number y
    ---@return number z
    getMousePos = function() end,
    ---@param name string
    ---@param buf ffi.cdata*
    ---@param bufSize integer
    ---@return bool
    inputText = function(name, buf, bufSize) end,
    ---@param name string
    ---@param buf ffi.cdata*
    ---@param bufSize integer
    ---@param width number
    ---@param height number
    ---@return bool
    inputTextMultiline = function(name, buf, bufSize, width, height) end,
    ---@param width number
    setNextItemWidth = function(width) end,
    ---@return number fontSize
    getFontSize = function() end,
    ---@return ImVec2
    getItemRectSize = function() end,
    getStyleColorVec4 = function(col) end,
    beginGroup = function() end,
    endGroup = function() end,
    ---@param spacing number
    space = function(spacing) end,
    ---@param text string
    calcTextSize = function(text) end,
    ---@return boolean
    isItemHovered = function() end,
    beginTooltip = function() end,
    endTooltip = function() end
}


---@param r integer
---@param g integer
---@param b integer
---@param a integer
---@return integer
IM_COL32 = function (r, g, b, a) end