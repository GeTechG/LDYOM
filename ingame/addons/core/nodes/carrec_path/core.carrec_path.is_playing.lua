local TYPE = "core.carrec_path.is_playing"

local desc = {
    type = TYPE,
    category = "carrec_path",
    style = "function",
    is_pure = true,
    pins = {
        { title = "nodes.core.carrec_path.vehicle",    type = "number", dir = "in" },
        { title = "nodes.core.carrec_path.is_playing", type = "bool",  dir = "out" },
    },
    on_execute = function(handle)
        local vehicleRef = math.floor(handle:getInput(1) or -1)
        if vehicleRef == -1 then
            handle:setOutput(2, false)
            return 0
        end
        handle:setOutput(2, carrec_paths.is_playing(vehicleRef))
        return 0
    end
}

node_editor.register(desc)
