local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteVisualEffect
---@field static table

---@type BaseNode | DeleteVisualEffect
local DeleteVisualEffect = class("DeleteVisualEffect", ed.BaseNode)
DeleteVisualEffect.static.name_ = "core.nodes.delete_visualEffect";
DeleteVisualEffect.static.type = ed.NodeType.Function;

---@param id integer
function DeleteVisualEffect:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.visualEffectUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteVisualEffect:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
    local visualEffects = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getVisualEffects();
	local indexVisualEffect = indexByUuid(visualEffects, self.visualEffectUuid);
    IncorrectHighlight(indexVisualEffect == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.visualEffect"), self.visualEffectUuid, indexVisualEffect, #visualEffects, function(i)
            return visualEffects[i+1]:getName();
        end, function(i)
            return visualEffects[i+1]:getUuid();
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
function DeleteVisualEffect:execute(scene, tasklist, context)
    local visualEffects = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getVisualEffects();
	local indexVisualEffect = indexByUuid(visualEffects, self.visualEffectUuid);
    assert(indexVisualEffect ~= -1, "VisualEffect not selected");
    ---@type VisualEffect
    local visualEffect = visualEffects[indexVisualEffect+1];
    visualEffect:deleteProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.visualEffect", DeleteVisualEffect);