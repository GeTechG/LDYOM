local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SetNextObjective
---@field static table

---@type BaseNode | SetNextObjective
local SetNextObjective = class("SetNextObjective", ed.BaseNode)
SetNextObjective.static.name_ = "core.nodes.set_next_objective";
SetNextObjective.static.type = ed.NodeType.Function;

---@param id integer
function SetNextObjective:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.objectiveUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SetNextObjective:draw()
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

    ImNodes.beginOutputAttribute(self.pins[3]);
    ImGui.text(self.pins[3].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function SetNextObjective:execute(scene, tasklist, context)
    local objectives = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getObjectives();
	local indexObjective = indexByUuid(objectives, self.objectiveUuid);
    assert(indexObjective ~= -1, "Objective not selected");
    setNextObjective(indexObjective);
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.objective", SetNextObjective);