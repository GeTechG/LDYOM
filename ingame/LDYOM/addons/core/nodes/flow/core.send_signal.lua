local TYPE = "core.send_signal"

local desc = {
    type = TYPE,
    category = "flow",
    style = "function",
    pins = {
        {
            title = "",
            type = "flow",
            dir = "in"
        },
        {
            title = "signal",
            type = "string",
            dir = "in",
            default = "default",
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(2) then
                    local v = handle:getInputDefault(2)
                    ImGui.SetNextItemWidth(120)
                    local ok, nv = ImGui.InputText("##send_signal_name", v or "default")
                    if ok then handle:setInputDefault(2, nv) end
                end
            end
        },
        {
            title = "",
            type = "flow",
            dir = "out"
        }
    },
    on_execute = function(handle)
        local signal = handle:getInput(2) or handle:getInputDefault(2) or "default"
        project_player:send_signal(signal)
        return 0
    end
}

node_editor.register(desc)
