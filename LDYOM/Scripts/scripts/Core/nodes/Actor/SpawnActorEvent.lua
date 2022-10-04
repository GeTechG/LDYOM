local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnActorEvent
---@field static table
---@field actorUuid ffi.cdata*

---@type BaseNode | SpawnActorEvent
local SpawnActorEvent = class("SpawnActorEvent", ed.BaseNode)
SpawnActorEvent.static.name_ = "core.nodes.spawn_actor_event";
SpawnActorEvent.static.type = ed.NodeType.Event;

---@param id integer
function SpawnActorEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.actorUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnActorEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local actors = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getActors();
	local indexActor = indexByUuid(actors, self.actorUuid);
    IncorrectHighlight(indexActor == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.actor"), self.actorUuid, indexActor, #actors, function(i)
            return actors[i+1]:getName();
        end, function(i)
            return actors[i+1]:getUuid();
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
function SpawnActorEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onActorSpawn[#global_data.signals.onActorSpawn + 1] = function(scene, tasklist, actor_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == SpawnActorEvent then
            if (UuidEquals(node.actorUuid, actor_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.actor", SpawnActorEvent);