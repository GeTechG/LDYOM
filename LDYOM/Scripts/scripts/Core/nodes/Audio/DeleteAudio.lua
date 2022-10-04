local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteAudio
---@field static table

---@type BaseNode | DeleteAudio
local DeleteAudio = class("DeleteAudio", ed.BaseNode)
DeleteAudio.static.name_ = "core.nodes.delete_audio";
DeleteAudio.static.type = ed.NodeType.Function;

---@param id integer
function DeleteAudio:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.audioUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteAudio:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
    local audio = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getAudio();
	local indexAudio = indexByUuid(audio, self.audioUuid);
    IncorrectHighlight(indexAudio == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.audio"), self.audioUuid, indexAudio, #audios, function(i)
            return audios[i+1]:getName();
        end, function(i)
            return audios[i+1]:getUuid();
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
function DeleteAudio:execute(scene, tasklist, context)
    local audios = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getAudio();
	local indexAudio = indexByUuid(audios, self.audioUuid);
    assert(indexAudio ~= -1, "Audio not selected");
    ---@type Audio
    local audio = audios[indexAudio+1];
    audio:deleteProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.audio", DeleteAudio);