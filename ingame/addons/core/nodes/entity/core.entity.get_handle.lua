local TYPE = "core.entity.get_handle"

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
            title = "handle",
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
        if ImGui.BeginCombo("##entity_get_handle", label, 0) then
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
        if not id or id == "" then
            handle:setOutput(1, -1)
            return 0
        end

        local entities = scenes.get_entities()
        for i, e in ipairs(entities) do
            if tostring(e.id) == id then
                local actor = e:getComponent("actor");
                if actor then
                    handle:setOutput(1, ActorComponent.cast(actor):getPedRef())
                    return 0
                end

                local vehicle = e:getComponent("vehicle");
                if vehicle then
                    handle:setOutput(1, VehicleComponent.cast(vehicle):getVehicleRef())
                    return 0
                end

                local obj = e:getComponent("object");
                if obj then
                    handle:setOutput(1, ObjectComponent.cast(obj):getObjectRef())
                    return 0
                end

                break
            end
        end

        handle:setOutput(1, -1)
        return 0
    end
}

node_editor.register(desc)
