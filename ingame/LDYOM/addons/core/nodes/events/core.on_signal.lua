local TYPE = "core.on_signal"

local desc = {
    type = TYPE,
    category = "events",
    style = "event",
    default_data = { signal_name = "" },
    pins = {
        {
            title = "",
            type = "flow",
            dir = "out"
        }
    },
    on_draw = function(handle)
        local v = handle:getData("signal_name") or ""
        ImGui.SetNextItemWidth(160)
        local ok, nv = ImGui.InputText("##on_signal_name", v)
        if ok then handle:setData("signal_name", nv) end
    end,
    on_execute = function(handle)
        return 0
    end
}

node_editor.register(desc)
