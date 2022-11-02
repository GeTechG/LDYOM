local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class EmitSignalNode
---@field static table

---@type BaseNode | EmitSignalNode
local EmitSignalNode = class("EmitSignalNode", ed.BaseNode)
EmitSignalNode.static.name_ = "core.nodes.emit_signal";
EmitSignalNode.static.type = ed.NodeType.Function;

---@param id integer
function EmitSignalNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function EmitSignalNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImGui.endGroup();

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[2]);
    ImGui.text(self.pins[2].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function EmitSignalNode:execute(scene, tasklist, context)

    emitSignal();

    context:callNodes(scene, tasklist, self.id + 2);
end

NodeEditor.registerNode("core.categories.main", EmitSignalNode);