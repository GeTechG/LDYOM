---
change: add-mission-mode-objectives
updated: 2026-04-20T00:00:00Z
status: completed
---

## In-Flight
none — all 47 tasks complete. User confirmed in-game behaviour works ("все ок работает") after post-implementation iterations: STYLE_MIDDLE for fail/end banners, removal of failTune, variant fail/end text modes (M_FAIL / M_PASSD-plain / M_PASS-with-money / Custom), optional endTune (PLAY_MISSION_PASSED_TUNE(1)). Spec updated to match as-built behaviour and re-validated.

## Decisions
- [task 1.1] `CTheScripts::OnAMissionFlag` at `CTheScripts.h:220`. Reachable via the existing `#include <CTheScripts.h>`.
- [task 1.2] `Notifications::error(const std::string& title, const std::string& message)`. `fmt::format` reached transitively via `<localization.h>`.
- [task 1.3] `ProjectPlayer` Lua declaration file: `C:/Users/User/Documents/LDYOM/ingame/LDYOM/addons/definitions/core/project_player.lua`. Existing fields use **snake_case** (`is_playing`, `is_faded`, `current_scene_id`). The new field is `---@field is_mission boolean ...`.
- [task 1.4] Addon core `en.json` contains only node-graph strings. Objective display names live in `C:/Users/User/Documents/LDYOM/ingame/LDYOM/languages/en.json` under `objectives.core.<slug>`.
- [task 2.5/2.7] `MissionModeState` holds `active`, `failTextGxt`, `failAction` (no raw `failText`). Setter: `setMissionMode(std::string failTextGxt, MissionFailAction action)` — accepts the pre-encoded gxt string.
- [task 2.8] Both constants placed as `public static constexpr int` members on `ProjectPlayer` in the header.
- [task 2.4 / post-build fix] `MissionFailAction` variant serialization uses a `nlohmann::adl_serializer<MissionFailAction>` specialization (the initial free-function approach failed ADL when invoked from inside nlohmann internals). `EndProject` keeps its own free `to_json`/`from_json` in the `mission_fail_actions` namespace (serializes as `{}` since it has no fields — the `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT` macro requires ≥1 field, so it was replaced). `from_json` throws `std::runtime_error` on unknown type.
- [task 3.1] `TaskManager::addTask("mission_fail_sequence", missionFailSequence)` — pass the function, NOT the return value.
- [task 3.2] `missionFailSequence()` is declared in `project_player.h` as a **private static ktwait method**; accesses `instance().m_state.missionMode` directly.
- [task 3.2] `SET_PLAYER_CONTROL` invoked via `plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false/true)`. `CMessages::AddBigMessage` first arg uses `(char*)str.c_str()` cast — matches the existing cast at `project_player.cpp:54`.
- [task 3.3] `<CTheScripts.h>` was already included at `project_player.cpp:15`.
- [task 4.2] `SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, titleText, titleTime, fadeIn, failText)` — bare member names, mirrors `clock.h:34` / `cutscene.h:49`.
- [task 4.4] Nested-error lookup: `ScenesManager::instance().getUnsafeCurrentScene().info.name` + `ProjectPlayer::instance().getCurrentObjectiveIndex()`.
- [task 4.3] `tr()` results fed into `.c_str()` are stored in a named `const auto` local first — avoids dangling pointers from `std::string` temporaries.
- [task 4.4] Fade-IN pattern: `DO_FADE(MISSION_FADE_TIME_MS, 1); setFaded(false);` — NO `co_await` in between.
- [task 5.4] Fade-OUT pattern: `DO_FADE(MISSION_FADE_TIME_MS, 0); co_await (MISSION_FADE_TIME_MS + 100); setFaded(true);` — mirrors `cutscene.h:86-87`.
- [task 7.1] Lua binding uses `"is_mission"` (snake_case), matching existing `is_playing` / `is_faded` / `current_scene_id` convention. Task text said `"isMission"` — the snake_case form is correct.
- [task 7.3] Objective data types live in a single `objectives.lua` file in `addons/definitions/core/`; fields are camelCase mirroring `SOL_LUA_FOR_EACH` C++ member names (same as `ObjectiveGotoSceneData`).
- [task 8.1/8.2] All four task-block contents were placed in `C:/Users/User/Documents/LDYOM/ingame/LDYOM/languages/en.json` — the task text incorrectly routed 8.1/8.2 to the addon file. `nested_error.body` uses two bare `{}` placeholders for `fmt::format(fmt::runtime(...), sceneName, objectiveIndex)`.
- [task 9.1] Debug build succeeded after two fixes: (1) replaced empty-field `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(EndProject)` with manual `to_json`/`from_json` free functions in `mission_fail_actions`; (2) moved `MissionFailAction` serialization into a `nlohmann::adl_serializer<MissionFailAction>` specialization.

## Gotchas
- Pre-existing clangd LSP cannot resolve the project's include paths (Conan deps, relative project includes). Every iteration shows cascading "file not found" / "undeclared identifier" diagnostics across stdlib, sol, nlohmann, CMessages, TheCamera, etc. These are NOT real compile errors — MSVC via CMake resolves everything. Real validation is the Debug build.
- The task text routes `objectives.core.mission_*.name` to the wrong file (addon `en.json`); the main `ingame/LDYOM/languages/en.json` is correct — all `objectives.core.*` lives there.
- Lua binding strings use snake_case even when task text says camelCase. Always verify against `project_player_binding.cpp`'s existing strings before accepting the task text verbatim.
- `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT` REQUIRES ≥1 field argument. For empty tag structs, write manual `to_json`/`from_json` free functions.
- `std::variant` needs `nlohmann::adl_serializer` specialization — free `to_json`/`from_json` in global or arbitrary namespace won't be found via ADL from inside nlohmann's templates.
- Lua definitions path is `C:/Users/User/Documents/LDYOM/ingame/LDYOM/addons/definitions/core/` (note the embedded `LDYOM` under `ingame`), not `ingame/definitions/core/`.

## Open Questions
(none — all implementation decisions made)

## Next Steps
1. 9.2 — Project `mission_start` → `timelimit` expiration → confirm fail banner bottom-right, controls blocked, project stops after banner.
2. 9.3 — `mission_start` → `mission_end` normal completion → confirm `OnAMissionFlag == 0` at end.
3. 9.4 — `mission_start` without `mission_end`, press LDSTOP mid-mission → confirm `OnAMissionFlag == 0` after stop.
4. 9.5 — Two `mission_start` in sequence → `Notifications::error` toast + `LDYOM_ERROR` log + project stops.
5. 9.6 — Standalone `mission_end` (no preceding start) → runs as no-op.
6. 9.7 — Required-actor-death fail during mission mode → author's fail text shown.
7. 9.8 — `projectPlayer.is_mission` returns `true` only between mission_start and mission_end.
8. 9.9 — JSON round-trip of a project with `mission_start` + `mission_end` — reload and inspect `failAction.type == "end_project"`.
9. 9.10 — Regression: default scene-transition fade still works across scene changes.
10. 9.11 — Re-entrancy: two fail signals in the same 1-second window → exactly ONE banner, clean stop, no leftover tasks.
11. 9.12 — Orphan-task cleanup: fail → LDSTOP during banner → banner disappears, `OnAMissionFlag == 0`, no `mission_fail_sequence` log chatter afterward.
