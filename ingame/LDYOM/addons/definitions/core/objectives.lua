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

-- ── core.mission_start ───────────────────────────────────────────────────────

---Data struct for the `core.mission_start` objective.
---@class ObjectiveMissionStartData
---@field titleText string Text to display as the mission title banner.
---@field titleTime number Duration (seconds) to show the title banner.
---@field fadeIn boolean Fade in from black when the mission starts.
---@field failTextMode integer Fail text source: 0 = Default (built-in GXT `M_FAIL`), 1 = Custom (uses `failText`).
---@field failText string Custom fail banner text (only read when `failTextMode == 1`).
--- Note: `failAction` is intentionally not exposed in v1 — sol2 requires per-variant-alternative
--- usertype registration plus a custom from_lua path. With only one alternative (EndProject)
--- the work is not justified. See design.md "Lua visibility".
local ObjectiveMissionStartData = {}

-- ── core.mission_end ─────────────────────────────────────────────────────────

---Data struct for the `core.mission_end` objective.
---@class ObjectiveMissionEndData
---@field endTextMode integer End banner source: 0 = Default (GXT `M_PASSD`), 1 = With money (GXT `M_PASS` + `endMoney`), 2 = Custom (`endText`).
---@field endText string Custom end banner text (only read when `endTextMode == 2`).
---@field endMoney integer Money amount shown in the banner when `endTextMode == 1`. Display only — does NOT grant money; chain a `core.money_add` objective if needed.
---@field endTime number Duration (seconds) to show the end banner.
---@field fadeOut boolean Fade out to black when the mission ends.
---@field endTune boolean Play the classic mission-passed tune (opcode 0394, same as DYOM's mission-passed path) together with the end banner.
local ObjectiveMissionEndData = {}
