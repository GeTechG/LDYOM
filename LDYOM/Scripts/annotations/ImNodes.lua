---@meta

ImNodes = {
    ---@param id integer
    ---@param name string
    ---@param nodeType NodeType
    beginNode = function (id, name, nodeType) end,
    ---@param id integer
    beginNodeNoTitle = function (id) end,
    ---@param pin Pin
    beginInputAttribute = function(pin) end,
    endInputAttribute = function() end,
    ---@param pin Pin
    beginOutputAttribute = function(pin) end,
    endOutputAttribute = function() end,
    endNode = function() end,
    endNodeNoTitle = function() end,
    ---@param nodeId integer
    ---@return ImVec2
    getNodeDimensions = function(nodeId) end,
    ---@return userdata
    editorContextCreate = function() end,
    ---@param nodeId integer
    ---@return ImVec2
    getNodeEditorSpacePos = function(nodeId) end
}