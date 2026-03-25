local TYPE = "core.wait"

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
            title = "duration_ms",
            type = "number",
            dir = "in",
            default = 1000,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(2) then
                    local v = handle:getInputDefault(2)
                    ImGui.SetNextItemWidth(80)
                    local ok, nv = ImGui.InputInt("##wait_duration", math.floor(v or 1000), 0)
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
        local duration = handle:getInput(2) or handle:getInputDefault(2) or 1000
        node_tasks.sleep(duration)
        return 0
    end
}

node_editor.register(desc)
