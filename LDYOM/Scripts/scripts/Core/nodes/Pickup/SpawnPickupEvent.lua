local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnPickupEvent
---@field static table
---@field pickupUuid ffi.cdata*

---@type BaseNode | SpawnPickupEvent
local SpawnPickupEvent = class("SpawnPickupEvent", ed.BaseNode)
SpawnPickupEvent.static.name_ = "core.nodes.spawn_pickup_event";
SpawnPickupEvent.static.type = ed.NodeType.Event;

---@param id integer
function SpawnPickupEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pickupUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnPickupEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local pickups = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getPickups();
	local indexPickup = indexByUuid(pickups, self.pickupUuid);
    IncorrectHighlight(indexPickup == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.pickup"), self.pickupUuid, indexPickup, #pickups, function(i)
            return pickups[i+1]:getName();
        end, function(i)
            return pickups[i+1]:getUuid();
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
function SpawnPickupEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onPickupSpawn[#global_data.signals.onPickupSpawn + 1] = function(scene, tasklist, pickup_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == SpawnPickupEvent then
            if (UuidEquals(node.pickupUuid, pickup_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.pickup", SpawnPickupEvent);