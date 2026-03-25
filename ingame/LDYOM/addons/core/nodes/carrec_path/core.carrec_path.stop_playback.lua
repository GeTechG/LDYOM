local TYPE = "core.carrec_path.stop_playback"

local desc = {
    type = TYPE,
    category = "carrec_path",
    style = "function",
    pins = {
        { title = "",                                   type = "flow",  dir = "in" },
        { title = "nodes.core.carrec_path.vehicle",   type = "number", dir = "in" },
        { title = "",        type = "flow",  dir = "out" },
    },
    on_execute = function(handle)
        local vehicleRef = math.floor(handle:getInput(2) or -1)
        if vehicleRef ~= -1 then
            carrec_paths.stop_playback(vehicleRef)
        end
        return 0
    end
}

node_editor.register(desc)
