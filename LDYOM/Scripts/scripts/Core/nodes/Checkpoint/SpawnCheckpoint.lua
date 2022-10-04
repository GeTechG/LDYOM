local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnCheckpoint
---@field static table

---@type BaseNode | SpawnCheckpoint
local SpawnCheckpoint = class("SpawnCheckpoint", ed.BaseNode)
SpawnCheckpoint.static.name_ = "core.nodes.spawn_checkpoint";
SpawnCheckpoint.static.type = ed.NodeType.Function;

---@param id integer
function SpawnCheckpoint:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.checkpointUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnCheckpoint:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
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
function SpawnCheckpoint:execute(scene, tasklist, context)
    local checkpoints = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getCheckpoints();
	local indexCheckpoint = indexByUuid(checkpoints, self.checkpointUuid);
    assert(indexCheckpoint ~= -1, "Checkpoint not selected");
    ---@type Checkpoint
    local checkpoint = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getCheckpoints()[indexCheckpoint+1];
    checkpoint:spawnProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.checkpoint", SpawnCheckpoint);