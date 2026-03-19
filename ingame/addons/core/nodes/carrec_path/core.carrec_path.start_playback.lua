local TYPE = "core.carrec_path.start_playback"

local desc = {
    type = TYPE,
    category = "carrec_path",
    style = "function",
    default_data = {
        path_id = ""
    },
    pins = {
        { title = "",                               type = "flow",   dir = "in" },
        { title = "nodes.core.carrec_path.vehicle", type = "number", dir = "in" },
        {
            title = "nodes.core.carrec_path.use_ai",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(3) then
                    local v = handle:getInputDefault(3)
                    local bv = (v == true or v == 1)
                    local ok, nv = ImGui.Checkbox("##cr_use_ai", bv)
                    if ok then handle:setInputDefault(3, nv) end
                end
            end
        },
        {
            title = "nodes.core.carrec_path.looped",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(4) then
                    local v = handle:getInputDefault(4)
                    local bv = (v == true or v == 1)
                    local ok, nv = ImGui.Checkbox("##cr_looped", bv)
                    if ok then handle:setInputDefault(4, nv) end
                end
            end
        },
        { title = "",                               type = "flow", dir = "out" },
        { title = "nodes.core.carrec_path.success", type = "bool", dir = "out" },
    },
    on_draw = function(handle)
        local selectedId = handle:getData("path_id") or ""

        local paths = carrec_paths.get_paths()
        local label = "---"
        for _, p in ipairs(paths) do
            if p.uuid == selectedId then
                label = p.name
                break
            end
        end

        ImGui.SetNextItemWidth(160)
        if ImGui.BeginCombo("##cr_path_start", label, 0) then
            for _, p in ipairs(paths) do
                local isSel = (p.uuid == selectedId)
                if ImGui.Selectable(p.name .. "##" .. p.uuid, isSel, 0, ImVec2.new(0, 0)) then
                    handle:setData("path_id", p.uuid)
                end
                if isSel then ImGui.SetItemDefaultFocus() end
            end
            ImGui.EndCombo()
        end
    end,
    on_execute = function(handle)
        local vehicleRef = math.floor(handle:getInput(2) or -1)
        local pathId     = handle:getData("path_id") or ""
        local useAI      = handle:getInput(3) or handle:getInputDefault(3) or false
        local looped     = handle:getInput(4) or handle:getInputDefault(4) or false

        if pathId == "" or vehicleRef == -1 then
            handle:setOutput(6, false)
            return 0
        end

        local ok = carrec_paths.start_playback(vehicleRef, pathId, useAI == true or useAI == 1,
        looped == true or looped == 1)
        handle:setOutput(6, ok)
        return 0
    end
}

node_editor.register(desc)
