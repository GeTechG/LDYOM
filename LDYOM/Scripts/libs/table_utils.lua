
--- Removes all elements from the container that satisfy the predicate pred.
---@generic T
---@param t T[]
---@param pred fun(v: T): boolean
table.erase_if = function(t, pred)
    local i = 1;
    while i <= #t do
        if pred(t[i]) then
            table.remove(t, i);
        else
            i = i + 1;
        end
    end
end




--- Checks if the table contains the value.
---@generic T
---@param t T[]
---@param value T
table.contains = function(t, value)
    for _, v in ipairs(t) do
        if v == value then
            return true
        end
    end
    return false
end