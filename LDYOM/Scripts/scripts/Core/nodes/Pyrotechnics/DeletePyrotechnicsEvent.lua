local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"

---@class DeletePyrotechnicsEvent
---@field static table
---@field pyrotechnicsUuid ffi.cdata*

---@type BaseNode | DeletePyrotechnicsEvent
local DeletePyrotechnicsEvent = class("DeletePyrotechnicsEvent", ed.BaseNode)
DeletePyrotechnicsEvent.static.name_ = "core.nodes.delete_pyrotechnics_event";
DeletePyrotechnicsEvent.static.type = ed.NodeType.Event;

---@param id integer
function DeletePyrotechnicsEvent:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pyrotechnicsUuid = ffi.new("unsigned char[16]", {0});
    self.pins = {
        [2] = ed.Pin:new(id + 2, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function DeletePyrotechnicsEvent:draw()
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
function DeletePyrotechnicsEvent:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 2);
end


---@param scene Scene
global_data.signals.onPyrotechnicsDelete[#global_data.signals.onPyrotechnicsDelete + 1] = function(scene, tasklist, pyrotechnics_uuid)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == DeletePyrotechnicsEvent then
            if (UuidEquals(node.pyrotechnicsUuid, pyrotechnics_uuid)) then
                Tasker.addTask(tasklist, function()
                    node:execute(scene, tasklist, context);
                end)
            end
        end
    end
end

NodeEditor.registerNode("core.categories.pyrotechnics", DeletePyrotechnicsEvent);