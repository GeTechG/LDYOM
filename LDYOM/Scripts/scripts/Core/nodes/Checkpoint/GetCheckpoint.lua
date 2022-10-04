local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetCheckpointChar
---@field static table
---@field checkpointUuid ffi.cdata*

---@type BaseNode | GetCheckpointChar
local GetCheckpointChar = class("GetCheckpointChar", ed.BaseNode)
GetCheckpointChar.static.name_ = "core.nodes.get_checkpoint_char";
GetCheckpointChar.static.type = ed.NodeType.Function;

---@param id integer
function GetCheckpointChar:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.checkpointUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "Char", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetCheckpointChar:draw()
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
function GetCheckpointChar:execute(scene, tasklist, context)
    local checkpoints = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getCheckpoints();
	local indexCheckpoint = indexByUuid(checkpoints, self.checkpointUuid);
    assert(indexCheckpoint ~= -1, "Checkpoint not selected");
    ---@type Checkpoint
    local _checkpoint = checkpoints[indexCheckpoint+1];
    --undefined get
    local checkpoint = _checkpoint:getProjectSphere();
    assert(checkpoint ~= nil, "Checkpoint is don't created");
    self.pins[2].value[0] = checkpoint;
end

--fix me
--NodeEditor.registerNode("core.categories.checkpoint", GetCheckpointChar);