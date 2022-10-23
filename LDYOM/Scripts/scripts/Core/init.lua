require "libs.enums"
require "libs.require"
local ed = require "scripts.Core.node_editor_entities"
local variable = require "scripts.Core.variable"
local ffi      = require "ffi"

local data = {}

data.info = {
    name = "Core",
    version = "0.1"
}

addLocalizationDirectory("Core/Languages");

if global_data == nil then
    global_data = {};
end
global_data.NodeEditorContext = ed.NodeEditorContext;
global_data.Variable = variable;
---@type NodeEditorContext[]
global_data.ed_contexts = {};
global_data.variables = {};
global_data.signals.init[#global_data.signals.init + 1] = function()
    global_data.ed_contexts[ProjectsService.getInstance():getCurrentProject():getCurrentSceneIndex()] = ed.NodeEditorContext:new();
end
global_data.signals.onNewScene[#global_data.signals.onNewScene + 1] = function(sceneId)
    global_data.ed_contexts[sceneId] = ed.NodeEditorContext:new();
end
global_data.signals.onDeleteScene[#global_data.signals.onDeleteScene + 1] = function(sceneId)
    global_data.ed_contexts[sceneId] = nil;
end
global_data.signals.onChangeScene[#global_data.signals.onChangeScene + 1] = function(sceneId)
end
global_data.signals.onStartProject[#global_data.signals.onStartProject + 1] = function()
    global_data["variablesR"] = {};
    for i, var in pairs(global_data["variables"]) do
        if var.type == ValueType.Number then
            global_data["variablesR"][i] = ffi.new("float[1]", {var.value[0]});
        elseif var.type == ValueType.Boolean then
            global_data["variablesR"][i] = ffi.new("bool[1]", {var.value[0]});
        elseif var.type == ValueType.String then
            global_data["variablesR"][i] = ffi.new("char["..constants.TEXT_SIZE.."]", ffi.string(var.value));
        end
    end
end
global_data.signals.saveProject[#global_data.signals.saveProject + 1] = function()
    return bitser.dumps(global_data.variables);
end
global_data.signals.loadProject[#global_data.signals.loadProject + 1] = function(data_load)
    global_data.variables = bitser.loads(data_load);
    global_data.ed_contexts = {};
end
global_data.signals.saveScene[#global_data.signals.saveScene + 1] = function(sceneId)
    return global_data.ed_contexts[sceneId]:save();
end
global_data.signals.loadScene[#global_data.signals.loadScene + 1] = function(sceneId, data_load)
    global_data.ed_contexts[sceneId] = ed.NodeEditorContext:new();
    global_data.ed_contexts[sceneId]:load(data_load);
end

global_data.signals.saveGame[#global_data.signals.saveGame+1] = function (sceneId, data)
    data["core"] = {
        variablesR = global_data["variablesR"];
    }
end

global_data.signals.loadGame[#global_data.signals.loadGame+1] = function (sceneId, data)
    global_data["variablesR"] = data["variablesR"];
end

req.tree("scripts.Core.nodes")

return data;