
--- Returns a shallow copy of the table.
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