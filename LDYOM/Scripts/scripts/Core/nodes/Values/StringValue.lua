local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class StringValueNode
---@field static table

---@type BaseNode | StringValueNode
local StringValueNode = class("StringValueNode", ed.BaseNode)
StringValueNode.static.name_ = "core.nodes.string_value";
StringValueNode.static.type = ed.NodeType.Constant;

---@param id integer
function StringValueNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.Out, ed.ValueType.String, "", constants.TEXT_SIZE)
    }
end

function StringValueNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginOutputAttribute(self.pins[1]);
    ImGui.inputTextMultiline("##value", self.pins[1].value, constants.TEXT_SIZE, 120, 50)
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function StringValueNode:execute(scene, tasklist, context)
end

NodeEditor.registerNode("core.categories.values", StringValueNode);