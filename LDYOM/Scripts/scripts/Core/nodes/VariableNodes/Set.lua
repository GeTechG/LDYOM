local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class SetVariableNode
---@field static table
---@field varId integer

---@type BaseNode | SetVariableNode
local SetVariableNode = class("SetVariableNode", ed.BaseNode)
SetVariableNode.static.name_ = "core.nodes.variable_set";
SetVariableNode.static.type = ed.NodeType.Variable;

---@param id integer
function SetVariableNode:initialize(id, varId)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.varId = varId;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void)
    };
    local varType = global_data["variables"][varId].type;
    if varType == ValueType.Number or varType == ValueType.Boolean then
        self.pins[#self.pins+1] = ed.Pin:new(id + 2, "", ed.PinType.In, varType);
    elseif varType == ValueType.String then
        self.pins[#self.pins+1] = ed.Pin:new(id + 2, "", ed.PinType.In, varType, "", constants.TEXT_SIZE);
    end
    self.pins[#self.pins+1] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void);
end

function SetVariableNode:draw()
    ImNodes.beginNode(self.id, loc.get(self.class.name_) .. " : " .. ffi.string(global_data["variables"][self.varId].name), self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[2]);
    ImGui.text(self.pins[2].name);
    ImNodes.endInputAttribute();

    ImGui.endGroup();

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[3]);
    ImGui.text(self.pins[3].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function SetVariableNode:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    if global_data["variables"][self.varId].type == ValueType.Number or global_data["variables"][self.varId].type == ValueType.Boolean then
        global_data["variablesR"][self.varId][0] = context:getValue(scene, tasklist, self.id + 2, queryId)[0];
    else
        ffi.copy(global_data["variablesR"][self.varId], context:getValue(scene, tasklist, self.id + 2, queryId), constants.TEXT_SIZE);
    end

    context:callNodes(scene, tasklist, self.id + 3);
end

global_data.SetVariableNode = SetVariableNode;

bitser.registerClass(SetVariableNode);
--NodeEditor.registerNode("core.categories.variables", SetVariableNode);