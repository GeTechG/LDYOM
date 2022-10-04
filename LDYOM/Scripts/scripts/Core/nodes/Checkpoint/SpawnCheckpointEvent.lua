local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnCheckpointEvent
---@field static table
---@field checkpointUuid ffi.cdata*

---@type BaseNode | SpawnCheckpointEvent
local SpawnCheckpointEvent = class("SpawnCheckpointEvent", ed.BaseNode)
SpawnCheckpointEvent.static.name_ = "core.nodes.spawn_checkpoint_event";
SpawnCheckpointEvent.static.type = ed.NodeType.Event;

---@param id integer
function SpawnCheckpointEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.checkpointUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnCheckpointEvent:draw()
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
function SpawnCheckpointEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onCheckpointSpawn[#global_data.signals.onCheckpointSpawn + 1] = function(scene, tasklist, checkpoint_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == SpawnCheckpointEvent then
            if (UuidEquals(node.checkpointUuid, checkpoint_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.checkpoint", SpawnCheckpointEvent);