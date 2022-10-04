local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnPickup
---@field static table

---@type BaseNode | SpawnPickup
local SpawnPickup = class("SpawnPickup", ed.BaseNode)
SpawnPickup.static.name_ = "core.nodes.spawn_pickup";
SpawnPickup.static.type = ed.NodeType.Function;

---@param id integer
function SpawnPickup:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pickupUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnPickup:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
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
function SpawnPickup:execute(scene, tasklist, context)
    local pickups = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getPickups();
	local indexPickup = indexByUuid(pickups, self.pickupUuid);
    assert(indexPickup ~= -1, "Pickup not selected");
    ---@type Pickup
    local pickup = pickups[indexPickup+1];
    pickup:spawnProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.pickup", SpawnPickup);