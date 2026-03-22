local TYPE = "core.logic.xor"

---@type NodeDescriptor
local desc = {
    type = TYPE,
    category = "logic",
    style = "function",
    is_pure = true,
    pins = {
        {
            title = "a",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(1) then
                    local v = handle:getInputDefault(1)
                    local _, nv = ImGui.Checkbox("##xor_a", v or false)
                    handle:setInputDefault(1, nv)
                end
            end
        },
        {
            title = "b",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(2) then
                    local v = handle:getInputDefault(2)
                    local _, nv = ImGui.Checkbox("##xor_b", v or false)
                    handle:setInputDefault(2, nv)
                end
            end
        },
        { title = "", type = "bool", dir = "out" }
    },
    on_execute = function(handle)
        local a = handle:getInput(1) or handle:getInputDefault(1) or false
        local b = handle:getInput(2) or handle:getInputDefault(2) or false
        handle:setOutput(3, (a and not b) or (not a and b))
        return 0
    end
}

node_editor.register(desc)
