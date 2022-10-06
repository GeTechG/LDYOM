local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"

---@class InvertNode
---@field static table

---@type BaseNode | InvertNode
local InvertNode = class("InvertNode", ed.BaseNode)
InvertNode.static.name_ = "core.nodes.invert_node";
InvertNode.static.type = ed.NodeType.Function;

---@param id integer
function InvertNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Boolean),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Boolean)
    }
end

function InvertNode:draw()
    ImNodes.beginNodeNoTitle(self.id);

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImGui.sameLine();

    ImNodes.beginStaticAttribute(self.id + 2);
    ImGui.text('¬');
    ImNodes.endStaticAttribute();

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[3]);
    ImGui.text(self.pins[3].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNodeNoTitle();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function InvertNode:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    local value = context:getValue(scene, tasklist, self.id + 1, queryId)[0];
    self.pins[3].value[0] = not value;
end

NodeEditor.registerNode("core.categories.main", InvertNode);