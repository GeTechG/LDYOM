local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class PrintLogNode
---@field static table

---@type BaseNode | PrintLogNode
local PrintLogNode = class("PrintLogNode", ed.BaseNode)
PrintLogNode.static.name_ = "core.nodes.print_log";
PrintLogNode.static.type = ed.NodeType.Function;

---@param id integer
function PrintLogNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        ed.Pin:new(id + 2, "general.text", ed.PinType.In, ed.ValueType.String),
        ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function PrintLogNode:draw()
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

    ImNodes.beginOutputAttribute(self.pins[3]);
    ImGui.text(self.pins[3].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function PrintLogNode:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    local text = ffi.string(context:getValue(scene, tasklist, self.id + 2, queryId));
    print(text);
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.main", PrintLogNode);