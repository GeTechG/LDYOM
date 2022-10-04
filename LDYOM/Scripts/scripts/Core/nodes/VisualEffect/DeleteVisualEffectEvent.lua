local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteVisualEffectEvent
---@field static table
---@field visualEffectUuid ffi.cdata*

---@type BaseNode | DeleteVisualEffectEvent
local DeleteVisualEffectEvent = class("DeleteVisualEffectEvent", ed.BaseNode)
DeleteVisualEffectEvent.static.name_ = "core.nodes.delete_visualEffect_event";
DeleteVisualEffectEvent.static.type = ed.NodeType.Event;

---@param id integer
function DeleteVisualEffectEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.visualEffectUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteVisualEffectEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
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

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[2]);
    ImGui.text(self.pins[2].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function DeleteVisualEffectEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onVisualEffectDelete[#global_data.signals.onVisualEffectDelete + 1] = function(scene, tasklist, visualEffect_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == DeleteVisualEffectEvent then
            if (UuidEquals(node.visualEffectUuid, visualEffect_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.visualEffect", DeleteVisualEffectEvent);