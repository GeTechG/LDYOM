local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetObject
---@field static table
---@field objectUuid ffi.cdata*

---@type BaseNode | GetObject
local GetObject = class("GetObject", ed.BaseNode)
GetObject.static.name_ = "core.nodes.get_object_char";
GetObject.static.type = ed.NodeType.Function;

---@param id integer
function GetObject:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.objectUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetObject:draw()
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
function GetObject:execute(scene, tasklist, context)
    local objects = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getObjects();
	local indexObject = indexByUuid(objects, self.objectUuid);
    assert(indexObject ~= -1, "Object not selected");
    ---@type Object
    local _object = objects[indexObject+1];
    local object = _object:getProjectObject();
    assert(object ~= nil, "Object is don't created");
    self.pins[2].value[0] = CObjectToHandle(object);
end

NodeEditor.registerNode("core.categories.object", GetObject);