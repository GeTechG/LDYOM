local TYPE = "core.branch"

local desc = {
    type = TYPE,
    category = "flow",
    style = "branch",
    pins = {
        {
            title = "",
            type = "flow",
            dir = "in"
        },
        {
            title = "condition",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(2) then
                    local v = handle:getInputDefault(2)
                    local _, nv = ImGui.Checkbox("##condition", v or false)
                    handle:setInputDefault(2, nv)
                end
            end
        },
        {
            title = "true",
            type = "flow",
            dir = "out"
        },
        {
            title = "false",
            type = "flow",
            dir = "out"
        }
    },
    on_execute = function(handle)
        local condition = handle:getInput(2)
        if condition == nil then
            condition = handle:getInputDefault(2) or false
        end
        if condition then
            return 0
        else
            return 1
        end
    end
}

node_editor.register(desc)
