---@class LDNodeEditorNodeType
---@field typeName string
---@field icon string | nil
---@field color ImVec4
---@field category string
---@field isCallable boolean
LDNodeEditorNodeType = {
    __class = "LDNodeEditorNodeType",
    ---@param ctx LDNodeEditorContext
    ---@param neNodeId integer
    ---@param getPinId fun():integer
    ---@param editor LDNodeEditor
    ---@return LDNodeEditorNode
    new = function(ctx, neNodeId, getPinId, editor) end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder) end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorNode
    ---@param inputValues any[]
    ---@return any[]
    run = function(editor, context, node, inputValues) end
}