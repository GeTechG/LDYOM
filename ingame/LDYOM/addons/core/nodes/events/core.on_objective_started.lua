local TYPE = "core.on_objective_started"

local desc = {
    type = TYPE,
    category = "events",
    style = "event",
    default_data = {
        objective_index = -1
    },
    pins = {
        {
            title = "",
            type = "flow",
            dir = "out"
        },
        {
            title = "index",
            type = "number",
            dir = "out"
        }
    },
    on_draw = function(handle)
        local selected = handle:getData("objective_index") or -1

        local objectives = scenes.get_objectives()
        local count = #objectives

        -- Build display label
        local label = loc.trv("tags_all")
        if selected >= 0 and selected < count then
            label = string.format("[%d] %s", selected, objectives[selected + 1].name)
        end

        ImGui.SetNextItemWidth(160)
        if ImGui.BeginCombo("##obj_filter", label, 0) then
            -- "All" option
            local isAll = (selected == -1)
            if ImGui.Selectable(loc.trv("tags_all"), isAll, 0, ImVec2.new(0, 0)) then
                handle:setData("objective_index", -1)
            end
            if isAll then ImGui.SetItemDefaultFocus() end

            for i, obj in ipairs(objectives) do
                local idx = i - 1
                local isSel = (selected == idx)
                local itemLabel = string.format("[%d] %s", idx, obj.name)
                if ImGui.Selectable(itemLabel, isSel,0, ImVec2.new(0, 0)) then
                    handle:setData("objective_index", idx)
                end
                if isSel then ImGui.SetItemDefaultFocus() end
            end

            ImGui.EndCombo()
        end
    end,
    on_execute = function(handle)
        return 0;
    end
}

node_editor.register(desc);
