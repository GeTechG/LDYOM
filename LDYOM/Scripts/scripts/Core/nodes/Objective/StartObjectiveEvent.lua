local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class StartObjectiveEvent
---@field static table
---@field objectiveUuid ffi.cdata*

---@type BaseNode | StartObjectiveEvent
local StartObjectiveEvent = class("StartObjectiveEvent", ed.BaseNode)
StartObjectiveEvent.static.name_ = "core.nodes.start_objective_event";
StartObjectiveEvent.static.type = ed.NodeType.Event;

---@param id integer
function StartObjectiveEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.objectiveUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function StartObjectiveEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
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

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[2]);
    ImGui.text(self.pins[2].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function StartObjectiveEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onObjectiveStart[#global_data.signals.onObjectiveStart + 1] = function(scene, tasklist, objective_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == StartObjectiveEvent then
            if (UuidEquals(node.objectiveUuid, objective_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.objective", StartObjectiveEvent);