local ed = require "scripts.Core.node_editor_entities"
local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"
local enums_id = require "scripts.Core.enums_id"

---@class {classname}
---@field static table

---@type BaseNode | {classname}
local {classname} = class("{classname}Node", ed.BaseNode)
{classname}.static.name_ = "core.nodes.{classname_snake}";
{classname}.static.type = ed.NodeType.{type};
{classname}.static.description = {description};

---@param id integer
function {classname}:initialize(id)
    ed.BaseNode.initialize(self, id);
    self.activation = {activation};
    self.pins = {{
        {pins}
    }}
end

function {classname}:draw()
    ImNodes.beginNode(self.id, self.class.name_, self.class.type, self.class.description);

    {draw}

    ImNodes.endNode();
end

---@param scene any
---@param tasklist any
---@param context NodeEditorContext
function {classname}:execute(scene, tasklist, context)
    local queryId = math.random(1, 100000);
    {get_params}

    {returns} = {clazz}Op["{method}"]({method_params});
    {copy_str}

    {call_next}
end

NodeEditor.registerNode("core.categories.{clazz_snake}", {classname});