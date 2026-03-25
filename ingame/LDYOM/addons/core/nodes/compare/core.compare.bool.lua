local TYPE = "core.compare.bool"

local ops = { "==", "!=" }
local op_keys = { "eq", "ne" }

local function execute_op(op, a, b)
    if op == "eq" then return a == b
    elseif op == "ne" then return a ~= b
    end
    return false
end

---@type NodeDescriptor
local desc = {
    type = TYPE,
    category = "compare",
    style = "function",
    is_pure = true,
    default_data = { op = "eq" },
    pins = {
        {
            title = "a",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(1) then
                    local v = handle:getInputDefault(1)
                    local _, nv = ImGui.Checkbox("##cb_a", v or false)
                    handle:setInputDefault(1, nv)
                end
            end
        },
        {
            title = "b",
            type = "bool",
            dir = "in",
            default = false,
            on_render = function(handle)
                if not handle then return end
                if not handle:isInputConnected(2) then
                    local v = handle:getInputDefault(2)
                    local _, nv = ImGui.Checkbox("##cb_b", v or false)
                    handle:setInputDefault(2, nv)
                end
            end
        },
        { title = "", type = "bool", dir = "out" }
    },
    on_draw = function(handle)
        local current = handle:getData("op") or "eq"
        local label = "=="
        for i, k in ipairs(op_keys) do
            if k == current then label = ops[i] break end
        end
        ImGui.SetNextItemWidth(60)
        if ImGui.BeginCombo("##cb_op", label, 0) then
            for i, k in ipairs(op_keys) do
                local isSel = (k == current)
                if ImGui.Selectable(ops[i], isSel, 0, ImVec2.new(0, 0)) then
                    handle:setData("op", k)
                end
                if isSel then ImGui.SetItemDefaultFocus() end
            end
            ImGui.EndCombo()
        end
    end,
    on_execute = function(handle)
        local a = handle:getInput(1) or handle:getInputDefault(1) or false
        local b = handle:getInput(2) or handle:getInputDefault(2) or false
        local op = handle:getData("op") or "eq"
        handle:setOutput(3, execute_op(op, a, b))
        return 0
    end
}

node_editor.register(desc)
