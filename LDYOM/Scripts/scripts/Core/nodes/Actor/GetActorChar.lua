local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetActorChar
---@field static table
---@field actorUuid ffi.cdata*

---@type BaseNode | GetActorChar
local GetActorChar = class("GetActorChar", ed.BaseNode)
GetActorChar.static.name_ = "core.nodes.get_actor_char";
GetActorChar.static.type = ed.NodeType.Function;

---@param id integer
function GetActorChar:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.actorUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "Char", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetActorChar:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local actors = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getActors();
	local indexActor = indexByUuid(actors, self.actorUuid);
    IncorrectHighlight(indexActor == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.actor"), self.actorUuid, indexActor, #actors, function(i)
            return actors[i+1]:getName();
        end, function(i)
            return actors[i+1]:getUuid();
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
function GetActorChar:execute(scene, tasklist, context)
    local actors = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getActors();
	local indexActor = indexByUuid(actors, self.actorUuid);
    assert(indexActor ~= -1, "Actor not selected");
    ---@type Actor
    local actor = actors[indexActor+1];
    local ped = actor:getProjectPed();
    assert(ped ~= nil, "Actor is don't created");
    self.pins[2].value[0] = CPedToHandle(ped);
end

NodeEditor.registerNode("core.categories.actor", GetActorChar);