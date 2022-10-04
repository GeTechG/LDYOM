local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteParticleEvent
---@field static table
---@field particleUuid ffi.cdata*

---@type BaseNode | DeleteParticleEvent
local DeleteParticleEvent = class("DeleteParticleEvent", ed.BaseNode)
DeleteParticleEvent.static.name_ = "core.nodes.delete_particle_event";
DeleteParticleEvent.static.type = ed.NodeType.Event;

---@param id integer
function DeleteParticleEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.particleUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteParticleEvent:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local particles = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getParticles();
	local indexParticle = indexByUuid(particles, self.particleUuid);
    IncorrectHighlight(indexParticle == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.particle"), self.particleUuid, indexParticle, #particles, function(i)
            return particles[i+1]:getName();
        end, function(i)
            return particles[i+1]:getUuid();
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
function DeleteParticleEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onParticleDelete[#global_data.signals.onParticleDelete + 1] = function(scene, tasklist, particle_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == DeleteParticleEvent then
            if (UuidEquals(node.particleUuid, particle_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.particle", DeleteParticleEvent);