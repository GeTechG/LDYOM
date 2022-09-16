local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class NumberToString
---@field static table

---@type BaseNode | NumberToString
local NumberToString = class("NumberToString", ed.BaseNode)
NumberToString.static.name_ = "core.nodes.number_to_string";
NumberToString.static.type = ed.NodeType.Function;

---@param id integer
function NumberToString:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Number),
        ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.String, "", constants.TEXT_SIZE)
    }
end

function NumberToString:draw()
    ImNodes.beginNodeNoTitle(self.id);

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[2]);
    ImGui.text(self.pins[2].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNodeNoTitle();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function NumberToString:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    ffi.copy(self.pins[2].value, tostring(context:getValue(scene, tasklist, self.id + 1, queryId)[0]))
end

NodeEditor.registerNode("core.categories.convertion", NumberToString);