local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteActorEvent
---@field static table
---@field actorUuid ffi.cdata*

---@type BaseNode | DeleteActorEvent
local DeleteActorEvent = class("DeleteActorEvent", ed.BaseNode)
DeleteActorEvent.static.name_ = "core.nodes.delete_actor_event";
DeleteActorEvent.static.type = ed.NodeType.Event;

---@param id integer
function DeleteActorEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.actorUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteActorEvent:draw()
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
function DeleteActorEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onActorDelete[#global_data.signals.onActorDelete + 1] = function(scene, tasklist, actor_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == DeleteActorEvent then
            if (UuidEquals(node.actorUuid, actor_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.actor", DeleteActorEvent);