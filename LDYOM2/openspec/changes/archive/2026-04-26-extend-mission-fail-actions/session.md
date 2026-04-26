---
change: extend-mission-fail-actions
updated: 2026-04-26T10:05:00Z
status: paused
---

## In-Flight
none

## Decisions
- [task 1.4] `objectives.core.mission_end.fade_out` exists; sibling JSON keys can't share. Added separate `objectives.core.mission_start.fade_out` (same text "Fade out") + new `..._tooltip` key.
- [task 2.3] `GotoScene::objectiveId` JSON: `to_json` omits the key when `nullopt`; `from_json` checks `contains(...) && !is_null(...)`.
- [task 3.3] Objective UUID accessor is `uuids::to_string(obj.id)`. Used `LDYOM_WARN` for jump-target-not-found.
- [task 4.5] `navigationalArmContinue` defined at `project_player.cpp:339`; declared private static in `project_player.h`. Membership check: `std::find_if` over `getScenesInfo()` comparing `si.id == sceneId`. Registers via `TaskManager::instance().addTask("run_project_player", runScenesLoop)` (NOT `run`, to avoid re-running startup fade-in).
- [task 5.1] Fade-in idiom: `plugin::Command<plugin::Commands::DO_FADE>(MISSION_FADE_TIME_MS, 1)` + `co_await (MISSION_FADE_TIME_MS + 100)`.
- [task 6.3] Editor uses `tr(...)` (i18ncpp). Scene combo mirrors `goto_scene.h:31-42`. Objective combo: index 0 = "first objective" sentinel ↔ `nullopt`. `ImGui::BeginDisabled(action.sceneId.empty())` wraps the objective combo. Non-current scenes loaded from disk to populate the objective list.
- [task 7.1] `MissionFailGotoScene::objectiveId` exposed as `sol::property` with `sol::this_state`-captured getter/setter mapping `sol::nil ↔ std::nullopt`.
- [task 7.2] `Data::failAction` getter returns a COPY via `sol::make_object`. Setter type-tag dispatches via `value.is<T>()`; throws `sol::error` on unknown type. Lua workflow: `local g = data.failAction; g.foo = ...; data.failAction = g`.
- [task 8.1] Lua decl uses module-level globals + table-with-`new` constructor pattern. `---@alias MissionFailAction` declared before `ObjectiveMissionStartData`.
- [post-build cleanup] Added missing `fail_action_end_project` localization key (referenced by editor at `mission_start.h:149` but absent from en.json — would have shown raw i18n path at runtime).
- [verified] Re-entrancy guard preserved at `project_player.cpp:303` (`missionMode.active = false` before any `co_await` in `missionFailSequence`); `failCurrentProject` early-return at line 290 unchanged.

## Gotchas
- Legacy `objectives.core.mission_start.fail_actions` array (single entry: "End project") is now stale — editor uses individual `fail_action_<name>` keys instead. Not touched in this change. Candidate for follow-up cleanup.
- `ru.json` was NOT updated. Translator/maintainer needs to add the same 8 new keys (`fail_action_end_project` + 7 from section 9) when ready.
- All implementation built clean with MSVC Debug. Clangd diagnostics throughout this session were spurious.

## Open Questions

## Next Steps (user playtest pending — implementation complete)
1. **Section 3.5 + 10.2** (combined): Free-roam regression — load a project with no `mission_start` and play through; scene transitions and project end should be byte-identical to v1.
2. **10.3** Mission with `EndProject{ fadeOut = true }`.
3. **10.4** Mission with `EndProject{ fadeOut = false }`.
4. **10.5** Mission with `RestartScene{ fadeOut = true }`.
5. **10.6** Mission with `GotoScene{ sceneId = X, objectiveId = nil }`.
6. **10.7** Mission with `GotoScene{ sceneId = X, objectiveId = Y }`.
7. **10.8** Re-entrancy: two near-simultaneous fail signals → ONE banner.
8. **10.9** Mid-banner LDSTOP cleanup.
9. **10.10** JSON round-trip (save + reload all three types).
10. **10.11** Legacy v1 JSON load (no `fadeOut` field → `EndProject{ fadeOut = true }`).
11. **10.12** Unknown fail-action type → `runtime_error` on load.
12. **10.13** Lua read of `data.failAction`.
13. **10.14** Lua write via `MissionFailGotoScene.new()` + assign-back.
14. **10.15** Lua wrong-type write → sol2 error.
15. **10.16** `GotoScene` with stale `sceneId` → `LDYOM_ERROR` + `stopCurrentProject`.
16. **10.17** Stop-path fade-in regression (LDSTOP mid-cutscene-fadeout).
