
---@class CoreScriptTable : ScriptTable
---@field editor LDNodeEditor

---@param scriptData CoreScriptTable | table
function init(scriptData)
    require("libs.table_utils")

    local fa = require("libs.fa")
    local bind = require("libs.bind")
    local LDNodeEditor = require("ld.node_editor")
    local drawNodeEditor = require("ld.draw")

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

    local editor = LDNodeEditor.new()

    local tasks = {}

    local nodes_dirs = Dirs.entries("LDYOM/Scripts/scripts/Core/nodes", true)
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

    ld.events.onStartProject[#ld.events.onStartProject+1] = function()
        for _, var in pairs(editor.variables) do
            var.value = var.defaultValue
        end
    end

    ld.events.onStartProject[#ld.events.onStartProject+1] = function()
        for _, context in pairs(editor.contexts) do
            for _, node in pairs(context.nodes) do
                if node.nodeType == "core.on_start_project" then
                    LDNodeEditor.runNode(editor, context, node)
                end
            end
        end
    end

    ld.events.onStartScene[#ld.events.onStartScene+1] = function(scene, ktcoro_tasklist)
        for _, context in pairs(editor.contexts) do
            for _, node in pairs(context.nodes) do
                ---@cast node LDNodeEditorOnStartSceneNode
                if node.nodeType == "core.on_start_scene" and scene:getId() == node.sceneId then
                    tasks.on_start_scene = function ()
                        LDNodeEditor.runNode(editor, context, node)
                    end
                    ktcoro_tasklist:add_task(tasks.on_start_scene)
                end
            end
        end
    end

    ld.events.mainLoop[#ld.events.mainLoop+1] = function(ktcoro_tasklist)
        for _, context in pairs(editor.contexts) do
            for _, node in pairs(context.nodes) do
                ---@cast node LDNodeEditorOnStartSceneNode
                if node.nodeType == "core.main_loop" then
                    tasks.main_loop = function ()
                        LDNodeEditor.runNode(editor, context, node)
                    end
                    ktcoro_tasklist:add_task(tasks.main_loop)
                end
            end
        end
    end

    ld.events.onStartObjective[#ld.events.onStartObjective+1] = function(objective, ktcoro_tasklist)
        for _, context in pairs(editor.contexts) do
            for _, node in pairs(context.nodes) do
                ---@cast node LDNodeEditorOnStartObjectiveNode
                if node.nodeType == "core.on_start_objective" and tostring(objective:getUuid()) == node.objectiveUuid then
                    tasks.on_start_objective = function ()
                        LDNodeEditor.runNode(editor, context, node)
                    end
                    ktcoro_tasklist:add_task(tasks.on_start_objective)
                end
            end
        end
    end

    ld.events.onSaveProject[#ld.events.onSaveProject+1] = function(projectDirectory)
        local nodesPath = projectDirectory .. "/nodes";
        Dirs.create(nodesPath);
        for _, context in ipairs(editor.contexts) do
            local saveData = {}
            saveData.nodes = context.nodes
            saveData.links = context.__links
            saveData.__nextUniqueId = context.__nextUniqueId
            local file = io.open(nodesPath .. "/" .. context.name .. ".json", "w")
            if file then
                file:write(json.encode(saveData, {indent = false}) --[[@as string]])
                file:close()
            end
        end
        --- editor data
        local saveData = {}
        saveData.variables = editor.variables
        local file = io.open(projectDirectory .. "/nodeEditor.json", "w")
        if file then
            file:write(json.encode(saveData, {indent = false}) --[[@as string]])
            file:close()
        end
    end

    ld.events.onLoadProject[#ld.events.onLoadProject+1] = function(projectDirectory)
        editor.contexts = {};
        editor.currentIndexContext = 1;

        local nodesPath = projectDirectory .. "/nodes";
        local contexts = Dirs.entries(nodesPath, false)

        for _, contextName in ipairs(contexts) do
            local file = io.open(nodesPath .. "/" .. contextName, "r")
            if file then
                local data = file:read("*all")
                file:close()
                local loadData = json.decode(data)
                if loadData then
                    LDNodeEditor.addNewContext(editor)
                    local context = editor.contexts[#editor.contexts]
                    context.name = contextName:sub(1, -6)
                    context.nodes = loadData.nodes
                    context.__links = loadData.links
                    context.__nextUniqueId = loadData.__nextUniqueId
                end
            end
        end
        --- editor data
        local file = io.open(projectDirectory .. "/nodeEditor.json", "r")
        if file then
            local data = file:read("*all")
            file:close()
            local loadData = json.decode(data)
            if loadData then
                editor.variables = loadData.variables
            end
        end
    end

    ld.window.setMainMenuRender("node_editor_button", function()
        local scaleFont = ImGui.GetFontSize() / 16;
        if ImGui.Button(fa.ICON_FA_PROJECT_DIAGRAM .. " " .. ld.loc.get("nodes.node_editor.title"), ImVec2.new(scaleFont * 200, 0)) then
            ld.window.replaceWindow(function()
                ld.window.closeMainMenu();
                ld.window.openLuaWindow(bind(drawNodeEditor, editor));
            end, function()
                ld.window.closeLuaWindow();
                ld.window.openMainMenu();
            end)
        end
    end)

    ld.loc.addLocalizationDirectory("Core/Languages")

    scriptData.editor = editor
    scriptData.tasks = tasks
end

---@type ScriptTable
local scriptData = {
    name = "Core",
    id = "core",
    init = init,
}

return scriptData