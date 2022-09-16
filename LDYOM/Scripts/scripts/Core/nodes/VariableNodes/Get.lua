local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class GetVariableNode
---@field static table
---@field varId integer

---@type BaseNode | GetVariableNode
local GetVariableNode = class("GetVariableNode", ed.BaseNode)
GetVariableNode.static.name_ = "core.nodes.variable_get";
GetVariableNode.static.type = ed.NodeType.Variable;

---@param id integer
function GetVariableNode:initialize(id, varId)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.varId = varId;
    self.pins = {};
    local varType = global_data["variables"][varId].type;
    if varType == ValueType.Number or varType == ValueType.Boolean then
        self.pins[#self.pins+1] = ed.Pin:new(id + 1, "", ed.PinType.Out, varType);
    elseif varType == ValueType.String then
        self.pins[#self.pins+1] = ed.Pin:new(id + 1, "", ed.PinType.Out, varType, "", constants.TEXT_SIZE);
    end
end

function GetVariableNode:draw()
    ImNodes.beginNode(self.id, loc.get(self.class.name_) .. " : " .. ffi.string(global_data["variables"][self.varId].name), self.class.type);

    ImNodes.beginOutputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function GetVariableNode:execute(scene, tasklist, context)
    if global_data["variables"][self.varId].type == ValueType.Number or global_data["variables"][self.varId].type == ValueType.Boolean then
        self.pins[1].value[0] = global_data["variablesR"][self.varId][0];
    else
        ffi.copy(self.pins[1], global_data["variablesR"][self.varId]);
    end
end

global_data.GetVariableNode = GetVariableNode;

bitser.registerClass(GetVariableNode);
--NodeEditor.registerNode("core.categories.variables", GetVariableNode);