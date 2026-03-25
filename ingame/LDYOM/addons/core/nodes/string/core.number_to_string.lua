local TYPE = "core.number_to_string"

local desc = {
    type = TYPE,
    category = "string",
    style = "function",
    is_pure = true,
    pins = {
        {
            title = "value",
            type = "number",
            dir = "in",
            default = 0,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(1) then
                    local v = handle:getInputDefault(1)
                    ImGui.SetNextItemWidth(80)
                    local ok, nv = ImGui.InputFloat("##fts_value", v, 0, 0, "%.3f", 0)
                    if ok then handle:setInputDefault(1, nv) end
                end
            end
        },
        {
            title = "text",
            type = "string",
            dir = "out"
        }
    },
    on_execute = function(handle)
        local v = handle:getInput(1)
        if v == nil then v = handle:getInputDefault(1) or 0 end
        handle:setOutput(2, tostring(v))
        return 0
    end
}

node_editor.register(desc)
