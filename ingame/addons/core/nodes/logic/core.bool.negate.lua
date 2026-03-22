local fa = require("fa");

local TYPE = "core.bool.negate"

---@type NodeDescriptor
local desc = {
    type = TYPE,
    category = "logic",
    style = "function",
    is_pure = true,
    pins = {
        {
            title = "",
            type = "bool",
            dir = "in",
            default = false,
        },
        {
            title = "",
            type = "bool",
            dir = "out"
        }
    },
    on_draw = function(handle)
        ImGui.Text(fa.ICON_FA_TILDE)
    end,
    on_execute = function(handle)
        local v = handle:getInput(1)
        if v == nil then v = handle:getInputDefault(1) or false end
        handle:setOutput(2, not v)
        return 0
    end
}

node_editor.register(desc)
