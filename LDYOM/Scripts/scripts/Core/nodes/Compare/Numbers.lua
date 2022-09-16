local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"

---@class CompareNumbers
---@field static table
---@field conditionType ffi.cdata*

---@type BaseNode | CompareNumbers
local CompareNumbers = class("CompareNumbers", ed.BaseNode)
CompareNumbers.static.name_ = "core.nodes.compare_numbers";
CompareNumbers.static.type = ed.NodeType.Condition;

---@param id integer
function CompareNumbers:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.conditionType = ffi.new("float[1]", {0});
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Number),
        ed.Pin:new(id + 2, "", ed.PinType.In, ed.ValueType.Number),
        ed.Pin:new(id + 3, "general.result", ed.PinType.Out, ed.ValueType.Boolean)
    }
end

function CompareNumbers:draw()
    ImNodes.beginNodeNoTitle(self.id);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    if not self.pins[1].connected then
        ImGui.setNextItemWidth(120);
        ImGui.inputFloat(self.pins[1].name, self.pins[1].value);
    else
        ImGui.text(loc.get(self.pins[1].name));
    end
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 4);
    ImGui.setNextItemWidth(120);
    ImGui.combo("", self.conditionType, loc.getArray("general.comparings"));
    ImNodes.endStaticAttribute();

    ImNodes.beginInputAttribute(self.pins[2]);
    if not self.pins[2].connected then
        ImGui.setNextItemWidth(120);
        ImGui.inputFloat(self.pins[2].name, self.pins[2].value);
    else
        ImGui.text(loc.get(self.pins[2].name));
    end
    ImNodes.endInputAttribute();

    ImGui.endGroup();

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[3]);
    ImGui.text(loc.get(self.pins[3].name));
    ImNodes.endOutputAttribute();

    ImNodes.endNodeNoTitle();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function CompareNumbers:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    local v1 = context:getValue(scene, tasklist, self.id + 1, queryId)[0];
    local v2 = context:getValue(scene, tasklist, self.id + 2, queryId)[0];
    self.pins[3].value[0] = self.conditionType[0] == 0 and v1 == v2 or
                            self.conditionType[0] == 1 and v1 ~= v2 or
                            self.conditionType[0] == 2 and v1 > v2 or
                            self.conditionType[0] == 3 and v1 >= v2 or
                            self.conditionType[0] == 4 and v1 < v2 or
                            self.conditionType[0] == 5 and v1 <= v2
end

NodeEditor.registerNode("core.categories.compare", CompareNumbers);