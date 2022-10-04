local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteVehicle
---@field static table

---@type BaseNode | DeleteVehicle
local DeleteVehicle = class("DeleteVehicle", ed.BaseNode)
DeleteVehicle.static.name_ = "core.nodes.delete_vehicle";
DeleteVehicle.static.type = ed.NodeType.Function;

---@param id integer
function DeleteVehicle:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.vehicleUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteVehicle:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
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
function DeleteVehicle:execute(scene, tasklist, context)
    local vehicles = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getVehicles();
	local indexVehicle = indexByUuid(vehicles, self.vehicleUuid);
    assert(indexVehicle ~= -1, "Vehicle not selected");
    ---@type Vehicle
    local vehicle = vehicles[indexVehicle+1];
    vehicle:deleteProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.vehicle", DeleteVehicle);