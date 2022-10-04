local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteTrainEvent
---@field static table
---@field trainUuid ffi.cdata*

---@type BaseNode | DeleteTrainEvent
local DeleteTrainEvent = class("DeleteTrainEvent", ed.BaseNode)
DeleteTrainEvent.static.name_ = "core.nodes.delete_train_event";
DeleteTrainEvent.static.type = ed.NodeType.Event;

---@param id integer
function DeleteTrainEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.trainUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteTrainEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local trains = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getTrains();
	local indexTrain = indexByUuid(trains, self.trainUuid);
    IncorrectHighlight(indexTrain == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.train"), self.trainUuid, indexTrain, #trains, function(i)
            return trains[i+1]:getName();
        end, function(i)
            return trains[i+1]:getUuid();
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
function DeleteTrainEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onTrainDelete[#global_data.signals.onTrainDelete + 1] = function(scene, tasklist, train_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == DeleteTrainEvent then
            if (UuidEquals(node.trainUuid, train_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.train", DeleteTrainEvent);