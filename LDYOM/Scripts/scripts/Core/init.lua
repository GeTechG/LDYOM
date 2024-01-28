require("libs.table_utils")


local fa = require("libs.fa")
local bind = require("libs.bind")
local LDNodeEditor = require("ld.node_editor")
local drawNodeEditor = require("ld.draw")

local editor = LDNodeEditor.new()

local nodes_dirs = Dirs.entries("scripts/Core/nodes")
for _, node_file in ipairs(nodes_dirs) do
    if string.match(node_file, "%.lua$") then
        ---@type LDNodeEditorNodeType
        local node = require("nodes." .. node_file:sub(1, -5))
        LDNodeEditor.addNodeType(editor, node.typeName, node)
    end
end

ld.window.setMainMenuRender("node_editor_button", function()
    local scaleFont = ImGui.GetFontSize() / 16;
    if ImGui.Button(fa.ICON_FA_PROJECT_DIAGRAM .. " Node Editor (Lua)", ImVec2.new(scaleFont * 200, 0)) then
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