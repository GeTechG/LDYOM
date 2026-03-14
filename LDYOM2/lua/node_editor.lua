function node_editor.run_flow_from(startUID)
    local currentUID = startUID
    while currentUID do
        node_editor.bump_pure_generation()
        local executeFn = node_editor.get_node_execute_fn(currentUID)
        local handle = node_editor.get_node_handle(currentUID)
        local outPin = nil
        if executeFn and handle then
            outPin = executeFn(handle)
        end
        currentUID = node_editor.get_next_flow_node(currentUID, outPin or 0)
    end
end
