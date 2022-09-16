local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class NumberValueNode
---@field static table

---@type BaseNode | NumberValueNode
local NumberValueNode = class("NumberValueNode", ed.BaseNode)
NumberValueNode.static.name_ = "core.nodes.number_value";
NumberValueNode.static.type = ed.NodeType.Constant;

---@param id integer
function NumberValueNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.type = ed.NodeType.Constant;
    self.activation = false;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function NumberValueNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginOutputAttribute(self.pins[1]);
    ImGui.setNextItemWidth(120);
    ImGui.inputFloat("##value", self.pins[1].value)
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function NumberValueNode:execute(scene, tasklist, context)
end

NodeEditor.registerNode("core.categories.values", NumberValueNode);