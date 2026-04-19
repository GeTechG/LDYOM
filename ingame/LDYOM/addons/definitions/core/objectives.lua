---@meta

-- ─── Objective Data structs ───────────────────────────────────────────────────

-- ── core.goto_scene ──────────────────────────────────────────────────────────

---Data struct for the `core.goto_scene` objective.
---@class ObjectiveGotoSceneData
---@field sceneId string UUID of the target scene.
---@field instantTransition boolean Skip fade-out/wait/fade-in when transitioning (default false).
local ObjectiveGotoSceneData = {}

-- ── core.goto_scene_if_var ───────────────────────────────────────────────────

---Data struct for the `core.goto_scene_if_var` objective.
---@class ObjectiveGotoSceneIfVarData
---@field sceneId string UUID of the target scene.
---@field varName string Name of the global variable to test.
---@field numberValue number Numeric value to compare against (used when the variable is a number).
---@field stringValue string String value to compare against (used when the variable is a string).
---@field boolValue boolean Boolean value to compare against (used when the variable is a boolean).
---@field instantTransition boolean Skip fade-out/wait/fade-in when transitioning (default false).
local ObjectiveGotoSceneIfVarData = {}
