local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteCheckpointEvent
---@field static table
---@field checkpointUuid ffi.cdata*

---@type BaseNode | DeleteCheckpointEvent
local DeleteCheckpointEvent = class("DeleteCheckpointEvent", ed.BaseNode)
DeleteCheckpointEvent.static.name_ = "core.nodes.delete_checkpoint_event";
DeleteCheckpointEvent.static.type = ed.NodeType.Event;

---@param id integer
function DeleteCheckpointEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.checkpointUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteCheckpointEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local checkpoints = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getCheckpoints();
	local indexCheckpoint = indexByUuid(checkpoints, self.checkpointUuid);
    IncorrectHighlight(indexCheckpoint == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.checkpoint"), self.checkpointUuid, indexCheckpoint, #checkpoints, function(i)
            return checkpoints[i+1]:getName();
        end, function(i)
            return checkpoints[i+1]:getUuid();
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
function DeleteCheckpointEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onCheckpointDelete[#global_data.signals.onCheckpointDelete + 1] = function(scene, tasklist, checkpoint_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == DeleteCheckpointEvent then
            if (UuidEquals(node.checkpointUuid, checkpoint_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.checkpoint", DeleteCheckpointEvent);