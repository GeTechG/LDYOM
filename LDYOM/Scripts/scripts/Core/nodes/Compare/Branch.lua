local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class BranchNode
---@field static table
---@field conditionType ffi.cdata*

---@type BaseNode | BranchNode
local BranchNode = class("BranchNode", ed.BaseNode)
BranchNode.static.name_ = "core.nodes.branch";
BranchNode.static.type = ed.NodeType.Function;

---@param id integer
function BranchNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        ed.Pin:new(id + 2, "general.condition", ed.PinType.In, ed.ValueType.Boolean),
        ed.Pin:new(id + 3, "general.true", ed.PinType.Out, ed.ValueType.Void),
        ed.Pin:new(id + 4, "general.false", ed.PinType.Out, ed.ValueType.Void)
    }
end

function BranchNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[2]);
    ImGui.text(loc.get(self.pins[2].name));
    ImNodes.endInputAttribute();

    ImGui.endGroup();

    ImGui.sameLine();

    local inp_size = ImGui.getItemRectSize();

    ImGui.beginGroup();

    ImNodes.beginOutputAttribute(self.pins[3]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[3].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(loc.get(self.pins[3].name));
    ImNodes.endOutputAttribute();

    ImNodes.beginOutputAttribute(self.pins[4]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[4].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(loc.get(self.pins[4].name));
    ImNodes.endOutputAttribute();

    ImGui.endGroup();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function BranchNode:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    local cond = context:getValue(scene, tasklist, self.id + 2, queryId)[0];
    if cond then
        context:callNodes(scene, tasklist, self.id + 3);
    else
        context:callNodes(scene, tasklist, self.id + 4);
    end
end

NodeEditor.registerNode("core.categories.compare", BranchNode);