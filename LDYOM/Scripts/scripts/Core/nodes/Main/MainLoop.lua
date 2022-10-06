local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"

---@class MainLoopNode
---@field static table

---@type BaseNode | MainLoopNode
local MainLoopNode = class("MainLoopNode", ed.BaseNode)
MainLoopNode.static.name_ = "core.nodes.main_loop";
MainLoopNode.static.type = ed.NodeType.Loops;

---@param id integer
function MainLoopNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function MainLoopNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginOutputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function MainLoopNode:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 1);
end

---@param scene Scene
global_data.signals.onMainLoop[#global_data.signals.onMainLoop + 1] = function(scene, tasklist)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == MainLoopNode then
            Tasker.addTask(tasklist, function()
                node:execute(scene, tasklist, context);
            end)
        end
    end
end

NodeEditor.registerNode("core.categories.main", MainLoopNode);