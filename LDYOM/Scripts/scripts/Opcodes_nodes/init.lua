function init()
    ---@module "ld.node_type"

    local LDNodeEditor = require("ld.node_editor")

    local editor = scripts["core"]["editor"]

    function findLast(haystack, needle)
        local found = haystack:reverse():find(needle:reverse(), nil, true)
        if found then
            return haystack:len() - needle:len() - found + 2
        else
            return found
        end
    end

    local function parsePath(path)
        local separatorIndex = findLast(path, "\\")
        if not separatorIndex then
            return nil, "Invalid path format"
        end
        
        local category = path:sub(1, separatorIndex - 1)
        local name = path:sub(separatorIndex + 1)
        
        return {
            name = name,
            category = category
        }
    end

    local nodes_dirs = Dirs.entries("LDYOM/Scripts/scripts/Opcodes_nodes/nodes", true)
    for _, node_file in ipairs(nodes_dirs) do
        if string.match(node_file, "%.lua$") then
            local nodeInfo = parsePath(node_file)
            if nodeInfo then
                nodeInfo.category = string.gsub(nodeInfo.category, "\\", ".")
                nodeInfo.name = nodeInfo.name:sub(1, -5):gsub("-", ".")
                LDNodeEditor.addNodeTypeInfo(editor, nodeInfo)
            end
        end
    end

    ld.loc.addLocalizationDirectory("Opcodes_nodes/Languages")
end

---@type ScriptTable
local opcodesNodesData = {
    name = "OpcodesNodes",
    id = "opcodes_nodes",
    init = init
}

return opcodesNodesData