local _flow_branch_counter = 0

function node_editor.run_flow_from(startUID)
    _flow_branch_counter = _flow_branch_counter + 1
    local key = "_flow_branch_" .. _flow_branch_counter
    tasks.run(key, function()
        local currentUID = startUID
        while currentUID do
            node_editor.bump_pure_generation()
            local executeFn = node_editor.get_node_execute_fn(currentUID)
            local handle = node_editor.get_node_handle(currentUID)
            local outPin = nil
            if executeFn and handle then
                outPin = executeFn(handle)
            end
            local nextNodes = node_editor.get_next_flow_nodes(currentUID, outPin or 0)
            local count = #nextNodes
            if count == 0 then
                break
            elseif count == 1 then
                currentUID = nextNodes[1]
            else
                -- Spawn all branches as independent tasks
                for i = 1, count do
                    node_editor.run_flow_from(nextNodes[i])
                end
                break
            end
        end
    end)
end
