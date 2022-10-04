local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetPickup
---@field static table
---@field pickupUuid ffi.cdata*

---@type BaseNode | GetPickup
local GetPickup = class("GetPickup", ed.BaseNode)
GetPickup.static.name_ = "core.nodes.get_pickup";
GetPickup.static.type = ed.NodeType.Function;

---@param id integer
function GetPickup:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pickupUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetPickup:draw()
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
function GetPickup:execute(scene, tasklist, context)
    local pickups = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getPickups();
	local indexPickup = indexByUuid(pickups, self.pickupUuid);
    assert(indexPickup ~= -1, "Pickup not selected");
    ---@type Pickup
    local _pickup = pickups[indexPickup+1];
    local pickup = _pickup:getProjectPickupIndex();
    assert(pickup ~= nil, "Pickup is don't created");
    self.pins[2].value[0] = pickup;
end

NodeEditor.registerNode("core.categories.pickup", GetPickup);