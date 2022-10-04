local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteVehicleEvent
---@field static table
---@field vehicleUuid ffi.cdata*

---@type BaseNode | DeleteVehicleEvent
local DeleteVehicleEvent = class("DeleteVehicleEvent", ed.BaseNode)
DeleteVehicleEvent.static.name_ = "core.nodes.delete_vehicle_event";
DeleteVehicleEvent.static.type = ed.NodeType.Event;

---@param id integer
function DeleteVehicleEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.vehicleUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteVehicleEvent:draw()
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
function DeleteVehicleEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onVehicleDelete[#global_data.signals.onVehicleDelete + 1] = function(scene, tasklist, vehicle_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == DeleteVehicleEvent then
            if (UuidEquals(node.vehicleUuid, vehicle_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.vehicle", DeleteVehicleEvent);