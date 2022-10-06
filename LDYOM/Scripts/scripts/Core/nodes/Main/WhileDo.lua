local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"

---@class WhileDoNode
---@field static table

---@type BaseNode | WhileDoNode
local WhileDoNode = class("WhileDoNode", ed.BaseNode)
WhileDoNode.static.name_ = "core.nodes.while_do";
WhileDoNode.static.type = ed.NodeType.Loops;

---@param id integer
function WhileDoNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        ed.Pin:new(id + 2, "general.condition", ed.PinType.In, ed.ValueType.Boolean),
        ed.Pin:new(id + 3, "core.pins.while", ed.PinType.Out, ed.ValueType.Void),
        ed.Pin:new(id + 4, "core.pins.after", ed.PinType.Out, ed.ValueType.Void),
    }
end

function WhileDoNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginInputAttribute(self.pins[2]);
    if not self.pins[2].connected then
        ImGui.checkbox(loc.get(self.pins[2].name), self.pins[2].value);
    else
        ImGui.text(loc.get(self.pins[2].name));
    end
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
function WhileDoNode:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);

    while context:getValue(scene, tasklist, self.id + 2, queryId)[0] do
        context:callNodes(scene, tasklist, self.id + 3);
        coroutine.yield(1);
        queryId = math.random(1, 100000);
    end

    context:callNodes(scene, tasklist, self.id + 4);
end

NodeEditor.registerNode("core.categories.main", WhileDoNode);