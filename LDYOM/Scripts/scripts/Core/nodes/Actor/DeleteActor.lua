local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteActor
---@field static table

---@type BaseNode | DeleteActor
local DeleteActor = class("DeleteActor", ed.BaseNode)
DeleteActor.static.name_ = "core.nodes.delete_actor";
DeleteActor.static.type = ed.NodeType.Function;

---@param id integer
function DeleteActor:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.actorUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteActor:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
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
function DeleteActor:execute(scene, tasklist, context)
    local actors = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getActors();
	local indexActor = indexByUuid(actors, self.actorUuid);
    assert(indexActor ~= -1, "Actor not selected");
    ---@type Actor
    local actor = actors[indexActor+1];
    actor:deleteProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.actor", DeleteActor);