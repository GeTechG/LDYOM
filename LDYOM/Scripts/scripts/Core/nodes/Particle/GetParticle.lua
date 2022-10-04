local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetParticle
---@field static table
---@field particleUuid ffi.cdata*

---@type BaseNode | GetParticle
local GetParticle = class("GetParticle", ed.BaseNode)
GetParticle.static.name_ = "core.nodes.get_particle";
GetParticle.static.type = ed.NodeType.Function;

---@param id integer
function GetParticle:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.particleUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetParticle:draw()
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
function GetParticle:execute(scene, tasklist, context)
    local particles = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getParticles();
	local indexParticle = indexByUuid(particles, self.particleUuid);
    assert(indexParticle ~= -1, "Particle not selected");
    ---@type Particle
    local _particle = particles[indexParticle+1];
    local particle = _particle:getProjectParticleId();
    assert(particle ~= nil, "Particle is don't created");
    self.pins[2].value[0] = particle;
end

NodeEditor.registerNode("core.categories.particle", GetParticle);