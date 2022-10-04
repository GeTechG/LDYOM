local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetTrain
---@field static table
---@field trainUuid ffi.cdata*

---@type BaseNode | GetTrain
local GetTrain = class("GetTrain", ed.BaseNode)
GetTrain.static.name_ = "core.nodes.get_train";
GetTrain.static.type = ed.NodeType.Function;

---@param id integer
function GetTrain:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.trainUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetTrain:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local trains = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getTrains();
	local indexTrain = indexByUuid(trains, self.trainUuid);
    IncorrectHighlight(indexTrain == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.train"), self.trainUuid, indexTrain, #trains, function(i)
            return trains[i+1]:getName();
        end, function(i)
            return trains[i+1]:getUuid();
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
function GetTrain:execute(scene, tasklist, context)
    local trains = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getTrains();
	local indexTrain = indexByUuid(trains, self.trainUuid);
    assert(indexTrain ~= -1, "Train not selected");
    ---@type Train
    local _train = trains[indexTrain+1];
    local train = _train:getProjectTrain();
    assert(train ~= nil, "Train is don't created");
    self.pins[2].value[0] = CVehicleToHandle(train);
end

NodeEditor.registerNode("core.categories.train", GetTrain);