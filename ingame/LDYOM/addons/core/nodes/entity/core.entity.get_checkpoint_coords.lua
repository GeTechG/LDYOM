local TYPE = "core.entity.get_checkpoint_coords"

local desc = {
    type = TYPE,
    category = "entity",
    style = "function",
    is_pure = true,
    default_data = {
        entity_id = ""
    },
    pins = {
        {
            title = "x",
            type = "number",
            dir = "out"
        },
        {
            title = "y",
            type = "number",
            dir = "out"
        },
        {
            title = "z",
            type = "number",
            dir = "out"
        },
        {
            title = "radius",
            type = "number",
            dir = "out"
        }
    },
    on_draw = function(handle)
        local selectedId = handle:getData("entity_id") or ""

        local entities = scenes.get_entities()
        local label = "---"
        for i, e in ipairs(entities) do
            if tostring(e.id) == selectedId then
                label = e.name
                break
            end
        end

        ImGui.SetNextItemWidth(160)
        if ImGui.BeginCombo("##entity_get_checkpoint_coords", label, 0) then
            for i, e in ipairs(entities) do
                if e:hasComponent("checkpoint") then
                    local strId = tostring(e.id)
                    local isSel = (strId == selectedId)
                    if ImGui.Selectable(e.name .. "##" .. strId, isSel, 0, ImVec2.new(0, 0)) then
                        handle:setData("entity_id", strId)
                    end
                    if isSel then ImGui.SetItemDefaultFocus() end
                end
            end
            ImGui.EndCombo()
        end
    end,
    on_execute = function(handle)
        local id = handle:getData("entity_id")
        if not id or id == "" then
            handle:setOutput(1, 0)
            handle:setOutput(2, 0)
            handle:setOutput(3, 0)
            handle:setOutput(4, 0)
            return 0
        end

        local entities = scenes.get_entities()
        for i, e in ipairs(entities) do
            if tostring(e.id) == id then
                local comp = e:getComponent("checkpoint")
                if comp then
                    local checkpoint = CheckpointComponent.cast(comp)
                    local pos = e.position
                    handle:setOutput(1, pos[1])
                    handle:setOutput(2, pos[2])
                    handle:setOutput(3, pos[3])
                    handle:setOutput(4, checkpoint.radius)
                    return 0
                end
                break
            end
        end

        handle:setOutput(1, 0)
        handle:setOutput(2, 0)
        handle:setOutput(3, 0)
        handle:setOutput(4, 0)
        return 0
    end
}

node_editor.register(desc)
