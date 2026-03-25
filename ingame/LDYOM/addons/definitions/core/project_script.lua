---@meta

-- ─── Project Script ───────────────────────────────────────────────────────────

---Descriptor table passed to `register_project_script`.
---Both fields are optional.
---@class ProjectScriptDescriptor
---@field on_load? fun() Called immediately after the script is loaded, when the project is opened in the editor.
---@field on_unload? fun() Called when the project is closed in the editor.

---Registers the local project script lifecycle callbacks.
---
---Call this once from `scripts/init.lua` in the project directory.
---The `on_load` callback is invoked immediately after registration.
---The `on_unload` callback is stored and invoked when the project is closed.
---
---Local modules placed in the same `scripts/` directory can be loaded with `require`:
---```lua
---local utils = require("utils")  -- resolves to scripts/utils.lua
---```
---
---**Example:**
---```lua
---local disconnect = nil
---
---register_project_script({
---    on_load = function()
---        log.info("Project opened!")
---        disconnect = events.on_scene_started(function(sceneId)
---            log.info("Scene started: " .. sceneId)
---        end)
---    end,
---    on_unload = function()
---        if disconnect then disconnect() end
---    end
---})
---```
---@param descriptor ProjectScriptDescriptor
function register_project_script(descriptor) end
