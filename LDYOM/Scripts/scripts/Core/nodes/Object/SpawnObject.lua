local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnObject
---@field static table

---@type BaseNode | SpawnObject
local SpawnObject = class("SpawnObject", ed.BaseNode)
SpawnObject.static.name_ = "core.nodes.spawn_object";
SpawnObject.static.type = ed.NodeType.Function;

---@param id integer
function SpawnObject:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.objectUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnObject:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
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
function SpawnObject:execute(scene, tasklist, context)
    local objects = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getObjects();
	local indexObject = indexByUuid(objects, self.objectUuid);
    assert(indexObject ~= -1, "Object not selected");
    ---@type Object
    local object = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getObjects()[indexObject+1];
    object:spawnProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.object", SpawnObject);