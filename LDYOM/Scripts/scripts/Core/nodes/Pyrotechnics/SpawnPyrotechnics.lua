local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class SpawnPyrotechnics
---@field static table

---@type BaseNode | SpawnPyrotechnics
local SpawnPyrotechnics = class("SpawnPyrotechnics", ed.BaseNode)
SpawnPyrotechnics.static.name_ = "core.nodes.spawn_pyrotechnics";
SpawnPyrotechnics.static.type = ed.NodeType.Function;

---@param id integer
function SpawnPyrotechnics:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = true;
    self.pyrotechnicsUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [1] = ed.Pin:new(id + 1, "", ed.PinType.In, ed.ValueType.Void),
        [3] = ed.Pin:new(id + 3, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function SpawnPyrotechnics:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImGui.beginGroup();

    ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();

    ImNodes.beginStaticAttribute(self.id + 2);
    local pyrotechnicss = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getPyrotechnicss();
	local indexPyrotechnics = indexByUuid(pyrotechnicss, self.pyrotechnicsUuid);
    IncorrectHighlight(indexPyrotechnics == -1, function()
        ImGui.setNextItemWidth(120);
        UuidCombo(loc.get("entities.pyrotechnics"), self.pyrotechnicsUuid, indexPyrotechnics, #pyrotechnicss, function(i)
            return pyrotechnicss[i+1]:getName();
        end, function(i)
            return pyrotechnicss[i+1]:getUuid();
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
function SpawnPyrotechnics:execute(scene, tasklist, context)
    local pyrotechnicss = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getPyrotechnics();
	local indexPyrotechnics = indexByUuid(pyrotechnicss, self.pyrotechnicsUuid);
    assert(indexPyrotechnics ~= -1, "Pyrotechnics not selected");
    ---@type Pyrotechnics
    local pyrotechnics = pyrotechnicss[indexPyrotechnics+1];
    pyrotechnics:spawnProjectEntity();
    
    context:callNodes(scene, tasklist, self.id + 3);
end

NodeEditor.registerNode("core.categories.pyrotechnics", SpawnPyrotechnics);