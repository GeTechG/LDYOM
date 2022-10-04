local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnTrain
---@field static table

---@type BaseNode | SpawnTrain
local SpawnTrain = class("SpawnTrain", ed.BaseNode)
SpawnTrain.static.name_ = "core.nodes.spawn_train";
SpawnTrain.static.type = ed.NodeType.Function;

---@param id integer
function SpawnTrain:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.trainUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnTrain:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
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
function SpawnTrain:execute(scene, tasklist, context)
    local trains = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getTrains();
	local indexTrain = indexByUuid(trains, self.trainUuid);
    assert(indexTrain ~= -1, "Train not selected");
    ---@type Train
    local train = trains[indexTrain+1];
    train:spawnProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.train", SpawnTrain);