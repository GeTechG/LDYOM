local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetAudioStream
---@field static table
---@field audioUuid ffi.cdata*

---@type BaseNode | GetAudioStream
local GetAudioStream = class("GetAudioStream", ed.BaseNode)
GetAudioStream.static.name_ = "core.nodes.get_audio_stream";
GetAudioStream.static.type = ed.NodeType.Function;

---@param id integer
function GetAudioStream:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.audioUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "Char", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetAudioStream:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local audios = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getAudio();
	local indexAudio = indexByUuid(audios, self.audioUuid);
    IncorrectHighlight(indexAudio == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.audio"), self.audioUuid, indexAudio, #audios, function(i)
            return audios[i+1]:getName();
        end, function(i)
            return audios[i+1]:getUuid();
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
function GetAudioStream:execute(scene, tasklist, context)
    local audios = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getAudio();
	local indexAudio = indexByUuid(audios, self.audioUuid);
    assert(indexAudio ~= -1, "Audio not selected");
    ---@type Audio
    local audio = audios[indexAudio+1];
    local stream = audio:getProjectAudio();
    assert(stream ~= nil, "Audio is don't created");
    self.pins[2].value[0] = stream;
end

NodeEditor.registerNode("core.categories.audio", GetAudioStream);