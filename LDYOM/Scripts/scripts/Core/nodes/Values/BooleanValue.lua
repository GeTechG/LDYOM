local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class BooleanValueNode
---@field static table

---@type BaseNode | BooleanValueNode
local BooleanValueNode = class("BooleanValueNode", ed.BaseNode)
BooleanValueNode.static.name_ = "core.nodes.boolean_value";
BooleanValueNode.static.type = ed.NodeType.Constant;

---@param id integer
function BooleanValueNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.Out, ed.ValueType.Boolean)
    }
end

function BooleanValueNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginOutputAttribute(self.pins[1]);
    ImGui.checkbox("##value", self.pins[1].value)
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function BooleanValueNode:execute(scene, tasklist, context)
end

NodeEditor.registerNode("core.categories.values", BooleanValueNode);