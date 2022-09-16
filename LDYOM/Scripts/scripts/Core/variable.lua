local class = require "libs.middleclass"
local ffi   = require "ffi"
local constants = require "scripts.Core.constants"
local ed = require "scripts.Core.node_editor_entities"

---@class Variable
---@field name ffi.cdata*;
---@field type ValueType
---@field value ffi.cdata*
local Variable = class("Variable");

function Variable:initialize()
    self.name = ffi.new("char["..constants.TEXT_SIZE.."]", "Var "..tostring(math.random(9999)));
    self:setType(ValueType.Number);
end

---@param type ValueType
function Variable:setType(type)
    if type == ValueType.Number then
        self.type = ValueType.Number;
        self.value = ffi.new("float[1]", {0});
    elseif type == ValueType.Boolean then
        self.type = ValueType.Boolean;
        self.value = ffi.new("bool[1]", {false});
    elseif type == ValueType.String then
        self.type = ValueType.String;
        self.value = ffi.new("char["..constants.TEXT_SIZE.."]", "");
    end
end

bitser.registerClass(Variable)

return Variable;