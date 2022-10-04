local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetVisualEffect
---@field static table
---@field visualEffectUuid ffi.cdata*

---@type BaseNode | GetVisualEffect
local GetVisualEffect = class("GetVisualEffect", ed.BaseNode)
GetVisualEffect.static.name_ = "core.nodes.get_visualEffect";
GetVisualEffect.static.type = ed.NodeType.Function;

---@param id integer
function GetVisualEffect:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.visualEffectUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetVisualEffect:draw()
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
function GetVisualEffect:execute(scene, tasklist, context)
    local visualEffects = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getVisualEffects();
	local indexVisualEffect = indexByUuid(visualEffects, self.visualEffectUuid);
    assert(indexVisualEffect ~= -1, "VisualEffect not selected");
    ---@type VisualEffect
    local _visualEffect = visualEffects[indexVisualEffect+1];
    --local visualEffect = _visualEffect:get();
    assert(visualEffect ~= nil, "VisualEffect is don't created");
    self.pins[2].value[0] = CVisualEffectToHandle(visualEffect);
end

--NodeEditor.registerNode("core.categories.visualEffect", GetVisualEffect);