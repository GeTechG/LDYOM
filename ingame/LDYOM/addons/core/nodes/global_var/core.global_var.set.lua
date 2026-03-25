local TYPE = "core.global_var.set"

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
    default_data = {
        var_name = ""
    },
    pins = {
        -- input pin 1: flow in
        { title = "", type = "flow", dir = "in" },
        -- input pin 2: value (type updated via setInputPinType when variable selection changes)
        { title = "value", type = "number", dir = "in" },
        -- output pin 1: flow out
        { title = "", type = "flow", dir = "out" },
    },
    on_draw = function(handle)
        local vars = global_vars.list()
        local selected = handle:getData("var_name") or ""

        -- Variable selector combo
        local label = selected ~= "" and selected or "---"
        ImGui.SetNextItemWidth(160)
        if ImGui.BeginCombo("##gv_set_name", label, 0) then
            for _, v in ipairs(vars) do
                local isSel = (v.name == selected)
                local itemLabel = v.name .. " (" .. v.type .. ")"
                if ImGui.Selectable(itemLabel, isSel, 0, ImVec2.new(0, 0)) then
                    handle:setData("var_name", v.name)
                    -- Reset default to a sensible zero value for the new type
                    if v.type == "string" then
                        handle:setInputDefault(2, "")
                    elseif v.type == "bool" then
                        handle:setInputDefault(2, false)
                    else
                        handle:setInputDefault(2, 0)
                    end
                    selected = v.name
                end
                if isSel then ImGui.SetItemDefaultFocus() end
            end
            ImGui.EndCombo()
        end

        -- Determine the pin type for the currently selected variable
        local pinType = "number"
        for _, v in ipairs(vars) do
            if v.name == selected then
                pinType = varTypeToPin(v.type)
                break
            end
        end

        -- Update pin type only when it changed
        if handle:getInputPinType(2) ~= pinType then
            handle:setInputPinType(2, pinType)
        end

        -- Render the default-value widget when the input pin is not connected
        if not handle:isInputConnected(2) then
            if pinType == "string" then
                local v = handle:getInputDefault(2) or ""
                ImGui.SetNextItemWidth(100)
                local ok, nv = ImGui.InputText("##gv_set_val", tostring(v), 0, nil, nil)
                if ok then handle:setInputDefault(2, nv) end
            elseif pinType == "bool" then
                local v = handle:getInputDefault(2)
                local bv = (v == true or v == 1)
                local ok, nv = ImGui.Checkbox("##gv_set_val", bv)
                if ok then handle:setInputDefault(2, nv) end
            else
                local v = handle:getInputDefault(2) or 0
                ImGui.SetNextItemWidth(100)
                local ok, nv = ImGui.InputFloat("##gv_set_val", v, 0, 0, "%.3f", 0)
                if ok then handle:setInputDefault(2, nv) end
            end
        end
    end,
    on_execute = function(handle)
        local name = handle:getData("var_name") or ""
        if name == "" then return end

        local val = handle:getInput(2)
        if val ~= nil then
            global_vars.set(name, val)
        end
    end
}

node_editor.register(desc)
