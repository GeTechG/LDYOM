---@class LDNodeEditorNodeType
---@field typeName string
LDNodeEditorNodeType = {
    ---@param ctx LDNodeEditorContext
    ---@return LDNodeEditorNode
    new = function(ctx) end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder) end,
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorNode
    run = function(ctx, node) end
}