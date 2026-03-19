local TYPE = "core.main_loop"

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
    on_execute = function(handle)
        return 0
    end
}

node_editor.register(desc)
