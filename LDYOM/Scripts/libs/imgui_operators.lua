local operators = {}

---@param a ImVec2
---@param b ImVec2
---@return ImVec2
operators.ImVec2Add = function(a, b)
    return ImVec2.new(a.x + b.x, a.y + b.y)
end

---@param a ImVec2
---@param b ImVec2
---@return ImVec2
operators.ImVec2Subtract = function(a, b)
    return ImVec2.new(a.x - b.x, a.y - b.y)
end

---@param a ImVec2
---@param b ImVec2
---@return ImVec2
operators.ImVec2Multiply = function(a, b)
    return ImVec2.new(a.x * b.x, a.y * b.y)
end

---@param a ImVec2
---@param b ImVec2
---@return ImVec2
operators.ImVec2Divide = function(a, b)
    return ImVec2.new(a.x / b.x, a.y / b.y)
end

---@param a ImVec2
---@return ImVec2
operators.ImVec2Negate = function(a)
    return ImVec2.new(-a.x, -a.y)
end

return operators