local TYPE = "core.string_concat"

local desc = {
    type = TYPE,
    category = "string",
    style = "function",
    is_pure = true,
    pins = {
        {
            title = "A",
            type = "string",
            dir = "in",
            default = "",
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(1) then
                    local v = handle:getInputDefault(1)
                    ImGui.SetNextItemWidth(80)
                    local ok, nv = ImGui.InputText("##sc_a", v, 0, nil, nil)
                    if ok then handle:setInputDefault(1, nv) end
                end
            end
        },
        {
            title = "B",
            type = "string",
            dir = "in",
            default = "",
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(2) then
                    local v = handle:getInputDefault(2)
                    ImGui.SetNextItemWidth(80)
                    local ok, nv = ImGui.InputText("##sc_b", v, 0, nil, nil)
                    if ok then handle:setInputDefault(2, nv) end
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
        local a = handle:getInput(1)
        if a == nil then a = handle:getInputDefault(1) or "" end
        local b = handle:getInput(2)
        if b == nil then b = handle:getInputDefault(2) or "" end
        handle:setOutput(3, tostring(a) .. tostring(b))
        return 0
    end
}

node_editor.register(desc)
