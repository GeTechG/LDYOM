local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetVehicle
---@field static table
---@field vehicleUuid ffi.cdata*

---@type BaseNode | GetVehicle
local GetVehicle = class("GetVehicle", ed.BaseNode)
GetVehicle.static.name_ = "core.nodes.get_vehicle";
GetVehicle.static.type = ed.NodeType.Function;

---@param id integer
function GetVehicle:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.vehicleUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetVehicle:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local vehicles = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getVehicles();
	local indexVehicle = indexByUuid(vehicles, self.vehicleUuid);
    IncorrectHighlight(indexVehicle == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.vehicle"), self.vehicleUuid, indexVehicle, #vehicles, function(i)
            return vehicles[i+1]:getName();
        end, function(i)
            return vehicles[i+1]:getUuid();
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
function GetVehicle:execute(scene, tasklist, context)
    local vehicles = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getVehicles();
	local indexVehicle = indexByUuid(vehicles, self.vehicleUuid);
    assert(indexVehicle ~= -1, "Vehicle not selected");
    ---@type Vehicle
    local _vehicle = vehicles[indexVehicle+1];
    local vehicle = _vehicle:getProjectVehicle();
    assert(vehicle ~= nil, "Vehicle is don't created");
    self.pins[2].value[0] = CVehicleToHandle(vehicle);
end

NodeEditor.registerNode("core.categories.vehicle", GetVehicle);