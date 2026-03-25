local TYPE = "core.boolean_to_string"

local desc = {
    type = TYPE,
    category = "string",
    style = "function",
    is_pure = true,
    pins = {
        {
            title = loc.trv("value"),
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(1) then
                    local v = handle:getInputDefault(1)
                    ImGui.SetNextItemWidth(80)
                    local ok, nv = ImGui.Checkbox("##fts_value", v)
                    if ok then handle:setInputDefault(1, nv) end
                end
            end
        },
        {
            title = loc.trv("text"),
            type = "string",
            dir = "out"
        }
    },
    on_execute = function(handle)
        local v = handle:getInput(1)
        if v == nil then v = handle:getInputDefault(1) or false end
        handle:setOutput(2, tostring(v))
        return 0
    end
}

node_editor.register(desc)
