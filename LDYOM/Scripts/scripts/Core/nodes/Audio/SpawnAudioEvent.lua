local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnAudioEvent
---@field static table
---@field audioUuid ffi.cdata*

---@type BaseNode | SpawnAudioEvent
local SpawnAudioEvent = class("SpawnAudioEvent", ed.BaseNode)
SpawnAudioEvent.static.name_ = "core.nodes.spawn_audio_event";
SpawnAudioEvent.static.type = ed.NodeType.Event;

---@param id integer
function SpawnAudioEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.audioUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnAudioEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local audios = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getAudios();
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
function SpawnAudioEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onAudioSpawn[#global_data.signals.onAudioSpawn + 1] = function(scene, tasklist, audio_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == SpawnAudioEvent then
            if (UuidEquals(node.audioUuid, audio_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.audio", SpawnAudioEvent);