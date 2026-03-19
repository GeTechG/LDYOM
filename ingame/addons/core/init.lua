local function findLast(haystack, needle)
    local found = haystack:reverse():find(needle:reverse(), nil, true)
    if found then
        return haystack:len() - needle:len() - found + 2
    else
        return found
    end
end

local function parsePath(path)
    local separatorIndex = findLast(path, "\\")
    if not separatorIndex then
        return nil, "Invalid path format"
    end

    local category = path:sub(1, separatorIndex - 1)
    local type_ = path:sub(separatorIndex + 1)

    return {
        type = type_,
        category = category,
        path = path
    }
end

local nodesPath = "LDYOM/addons/core/nodes";
---@type function | nil
local on_scene_started_disconnect = nil;
---@type function | nil
local on_objective_started_disconnect = nil;

register_addon({
    id = "skic.core",
    name = "core",
    version = "1.0.0",
    description = "Core ldyom addon providing essential functions and utilities.",
    author = "SKIC",
    priority = 100,
    on_load = function()
        addon_global_set("test", 5)
        on_scene_started_disconnect = events.on_scene_started(function(sceneId)
            local nodes = node_editor.get_scene_nodes();
            for _, nodeInfo in ipairs(nodes) do
                if nodeInfo.type == "core.start_scene" then
                    local uid = nodeInfo.uid
                    node_tasks.run("scene_chain_" .. sceneId .. "_" .. tostring(uid), function()
                        node_editor.run_flow_from(uid)
                    end)
                elseif nodeInfo.type == "core.main_loop" then
                    local uid = nodeInfo.uid
                    node_tasks.run("main_loop_" .. sceneId .. "_" .. tostring(uid), function()
                        while true do
                            node_editor.run_flow_from(uid)
                            node_tasks.yield()
                        end
                    end)
                end
            end
        end)

        on_objective_started_disconnect = events.on_objective_started(function(index)
            local nodes = node_editor.get_scene_nodes();
            for _, nodeInfo in ipairs(nodes) do
                if nodeInfo.type == "core.on_objective_started" then
                    local uid = nodeInfo.uid
                    local handle = node_editor.get_node_handle(uid)
                    if handle then
                        local filter = handle:getData("objective_index")
                        -- fire only if filter is "all" (-1) or matches the current objective
                        if filter ~= nil and filter ~= -1 and filter ~= index then
                            goto continue
                        end
                        handle:setOutput(2, index)
                    end
                    node_tasks.run("objective_chain_" .. tostring(index) .. "_" .. tostring(uid), function()
                        node_editor.run_flow_from(uid)
                    end)
                    ::continue::
                end
            end
        end)

        local nodes_dirs = Dirs.entries(nodesPath, true)
        for _, node_file in ipairs(nodes_dirs) do
            if string.match(node_file, "%.lua$") then
                local nodeInfo = parsePath(node_file)
                if nodeInfo then
                    nodeInfo.category = string.gsub(nodeInfo.category, "\\", ".")
                    nodeInfo.type = nodeInfo.type:sub(1, -5)
                    nodeInfo.path = nodesPath .. "/" .. nodeInfo.path;
                    node_editor.register_lazy(nodeInfo);
                end
            end
        end

        log.info("Example addon loaded successfully!")
    end,
    on_unload = function()
        if (on_scene_started_disconnect ~= nil) then
            on_scene_started_disconnect();
        end
        if (on_objective_started_disconnect ~= nil) then
            on_objective_started_disconnect();
        end
    end
});
