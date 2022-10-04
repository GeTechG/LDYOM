local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class GetFire
---@field static table
---@field pyrotechnicsUuid ffi.cdata*

---@type BaseNode | GetFire
local GetFire = class("GetFire", ed.BaseNode)
GetFire.static.name_ = "core.nodes.get_pyrotechnics";
GetFire.static.type = ed.NodeType.Function;

---@param id integer
function GetFire:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pyrotechnicsUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Number)
    }
end

function GetFire:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginStaticAttribute(self.id + 1);
    local pyrotechnicss = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getPyrotechnics();
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

    ImGui.sameLine();

    ImNodes.beginOutputAttribute(self.pins[2]);
    ImGui.text(self.pins[2].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function GetFire:execute(scene, tasklist, context)
    local pyrotechnicss = ProjectsService.getInstance():getCurrentProject():getCurrentScene():getPyrotechnics();
	local indexPyrotechnics = indexByUuid(pyrotechnicss, self.pyrotechnicsUuid);
    assert(indexPyrotechnics ~= -1, "Pyrotechnics not selected");
    ---@type Pyrotechnics
    local _pyrotechnics = pyrotechnicss[indexPyrotechnics+1];
    local pyrotechnics = _pyrotechnics:getProjectFireId();
    assert(pyrotechnics ~= nil, "Pyrotechnics is don't created");
    self.pins[2].value[0] = pyrotechnics;
end

NodeEditor.registerNode("core.categories.pyrotechnics", GetFire);