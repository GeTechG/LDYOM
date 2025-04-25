---@class LDNodeEditorContext
---@field context userdata
---@field name string
---@field nodes table<number, LDNodeEditorNode>
---@field __links LDNodeEditorLink[]
---@field newLinkPin LDNodeEditorPin | nil
---@field __nextUniqueId integer
---@field __loaded boolean

local LDNodeEditorContext = {}

LDNodeEditorContext.new = function(context, name)
    ---@type LDNodeEditorContext
    local newContext = {
        context = context,
        name = name,
        nodes = {},
        __links = {},
        __nextUniqueId = 0,
        __loaded = false
    }
    return newContext
end

---@param context LDNodeEditorContext
---@return integer
LDNodeEditorContext.getNextUniqueId = function(context)
    context.__nextUniqueId = context.__nextUniqueId + 100;
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
---@param node LDNodeEditorNode
LDNodeEditorContext.addNode = function(context, node)
    context.nodes[node.id] = node
end

---@param context LDNodeEditorContext
---@param id integer
---@return boolean
LDNodeEditorContext.isPinLinked = function(context, id)
    for _, link in ipairs(context.__links) do
        if link.inputId == id or link.outputId == id then
            return true
        end
    end
    return false;
end

---@param context LDNodeEditorContext
---@param id integer
LDNodeEditorContext.getLinkByPin = function(context, id)
    for _, link in ipairs(context.__links) do
        if link.inputId == id or link.outputId == id then
            return link
        end
    end
    return nil
end

---@param context LDNodeEditorContext
---@param id integer
LDNodeEditorContext.findPin = function(context, id)
    if not id then
        return nil
    end
    local nodeId = math.floor(id / 100) * 100;
    local node = LDNodeEditorContext.findNode(context, nodeId)
    if not node then
        return nil
    end
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
    return nil
end


---@param context LDNodeEditorContext
---@param id integer
LDNodeEditorContext.findNode = function(context, id)
    if not id then
        return nil
    end
    return context.nodes[id]
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