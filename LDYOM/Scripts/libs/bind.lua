--- Bind a function to a set of arguments.
---@param func function
---@param ... any
---@return function
function bind(func, ...)
    local rest = {...}

    return function (...)
        local args = {}

        for i = 1, #rest do
            args[i] = rest[i]
        end

        for i = 1, select("#", ...) do
            table.insert(args, select(i, ...))
        end

        return func(unpack(args))
    end
end

return bind