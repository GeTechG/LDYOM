local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local imgui_operators = require("libs.imgui_operators")

---@class LDNodeEditorCommentNode : LDNodeEditorNode
---@field comment string

local COMMENT_NODE_TYPE = "core.comment"

---@type LDNodeEditorNodeType
local commentNode = {
    typeName = COMMENT_NODE_TYPE,
    category = "main",
    icon = fa.ICON_FA_STICKY_NOTE,
    color = ImVec4.new(191/255, 188/255, 182/255, 1),
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCommentNode
        local newNode = {
            id = newNodeId,
            nodeType = COMMENT_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
                },
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                },
            },
            comment = "Some text here"
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCommentNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        local commentAlpha = 0.75

        ImGui.PushStyleVar(ImGuiStyleVar.Alpha, commentAlpha)
        NodeEditor.PushStyleColor(NodeEditorStyleColor.NodeBg, ImVec4.new(1, 1, 1, 64/255))
        NodeEditor.PushStyleColor(NodeEditorStyleColor.NodeBorder, ImVec4.new(1, 1, 1, 64/255))
        NodeEditor.BeginNode(NodeEditor.NodeId(node.id))
        ImGui.PushID(node.id)
        ImGui.BeginVertical("content", ImVec2.new(), -1);
        ImGui.Text(node.comment)
        ImGui.EndVertical()
        ImGui.PopID()

        if ImGui.Button(ld.loc.get("nodes.core.comment.edit"), ImVec2.new(0, 0)) then
            NodeEditor.Suspend()
            ImGui.OpenPopup("##commentTextEdit", 0);
            NodeEditor.Resume()
        end

        NodeEditor.EndNode()
        NodeEditor.PopStyleColor(2)
        ImGui.PopStyleVar(1)

        NodeEditor.Suspend()
        if ImGui.BeginPopupModal("##commentTextEdit", nil, ImGuiWindowFlags.AlwaysAutoResize) then
            local text = node.comment
            local textChanged, newText = ImGui.InputTextMultiline("##comment", text, ImVec2.new(400, 200), ImGuiInputTextFlags.AllowTabInput)
            if textChanged then
                node.comment = newText
            end
            if ImGui.Button(ld.loc.get("general.ok"), ImVec2.new(100, 0)) then
                ImGui.CloseCurrentPopup()
            end
            ImGui.EndPopup()
        end
        NodeEditor.Resume()

    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCommentNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)

        return {1}
    end
}

return commentNode
