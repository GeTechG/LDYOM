local TYPE = "core.entity.despawn"

local desc = {
    type = TYPE,
    category = "entity",
    style = "function",
    default_data = {
        entity_id = ""
    },
    pins = {
        {
            title = "",
            type = "flow",
            dir = "in"
        },
        {
            title = "",
            type = "flow",
            dir = "out"
        }
    },
    on_draw = function(handle)
        local selectedId   = handle:getData("entity_id")   or ""

        local entities = scenes.get_entities()
        local label = "---"
        for i, e in ipairs(entities) do
            if tostring(e.id) == selectedId then
                label = e.name
                break
            end
        end

        ImGui.SetNextItemWidth(160)
        if ImGui.BeginCombo("##entity_despawn", label, 0) then
            for i, e in ipairs(entities) do
                local strId = tostring(e.id)
                local isSel = (strId == selectedId)
                if ImGui.Selectable(e.name .. "##" .. strId, isSel, 0, ImVec2.new(0, 0)) then
                    handle:setData("entity_id", strId)
                end
                if isSel then ImGui.SetItemDefaultFocus() end
            end
            ImGui.EndCombo()
        end
    end,
    on_execute = function(handle)
        local id = handle:getData("entity_id")
        if id and id ~= "" then
            scenes.despawn_entity(id)
        end
        return 0
    end
}

node_editor.register(desc)
