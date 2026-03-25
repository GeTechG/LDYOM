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
        local myUid = handle.uid

        while true do
            node_editor.bump_pure_generation()
            local condition = handle:getInput(2)
            if condition == nil then
                condition = handle:getInputDefault(2) or false
            end

            if not condition then
                return 1  -- follow "done" flow output
            end

            -- run body and wait for it to complete before next iteration
            if myUid then
                local bodyNodes = node_editor.get_next_flow_nodes(myUid, 0)
                for _, bodyUid in ipairs(bodyNodes) do
                    node_editor.run_flow_from(bodyUid)
                end
            end

            node_tasks.yield()
        end
    end
}

node_editor.register(desc)
