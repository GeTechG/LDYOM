local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class CheckCurrentObjectiveNode
---@field static table
---@field objectiveUuid ffi.cdata*

---@type BaseNode | CheckCurrentObjectiveNode
local CheckCurrentObjectiveNode = class("CheckCurrentObjectiveNode", ed.BaseNode)
CheckCurrentObjectiveNode.static.name_ = "core.nodes.check_current_objective";
CheckCurrentObjectiveNode.static.type = ed.NodeType.Condition;

---@param id integer
function CheckCurrentObjectiveNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.objectiveUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "general.true", ed.PinType.Out, ed.ValueType.Void),
        [4] = ed.Pin:new(id + 4, "general.false", ed.PinType.Out, ed.ValueType.Void),
    }
end

function CheckCurrentObjectiveNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
    local objectives = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getObjectives();
	local indexObjective = indexByUuid(objectives, self.objectiveUuid);
    IncorrectHighlight(indexObjective == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("objective.title"), self.objectiveUuid, indexObjective, #objectives, function(i)
            return objectives[i+1]:getName();
        end, function(i)
            return objectives[i+1]:getUuid();
        end)
    end);
    ImNodes.endStaticAttribute();

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
function CheckCurrentObjectiveNode:execute(scene, tasklist, context)
    ---@type BaseObjective
    local currentObjective = getCurrentObjective();
    if (currentObjective ~= nil) then
        if (UuidEquals(self.objectiveUuid, currentObjective:getUuid())) then
            context:callNodes(scene, tasklist, self.id + 3);
        else
            context:callNodes(scene, tasklist, self.id + 4);
        end
    end
end

NodeEditor.registerNode("core.categories.objective", CheckCurrentObjectiveNode);