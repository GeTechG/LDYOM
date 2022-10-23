local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"

---@class SaveNode
---@field static table
---@field showConfirm bool

---@type BaseNode | SaveNode
local SaveNode = class("SaveNode", ed.BaseNode)
SaveNode.static.name_ = "core.nodes.save";
SaveNode.static.type = ed.NodeType.Function;

---@param id integer
function SaveNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.showConfirm = ffi.new("bool[1]", false);
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void),
        [4] = ed.Pin:new(id + 4, "core.pins.load", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SaveNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(2);
    ImGui.checkbox(loc.get("save_objective.show_confirm"), self.showConfirm)
    ImNodes.endStaticAttribute();

    ImGui.endGroup();

    ImGui.sameLine();

    local inp_size = ImGui.getItemRectSize();

    ImGui.beginGroup();

    ImNodes.beginOutputAttribute(self.pins[3]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[3].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(self.pins[3].name);
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
function SaveNode:execute(scene, tasklist, context)
    local _end = false;
    saveGame(self.showConfirm[0], self.pins[4].id, scene, tasklist, function()
        _end = true;
    end)

    while not _end do
        coroutine.yield(1);
    end

    context:callNodes(scene, tasklist, self.id + 3);
end

function callNode(scene, tasklist, nodeId)
    local context = global_data.ed_contexts[scene:getId()];
    local node = context.nodes[nodeId];
    Tasker.addTask(tasklist, function()
        node:execute(scene, tasklist, context);
    end)
end

NodeEditor.registerNode("core.categories.main", SaveNode);