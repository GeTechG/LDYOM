local TYPE = "core.global_var.get"

local function varTypeToPin(t)
    if t == "string" then return "string"
    elseif t == "bool" then return "bool"
    else return "number"
    end
end

local desc = {
    type = TYPE,
    category = "global_var",
    style = "variable",
    is_pure = true,
    default_data = {
        var_name = ""
    },
    pins = {
        -- output pin 1: value (type updated via setOutputPinType when variable selection changes)
        { title = "value", type = "number", dir = "out" },
    },
    on_draw = function(handle)
        local selected = handle:getData("var_name") or ""
        local vars = global_vars.list()

        -- Determine the pin type for the currently selected variable
        local pinType = "number"
        for _, v in ipairs(vars) do
            if v.name == selected then
                pinType = varTypeToPin(v.type)
                break
            end
        end

        -- Update pin type only when it changed (avoids disconnecting links unnecessarily)
        if handle:getOutputPinType(1) ~= pinType then
            handle:setOutputPinType(1, pinType)
        end

        -- Variable selector combo
        local label = selected ~= "" and selected or "---"
        ImGui.SetNextItemWidth(160)
        if ImGui.BeginCombo("##gv_get_name", label, 0) then
            for _, v in ipairs(vars) do
                local isSel = (v.name == selected)
                local itemLabel = v.name .. " (" .. v.type .. ")"
                if ImGui.Selectable(itemLabel, isSel, 0, ImVec2.new(0, 0)) then
                    handle:setData("var_name", v.name)
                end
                if isSel then ImGui.SetItemDefaultFocus() end
            end
            ImGui.EndCombo()
        end
    end,
    on_execute = function(handle)
        local name = handle:getData("var_name") or ""
        if name == "" then return 0 end
        handle:setOutput(1, global_vars.get(name))
        return 0
    end
}

node_editor.register(desc)
