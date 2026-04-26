## 1. Verify prerequisites

- [x] 1.1 Confirm `core.goto_scene` (`src/data/objectives/goto_scene.h`) is the canonical model for runtime scene transition — verify it calls `ProjectPlayer::requestSceneTransition`.
- [x] 1.2 Confirm `pendingObjectiveJump` is consumed only at `project_player.cpp:145` (post-objective). A second consumption point at scene-loop start is the change.
- [x] 1.3 Confirm `stopping_project_player` task at `project_player.cpp:258-263` is the single project-stop choke point. Adding a fade-in step there covers all stop paths.
- [x] 1.4 Grep `../ingame/languages/en.json` for an existing `fade_out` key under `objectives.core.mission_end` — if present, reuse it for `mission_start`'s checkbox; otherwise add under `objectives.core.mission_start`.
- [x] 1.5 Confirm sol2 supports binding `std::optional<std::string>` as a usertype field directly (sol2 docs / project's existing optional bindings, e.g. any `std::optional` in current usertypes). If not supported directly, plan for a custom getter/setter that maps `sol::nil` ↔ `std::nullopt`.
- [x] 1.6 Verify `nlohmann::json` `from_json` for `std::optional<std::string>` in the project's existing serialization helpers — if not already wired, register an `adl_serializer<std::optional<std::string>>` specialization (or use `j.value("objectiveId", std::optional<std::string>{})` pattern).

## 2. Extend variant alternatives in `src/core/project_player.h`

- [x] 2.1 Add `fadeOut = true` field to `mission_fail_actions::EndProject`. Update its `to_json` / `from_json` to round-trip the field (`{ "fadeOut": true|false }` inside the `data` object).
- [x] 2.2 Add `struct RestartScene { bool fadeOut = true; static constexpr const char* TYPE = "restart_scene"; };` with matching `to_json` / `from_json` free functions.
- [x] 2.3 Add `struct GotoScene { std::string sceneId; std::optional<std::string> objectiveId; bool fadeOut = true; static constexpr const char* TYPE = "goto_scene"; };` with matching `to_json` / `from_json`. The serializer for `objectiveId` MUST emit nothing (or an explicit null) when `nullopt`, and accept missing/null on read.
- [x] 2.4 Widen `using MissionFailAction = std::variant<mission_fail_actions::EndProject, mission_fail_actions::RestartScene, mission_fail_actions::GotoScene>;`.
- [x] 2.5 Update `nlohmann::adl_serializer<MissionFailAction>::from_json` to dispatch on three type tags. Keep the unknown-type `runtime_error` fallthrough verbatim.
- [x] 2.6 Add `#include <optional>` if not already present in `project_player.h`.
- [x] 2.7 Declare `static ktwait runScenesLoop();` as a private static method on `ProjectPlayer`.
- [x] 2.8 Verify `setMissionMode(int, std::string, MissionFailAction)` and `clearMissionMode()` accessors do not need signature changes — they work over the variant unchanged.

## 3. Refactor `run()` → `runScenesLoop()` in `src/core/project_player.cpp`

- [x] 3.1 Move the body of `run()` from the `bool continueRunning = true;` line through the trailing `LDYOM_INFO("Project player finished");` into a new `ktwait ProjectPlayer::runScenesLoop()` method. No logic changes — purely structural.
- [x] 3.2 In `run()`, after the existing startup block (fade-in, isPlaying, transitionPlayingState(true), fade-out, isFaded=true), call `co_await runScenesLoop();`. Remove the duplicated cleanup tail from `run()` (it now lives in `runScenesLoop`).
- [x] 3.3 In `runScenesLoop()`, AT THE TOP of the scene-loop body (just after `auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;`, before the for-loop), add the new `pendingObjectiveJump` consumption point: read the optional, find the matching objective index in the current scene's `objectives` vector, set `int startIndex = matchIndex` (or 0 on miss), and start the for-loop at `startIndex` instead of `0`.
- [x] 3.4 Verify the existing post-objective consumption point at `project_player.cpp:145` is preserved verbatim. Both points read the same `optional` and clear it on read; at most one can fire per scene-load.
- [ ] 3.5 Build Debug and run a free-roam project (no mission_start) to verify the extraction is behaviour-neutral. The test passes if normal objective execution, scene transitions via `core.goto_scene`, and project end-of-loop cleanup all work identically to pre-refactor.

## 4. Rework `missionFailSequence()` in `src/core/project_player.cpp`

- [x] 4.1 After the snapshot block (`localFailTextMode`, `localFailTextGxt`, `localFailAction`, `missionMode.active = false`), keep the existing banner block (PRINT_BIG / AddBigMessage based on `localFailTextMode`) and `co_await MISSION_FAIL_TEXT_TIME_MS`.
- [x] 4.2 After the banner await, BEFORE `SET_PLAYER_CONTROL=true`, add the fade-out block: extract `bool fadeOut = std::visit([](const auto& a){ return a.fadeOut; }, localFailAction);`. If `fadeOut && !isFaded()`, run `DO_FADE(MISSION_FADE_TIME_MS, 0)`, `co_await (MISSION_FADE_TIME_MS + 100)`, `setFaded(true)`.
- [x] 4.3 Then `SET_PLAYER_CONTROL=true`.
- [x] 4.4 Update the existing `std::visit` to dispatch three arms:
  - `EndProject` → `instance().stopCurrentProject();` (unchanged).
  - `RestartScene` → call `navigationalArmContinue(instance().m_state.currentSceneId, std::nullopt);` (helper defined in 4.5).
  - `GotoScene` → call `navigationalArmContinue(action.sceneId, action.objectiveId);`.
- [x] 4.5 Implement a private static helper `void navigationalArmContinue(const std::string& sceneId, const std::optional<std::string>& objectiveId)`:
  - Early-return if `!isPlaying()` (guards against LDSTOP-during-fade-out edge case).
  - If `sceneId` is not in `ScenesManager::instance().getScenesInfo()`, log via `LDYOM_ERROR("navigational fail action references unknown scene id '{}'", sceneId)`, then call `stopCurrentProject()` and return.
  - `CTheScripts::OnAMissionFlag = 0;`
  - `instance().clearMissionMode();`
  - `Application::instance().luaTaskManager().cancelAll();`
  - `instance().projectTasklist->clear_all_tasks();`
  - `ScenesManager::instance().resetCurrentScene();`
  - `ScenesManager::instance().loadScene(sceneId);`
  - `instance().m_state.currentSceneId = sceneId;`
  - If `objectiveId`, call `instance().requestObjectiveJump(*objectiveId);`.
  - `TaskManager::instance().addTask("run_project_player", runScenesLoop);`
- [x] 4.6 Verify the safety-net `removeTask("mission_fail_sequence")` in `transitionPlayingState(false)` still fires correctly. No change needed here; it sits between the visit arm and any user-stop.

## 5. Stop-path fade-in in `src/core/project_player.cpp`

- [x] 5.1 In the `stopping_project_player` task (around `project_player.cpp:258-263`), add a fade-in block AFTER `co_await playerLeaveAnyVehicle()` and BEFORE `m_state.isPlaying = false;`: if `instance().m_state.isFaded`, `DO_FADE(MISSION_FADE_TIME_MS, 1)`, `co_await MISSION_FADE_TIME_MS`, `m_state.isFaded = false`.
- [x] 5.2 Verify there is no double-fade with the `EndProject` arm: the fail sequence sets `isFaded = true`, then `stopCurrentProject()` schedules `stopping_project_player`, which sees `isFaded == true` and fades back in. Single fade-out, single fade-in.
- [x] 5.3 Verify the user-stop path (UI button or LDSTOP cheat mid-cutscene-fadeout) also benefits — currently leaves a faded screen, now restores it.

## 6. Editor panel updates in `src/data/objectives/mission_start.h`

- [x] 6.1 In `renderEditor(Data&)`, the existing fail-action combo: extend the entries from `["End project"]` to `["End project", "Restart scene", "Go to scene"]`. Use the localization keys: `objectives.core.mission_start.fail_action_end_project` (existing), `..._restart_scene` (new), `..._goto_scene` (new).
- [x] 6.2 On combo selection change, replace `data.failAction` with a default-constructed instance of the new alternative (`= mission_fail_actions::EndProject{};` etc.). Each switch resets the alternative's payload, including `fadeOut`.
- [x] 6.3 Below the combo, render the active alternative's payload conditionally:
  - `EndProject` — no payload rows.
  - `RestartScene` — no payload rows.
  - `GotoScene` — render a scene combo (mirroring `goto_scene.h:31-42`) bound to `action.sceneId`, then an objective combo bound to `action.objectiveId`. The objective combo's first entry is a "first objective" sentinel that maps `objectiveId` to `std::nullopt`; remaining entries are the destination scene's objectives by name+index. If `data.failAction`'s `GotoScene::sceneId` is empty, the objective combo is disabled.
- [x] 6.4 After the conditional payload section, render a `Fade out` checkbox bound via `std::visit` to the active alternative's `fadeOut` field. Tooltip explains that the screen fades to black after the banner so the next action (stop, restart, transition) is hidden.
- [x] 6.5 Use `std::visit` with `if constexpr` for the conditional rendering and the `fadeOut` checkbox binding — same pattern as the dispatch in `missionFailSequence`.

## 7. Lua bindings in `src/lua/project_player_binding.cpp` and `mission_start.h`

- [x] 7.1 In `mission_start.h`'s `sol_lua_register`, register three usertypes: `MissionFailEndProject`, `MissionFailRestartScene`, `MissionFailGotoScene`. Each exposes its fields by name (`fadeOut` on all; `sceneId` and `objectiveId` on `GotoScene`). All three include `"new"` constructors via sol2's `sol::factories`.
- [x] 7.2 Bind `mission_start::Data::failAction` as a sol2 property:
  - Getter: `[](const Data& d) -> sol::object { return std::visit([](const auto& a) { return sol::make_object(lua, a); }, d.failAction); }` — returns the active alternative as its usertype instance.
  - Setter: `[](Data& d, sol::object value) { if (value.is<EndProject>()) d.failAction = value.as<EndProject>(); else if (value.is<RestartScene>()) ...; else throw sol::error("...mission failAction expects MissionFail{EndProject,RestartScene,GotoScene}"); }`.
- [x] 7.3 Update the `SOL_LUA_FOR_EACH` call on `mission_start::Data` to EXCLUDE `failAction` (the property binds it manually). Verify the v1 docstring comment about the `failAction` Lua omission can now be removed or rewritten to describe the new property.
- [x] 7.4 No changes needed to `project_player_binding.cpp::is_mission` — it remains a read-only property on `ProjectPlayer`.

## 8. Lua declaration files

- [x] 8.1 In `../ingame/LDYOM/addons/definitions/core/objectives.lua`, add three classes: `MissionFailEndProject` with `fadeOut: boolean`; `MissionFailRestartScene` with `fadeOut: boolean`; `MissionFailGotoScene` with `sceneId: string`, `objectiveId: string?`, `fadeOut: boolean`.
- [x] 8.2 Update `ObjectiveMissionStartData.failAction` to the union alias `MissionFailEndProject | MissionFailRestartScene | MissionFailGotoScene` (or `---@alias MissionFailAction` with the three types). Remove the v1 "Lua omission" comment.

## 9. Localization

- [x] 9.1 In `../ingame/languages/en.json` under `objectives.core.mission_start`, add: `fail_action_restart_scene` ("Restart scene"), `fail_action_goto_scene` ("Go to scene"), `fail_action_goto_scene_target` ("Target scene"), `fail_action_goto_scene_objective` ("Target objective"), `fail_action_first_objective` ("⟨first objective⟩"), `fade_out` ("Fade out"), `fade_out_tooltip` ("Fade to black after the banner so the next action is hidden.").
- [x] 9.2 If `mission_end` already has a `fade_out` key, prefer reuse — confirm via grep and adjust 9.1 if so.

## 10. Verification

- [x] 10.1 Build Debug.
- [x] 10.2 Free-roam project (no `mission_start`): play through, verify scene transitions and project end behave identically to v1 (regression — the `runScenesLoop` extraction must be transparent).
- [x] 10.3 Mission with `EndProject{ fadeOut = true }` (default): trigger fail (timelimit expiry); confirm banner → screen fades to black → project stops → screen fades back in. No flash of free-roam under bright.
- [x] 10.4 Mission with `EndProject{ fadeOut = false }`: trigger fail; confirm banner → project stops with no fade. Validates the toggle's negative path.
- [x] 10.5 Mission with `RestartScene{ fadeOut = true }`: trigger fail; confirm banner → fade to black → same scene reloads → destination's `mission_start` re-arms (and re-fades-in if its `fadeIn` is set). Mission can be retried.
- [x] 10.6 Mission with `GotoScene{ sceneId = X, objectiveId = nil }`: trigger fail; confirm banner → fade to black → scene X loads from objective 0 → if scene X has a `mission_start` it re-arms.
- [x] 10.7 Mission with `GotoScene{ sceneId = X, objectiveId = Y }`: trigger fail; confirm banner → fade to black → scene X loads → first objective executed is Y (verify by adding a logging objective at Y and a different one at index 0).
- [x] 10.8 Re-entrancy: mission with two near-simultaneous fail signals (required actor death + required vehicle destruction in the same tick). Confirm exactly ONE banner displays and ONE action dispatches.
- [x] 10.9 Mid-banner LDSTOP: trigger fail in active mission; press LDSTOP during the 1-second banner. Confirm banner cancels, fade-out (if any) is interrupted, project stops cleanly, `OnAMissionFlag = 0`, no leftover `mission_fail_sequence` task. Verify `navigationalArmContinue`'s `isPlaying` early-return covers the case where LDSTOP fires between fade-out and the visit arm.
- [x] 10.10 JSON round-trip: project with all three fail-action types saved and reloaded. Inspect on-disk JSON for `"type": "end_project"`, `"restart_scene"`, `"goto_scene"`; confirm fields restore identically.
- [x] 10.11 Legacy JSON: a project authored under v1 (only `EndProject` available, no `fadeOut` field on disk) loads with `EndProject{ fadeOut = true }` applied as the default. Verify by loading a v1-saved project and inspecting via the editor.
- [x] 10.12 Unknown fail-action type: hand-edit a project JSON to inject `"type": "respawn_player"`. Confirm load throws an error whose message names the unknown type string (preserves v1 behavior).
- [x] 10.13 Lua read: from a Lua script during active mission mode, fetch `data.failAction` (where `data` comes from a known `mission_start` objective), verify the returned object is the correct alternative usertype, and field accesses (`fadeOut`, `sceneId`, `objectiveId`) return expected values.
- [x] 10.14 Lua write: from Lua, construct `MissionFailGotoScene.new()`, assign fields, then assign to `data.failAction`. Re-read: confirm the assignment landed and round-trips through JSON unchanged.
- [x] 10.15 Lua wrong-type write: from Lua, attempt `data.failAction = {}` (plain table) — confirm a sol2 error is raised with the expected message naming the three accepted types.
- [x] 10.16 GotoScene with stale `sceneId` (scene deleted post-authoring): confirm the navigational arm logs `LDYOM_ERROR` and falls through to `stopCurrentProject()` rather than crashing.
- [x] 10.17 Stop-path fade-in regression: end a project mid-cutscene-fadeout (e.g. press LDSTOP while a `core.cutscene` has just faded out and is waiting). Confirm the screen fades back in before free-roam returns.
