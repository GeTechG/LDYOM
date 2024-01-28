---@class LDNodeEditorContext
---@field context userdata
---@field name string
---@field nodes LDNodeEditorNode[]
---@field __links LDNodeEditorLink[]
---@field newLinkPin LDNodeEditorPin | nil
---@field __nextUniqueId integer

local LDNodeEditorContext = {}

LDNodeEditorContext.new = function(context, name)
    ---@type LDNodeEditorContext
    local newContext = {
        context = context,
        name = name,
        nodes = {},
        __links = {},
        __nextUniqueId = 0
    }
    return newContext
end

---@param context LDNodeEditorContext
---@return integer
LDNodeEditorContext.getNextUniqueId = function(context)
    context.__nextUniqueId = context.__nextUniqueId + 1;
    return context.__nextUniqueId
end

---@param context LDNodeEditorContext
---@param inputId integer
---@param outputId integer
LDNodeEditorContext.addLink = function(context, inputId, outputId)
    local newLink = {
        id = LDNodeEditorContext.getNextUniqueId(context),
        inputId = inputId,
        outputId = outputId
    }
    table.insert(context.__links, newLink)
end

---@param context LDNodeEditorContext
---@param id integer
---@return boolean
LDNodeEditorContext.isPinLinked = function(context, id)
    local of = false;
    for _, link in ipairs(context.__links) do
        if link.inputId == id or link.outputId == id then
            of = true;
            break;
        end
    end
    return of;
end

---@param context LDNodeEditorContext
---@param id integer
LDNodeEditorContext.findPin = function(context, id)
    if not id then
        return nil
    end
    for _, node in ipairs(context.nodes) do
        for _, pin in ipairs(node.inputs) do
            if pin.id == id then
                return pin
            end
        end
        for _, pin in ipairs(node.outputs) do
            if pin.id == id then
                return pin
            end
        end
    end
    return nil
end


---@param context LDNodeEditorContext
---@param id integer
LDNodeEditorContext.findNode = function(context, id)
    if not id then
        return nil
    end
    for _, node in ipairs(context.nodes) do
        if node.id == id then
            return node
        end
    end
    return nil
end

---@param context LDNodeEditorContext
---@param id integer
LDNodeEditorContext.findLink = function(context, id)
    if not id then
        return nil
    end
    for _, link in ipairs(context.__links) do
        if link.id == id then
            return link
        end
    end
    return nil
end

return LDNodeEditorContext