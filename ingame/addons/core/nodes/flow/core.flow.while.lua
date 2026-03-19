local TYPE = "core.flow.while"

local desc = {
    type = TYPE,
    category = "flow",
    style = "branch",
    pins = {
        {
            title = "",
            type = "flow",
            dir = "in"
        },
        {
            title = "condition",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(2) then
                    local v = handle:getInputDefault(2)
                    local _, nv = ImGui.Checkbox("##while_condition", v or false)
                    handle:setInputDefault(2, nv)
                end
            end
        },
        {
            title = "body",
            type = "flow",
            dir = "out"
        },
        {
            title = "done",
            type = "flow",
            dir = "out"
        }
    },
    on_execute = function(handle)
        -- find our own uid by matching the handle reference
        local myUid = nil
        local nodes = node_editor.get_scene_nodes()
        for _, nodeInfo in ipairs(nodes) do
            if nodeInfo.type == TYPE then
                local h = node_editor.get_node_handle(nodeInfo.uid)
                if h == handle then
                    myUid = nodeInfo.uid
                    break
                end
            end
        end

        while true do
            node_editor.bump_pure_generation()
            local condition = handle:getInput(2)
            if condition == nil then
                condition = handle:getInputDefault(2) or false
            end

            if not condition then
                return 1  -- follow "done" flow output
            end

            -- spawn body as a separate task each frame (fixed key = at most one body at a time)
            if myUid then
                local bodyUid = node_editor.get_next_flow_node(myUid, 0)
                if bodyUid then
                    local bodyKey = "while_body_" .. myUid
                    node_tasks.run(bodyKey, function()
                        node_editor.run_flow_from(bodyUid)
                    end)
                end
            end

            node_tasks.yield()
        end
    end
}

node_editor.register(desc)
