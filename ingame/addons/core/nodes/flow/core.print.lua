local TYPE = "core.print"

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
            title = "value",
            type = "string",
            dir = "in",
            default = "",
            on_render = function(handle)
                if not handle then return end
                if (not handle:isInputConnected(2)) then
                    local valObj = handle:getInputDefault(2);
                    ImGui.SetNextItemWidth(80)
                    local result, newVal = ImGui.InputText("##value", valObj, 0, nil, nil)
                    if result then
                        handle:setInputDefault(2, newVal)
                    end
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
        local text = handle:getInput(2)
        if text == nil then
            text = handle:getData("value") or ""
        end
        log.info(tostring(text))
        return 0
    end
}

node_editor.register(desc)
