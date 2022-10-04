local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeleteParticle
---@field static table

---@type BaseNode | DeleteParticle
local DeleteParticle = class("DeleteParticle", ed.BaseNode)
DeleteParticle.static.name_ = "core.nodes.delete_particle";
DeleteParticle.static.type = ed.NodeType.Function;

---@param id integer
function DeleteParticle:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.particleUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeleteParticle:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
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
function DeleteParticle:execute(scene, tasklist, context)
    local particles = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getParticles();
	local indexParticle = indexByUuid(particles, self.particleUuid);
    assert(indexParticle ~= -1, "Particle not selected");
    ---@type Particle
    local particle = particles[indexParticle+1];
    particle:deleteProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.particle", DeleteParticle);