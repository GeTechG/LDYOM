local TYPE = "core.logic.is_current_objective"

---@type NodeDescriptor
local desc = {
    type = TYPE,
    category = "logic",
    style = "function",
    is_pure = true,
    default_data = { objective_index = 0 },
    pins = {
        { title = "", type = "bool", dir = "out" }
    },
    on_draw = function(handle)
        local selected = handle:getData("objective_index") or 0

        local objectives = scenes.get_objectives()
        local count = #objectives

        local label = string.format("[%d] ???", selected)
        if selected >= 0 and selected < count then
            label = string.format("[%d] %s", selected, objectives[selected + 1].name)
        end

        ImGui.SetNextItemWidth(160)
        if ImGui.BeginCombo("##ico_obj", label, 0) then
            for i, obj in ipairs(objectives) do
                local idx = i - 1
                local isSel = (selected == idx)
                local itemLabel = string.format("[%d] %s", idx, obj.name)
                if ImGui.Selectable(itemLabel, isSel, 0, ImVec2.new(0, 0)) then
                    handle:setData("objective_index", idx)
                end
                if isSel then ImGui.SetItemDefaultFocus() end
            end
            ImGui.EndCombo()
        end
    end,
    on_execute = function(handle)
        local idx = handle:getData("objective_index") or 0
        handle:setOutput(1, project_player.current_objective_index == idx)
        return 0
    end
}

node_editor.register(desc)
