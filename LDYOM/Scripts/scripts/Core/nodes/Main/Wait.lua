local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class WaitNode
---@field static table

---@type BaseNode | WaitNode
local WaitNode = class("WaitNode", ed.BaseNode)
WaitNode.static.name_ = "core.nodes.wait_node";
WaitNode.static.type = ed.NodeType.Function;

---@param id integer
function WaitNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        ed.Pin:new(id + 2, "general.time", ed.PinType.In, ed.ValueType.Number),
        ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function WaitNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[2]);
    ImGui.text(loc.get(self.pins[2].name));
    ImGui.setNextItemWidth(120);
    ImGui.inputFloat("##timeValue", self.pins[2].value)
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
function WaitNode:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    local time = context:getValue(scene, tasklist, self.id + 2, queryId)[0];
    
    coroutine.yield(math.floor(time * 1000));
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.main", WaitNode);