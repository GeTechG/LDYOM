local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnObjectEvent
---@field static table
---@field objectUuid ffi.cdata*

---@type BaseNode | SpawnObjectEvent
local SpawnObjectEvent = class("SpawnObjectEvent", ed.BaseNode)
SpawnObjectEvent.static.name_ = "core.nodes.spawn_object_event";
SpawnObjectEvent.static.type = ed.NodeType.Event;

---@param id integer
function SpawnObjectEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.objectUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnObjectEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local objects = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getObjects();
	local indexObject = indexByUuid(objects, self.objectUuid);
    IncorrectHighlight(indexObject == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.object"), self.objectUuid, indexObject, #objects, function(i)
            return objects[i+1]:getName();
        end, function(i)
            return objects[i+1]:getUuid();
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
function SpawnObjectEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onObjectSpawn[#global_data.signals.onObjectSpawn + 1] = function(scene, tasklist, object_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == SpawnObjectEvent then
            if (UuidEquals(node.objectUuid, object_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.object", SpawnObjectEvent);