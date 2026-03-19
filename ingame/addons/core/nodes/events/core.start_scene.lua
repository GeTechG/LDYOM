local TYPE = "core.start_scene"

local desc = {
    type = TYPE,
    category = "events",
    style = "event",
    pins = {
        {
            title = "",
            type = "flow",
            dir = "out"
        }
    },
    on_draw = function(handle)
    end,
    on_execute = function(handle)
        return 0;
    end
}

node_editor.register(desc);
