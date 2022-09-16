local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"

---@class StartSceneNode
---@field static table

---@type BaseNode | StartSceneNode
local StartSceneNode = class("StartSceneNode", ed.BaseNode)
StartSceneNode.static.name_ = "core.nodes.start_scene";
StartSceneNode.static.type = ed.NodeType.Event;

---@param id integer
function StartSceneNode:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = false;
    self.pins = {
        ed.Pin:new(id + 1, "", ed.PinType.Out, ed.ValueType.Void)
    }
end

function StartSceneNode:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type);

    ImNodes.beginOutputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endOutputAttribute();

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function StartSceneNode:execute(scene, tasklist, context)
    context:callNodes(scene, tasklist, self.id + 1);
end

---@param scene Scene
global_data.signals.onStartScene[#global_data.signals.onStartScene + 1] = function(scene, tasklist)
    local context = global_data.ed_contexts[scene:getId()];
    for _, node in pairs(context.nodes) do
        if node.class == StartSceneNode then
            Tasker.addTask(tasklist, function()
                node:execute(scene, tasklist, context);
            end)
        end
    end
end

NodeEditor.registerNode("core.categories.main", StartSceneNode);