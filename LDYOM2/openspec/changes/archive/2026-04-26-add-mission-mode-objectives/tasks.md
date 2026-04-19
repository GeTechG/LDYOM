## 1. Verify prerequisites

- [x] 1.1 Confirm `CTheScripts::OnAMissionFlag` is reachable with the existing `<CTheScripts.h>` include already in use by `cutscene.h:9` (no new plugin-sdk dependency).
- [x] 1.2 Confirm `Notifications::error(title, message)` signature in `src/core/notifications.h` and verify `fmt::format` is available in the existing include surface for the `mission_start` translation unit.
- [x] 1.3 Inspect `../ingame/definitions/core/` to find where `ProjectPlayer.isPlaying` is declared (grep for `isPlaying`) so `isMission` can land beside it following the same convention.
- [x] 1.4 Grep the addon localization file `../ingame/addons/core/languages/en.json` under `objectives.core.*.name` to confirm the display-name key convention for the two new objectives.

## 2. Runtime state and types in `src/core/project_player.h`

- [x] 2.1 Add `#include <variant>` and `#include <string>` (if missing) at the top of `project_player.h`.
- [x] 2.2 Add the `mission_fail_actions` namespace with `struct EndProject { static constexpr const char* TYPE = "end_project"; };` and an `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(EndProject)` for empty-field serialization.
- [x] 2.3 Define `using MissionFailAction = std::variant<mission_fail_actions::EndProject>;`.
- [x] 2.4 Add free `to_json` / `from_json` for `MissionFailAction` implementing the tagged envelope `{ "type": "<TYPE>", "data": { ... } }`. `from_json` MUST throw a descriptive error on unknown `type` values.
- [x] 2.5 Add `struct MissionModeState { bool active = false; std::string failText; std::string failTextGxt; MissionFailAction failAction = mission_fail_actions::EndProject{}; };` above `ProjectPlayerState`.
- [x] 2.6 Add `MissionModeState missionMode;` as a field on `ProjectPlayerState`.
- [x] 2.7 Add public accessor `bool isMissionActive() const { return m_state.missionMode.active; }` and a private-friendly accessor for the objective headers that need to write the state (prefer a `setMissionMode(std::string failText, MissionFailAction action)` method and a `clearMissionMode()` method over exposing the raw struct).
- [x] 2.8 Add `static constexpr int MISSION_FAIL_TEXT_TIME_MS = 1000;` and `static constexpr int MISSION_FADE_TIME_MS = 500;` as public constants on `ProjectPlayer` (or at file scope inside an anonymous namespace in the .cpp if preferred — match whatever convention the existing fade/timer literals follow).

## 3. Smart `failCurrentProject()` and safety net in `src/core/project_player.cpp`

- [x] 3.1 Replace the body of `ProjectPlayer::failCurrentProject()` with: early-return `stopCurrentProject()` when `!m_state.missionMode.active`; otherwise `TaskManager::removeTask("run_project_player")` followed by `TaskManager::addTask("mission_fail_sequence", ...)` spawning the fail coroutine.
- [x] 3.2 Implement the `mission_fail_sequence` coroutine with the re-entrancy guard: the coroutine MUST, as its first statements before any `co_await`, copy `missionMode.failTextGxt` and `missionMode.failAction` into local variables and set `missionMode.active = false`. Then: `SET_PLAYER_CONTROL(0, false)` → `CMessages::AddBigMessage(localFailTextGxt.data(), MISSION_FAIL_TEXT_TIME_MS, STYLE_BOTTOM_RIGHT)` → `co_await MISSION_FAIL_TEXT_TIME_MS` → `SET_PLAYER_CONTROL(0, true)` → `std::visit` over the local `failAction` copy to execute the chosen action (today: `EndProject` → `stopCurrentProject()`). The guard ensures a second fail signal during the 1s banner falls through to the bare `stopCurrentProject()` path and does not spawn a duplicate task.
- [x] 3.3 In `transitionPlayingState(false)` (around `project_player.cpp:325`, after the existing `isFaded = false;` reset), add three lines in order: `TaskManager::instance().removeTask("mission_fail_sequence");` (prevents an orphan fail coroutine outliving its parent run task after an LDSTOP mid-banner), `CTheScripts::OnAMissionFlag = 0;`, `instance().m_state.missionMode = {};`.
- [x] 3.4 Verify that no other code path writes to `missionMode` outside `setMissionMode` / `clearMissionMode` / the fail coroutine's `active = false` flip / the safety net.

## 4. Objective `core.mission_start` (`src/data/objectives/mission_start.h`)

- [x] 4.1 Create header under `namespace objectives::mission_start` with `constexpr const char* TYPE = "core.mission_start";`.
- [x] 4.2 Define `struct Data { std::string titleText; float titleTime = 1.f; bool fadeIn = false; std::string failText; MissionFailAction failAction = mission_fail_actions::EndProject{}; std::string gameTitleText; /* cached gxt */ };` with `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT` over the persisted fields (exclude `gameTitleText` — it's derived). `SOL_LUA_FOR_EACH` exposes `titleText`, `titleTime`, `fadeIn`, `failText` — but deliberately NOT `failAction`. sol2 requires per-variant-alternative usertype registration plus a custom `from_lua` path to round-trip a Lua-constructed variant back into the C++ field; with only one variant alternative in v1 the work is not justified. Document this gap next to the `SOL_LUA_FOR_EACH` call with a short comment pointing to `design.md`.
- [x] 4.3 Implement `renderEditor(Data&)` with ImGui rows for: title text (string input), title time (DragFloat, 0–10s), fade-in (checkbox), fail text (string input), fail action (Combo — today only "End project"; the combo is scaffolding for future actions).
- [x] 4.4 Implement `execute(Data&)` coroutine:
  - If `ProjectPlayer::instance().isMissionActive()` → emit `Notifications::error` naming the scene name and current objective index, `LDYOM_ERROR` with the same data, call `stopCurrentProject()`, `co_return`.
  - Cache `titleText` → `gameTitleText` via `utf8_to_cp1251 + gxt_encode`.
  - Cache `failText` into a cp1251+gxt string and pass it into `ProjectPlayer::setMissionMode(cachedFailGxt, failAction)`.
  - Write `CTheScripts::OnAMissionFlag = 1`.
  - If `fadeIn` and `ProjectPlayer::instance().isFaded()` → `DO_FADE(MISSION_FADE_TIME_MS, 1)`, `setFaded(false)` (cooperate with the existing `isFaded` invariant).
  - `CMessages::AddBigMessage(gameTitleText.data(), static_cast<int>(titleTime * 1000), STYLE_BOTTOM_RIGHT)`.
- [x] 4.5 Implement `make()` (default Data) and `builder()` returning the registration struct, matching the `clock.h:77-82` / `cutscene.h:282-302` pattern.

## 5. Objective `core.mission_end` (`src/data/objectives/mission_end.h`)

- [x] 5.1 Create header under `namespace objectives::mission_end` with `constexpr const char* TYPE = "core.mission_end";`.
- [x] 5.2 Define `struct Data { std::string endText; float endTime = 1.f; bool fadeOut = false; std::string gameEndText; };` with `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT` and `SOL_LUA_FOR_EACH` (exclude `gameEndText`).
- [x] 5.3 Implement `renderEditor(Data&)` mirroring the `mission_start` layout minus the fail fields.
- [x] 5.4 Implement `execute(Data&)` coroutine:
  - If `!ProjectPlayer::instance().isMissionActive()` → `co_return` immediately (orphan-end no-op).
  - Cache `endText` → `gameEndText`.
  - If `fadeOut` and `!ProjectPlayer::instance().isFaded()` → `DO_FADE(MISSION_FADE_TIME_MS, 0)`, wait `MISSION_FADE_TIME_MS + 100ms` (match the cutscene precedent at `cutscene.h:86-87`), `setFaded(true)`.
  - `CMessages::AddBigMessage(gameEndText.data(), static_cast<int>(endTime * 1000), STYLE_BOTTOM_RIGHT)`.
  - `CTheScripts::OnAMissionFlag = 0`.
  - `ProjectPlayer::instance().clearMissionMode()`.
- [x] 5.5 Implement `make()` and `builder()`.

## 6. Register the new objectives

- [x] 6.1 Add `#include "objectives/mission_start.h"` and `#include "objectives/mission_end.h"` in `src/core/objectives_manager.cpp` near the other objective includes.
- [x] 6.2 Add `this->registerObjectiveBuilder<mission_start::Data>(mission_start::builder());` and the matching line for `mission_end` next to `cutscene::builder()` at `objectives_manager.cpp:125`.

## 7. Lua binding

- [x] 7.1 In `src/lua/project_player_binding.cpp` around line 100 (where `"stop"` and `"fail"` are bound), add `"isMission", sol::property(&ProjectPlayer::isMissionActive)` to expose the read-only flag. — Implemented as `"is_mission"` (snake_case per actual convention).
- [x] 7.2 Update the core Lua declaration file (identified in task 1.3) to declare `ProjectPlayer.isMission: boolean` (read-only) beside the existing `isPlaying`.
- [x] 7.3 Add Lua declarations for `ObjectiveMissionStartData` and `ObjectiveMissionEndData` in `../ingame/definitions/core/` matching the `SOL_LUA_FOR_EACH` field list. The declaration for `ObjectiveMissionStartData` MUST omit `failAction` (not exposed to Lua in v1 — see design.md, "Lua visibility"). Document the omission with a short `---` comment in the declaration so future readers know it is intentional.

## 8. Localization

- [x] 8.1 Add `objectives.core.mission_start.name` display name key in `../ingame/addons/core/languages/en.json` (mirror the `clock` / `cutscene` entries found in task 1.4). — Placed in MAIN `ingame/LDYOM/languages/en.json` instead; addon file does not hold objective display names (see session gotcha).
- [x] 8.2 Add `objectives.core.mission_end.name` in the same file. — Same routing correction as 8.1.
- [x] 8.3 Add editor-panel keys under `objectives.core.mission_start` in `../ingame/languages/en.json`: `title_text`, `title_time`, `fade_in`, `fail_text`, `fail_action`, `fail_action_end_project`, `nested_error.title`, `nested_error.body` (body is an `fmt::format` template with `{}` placeholders for scene name and objective index).
- [x] 8.4 Add editor-panel keys under `objectives.core.mission_end` in `../ingame/languages/en.json`: `end_text`, `end_time`, `fade_out`.

## 9. Verification

- [x] 9.1 Build the project in Debug configuration (per CLAUDE.md: always build Debug).
- [x] 9.2 Manually verify: project with `mission_start` → `timelimit` that expires → confirm fail text appears bottom-right, controls are blocked, project stops after the banner.
- [x] 9.3 Manually verify: project with `mission_start` → `mission_end` → normal completion; confirm `OnAMissionFlag` is `0` at the end (inspect via CLEO cheat or a debug log line).
- [x] 9.4 Manually verify: project with `mission_start` but no `mission_end`; press LDSTOP cheat mid-mission; confirm `OnAMissionFlag` is `0` after stop (safety net).
- [x] 9.5 Manually verify: project with two `mission_start` objectives in sequence; confirm the second one pops a `Notifications::error` toast naming the scene and objective index, logs via `LDYOM_ERROR`, and stops the project.
- [x] 9.6 Manually verify: project with a standalone `mission_end` (no preceding `mission_start`); confirm it runs as a no-op (no banner, no flag change, next objective runs).
- [x] 9.7 Manually verify: required-actor-death fail (kill an actor marked as required during mission mode); confirm the fail sequence runs with the author-supplied fail text, not a generic banner.
- [x] 9.8 Manually verify: `projectPlayer.isMission` returns `true` from Lua during mission mode and `false` before `mission_start` / after `mission_end`.
- [x] 9.9 Manually verify: project saved with `mission_start` + `mission_end` round-trips through JSON — reload from disk, inspect `failAction.type == "end_project"`, all fields restore.
- [x] 9.10 Manually verify: default scene-transition fade behavior is unaffected (regression check — the `isFaded` handshake at `project_player.cpp:120-132` still works across scene changes with or without mission mode active).
- [x] 9.11 Manually verify re-entrancy: construct a project where two fail signals fire within the same 1-second window during active mission mode (e.g. a `core.timelimit` that expires at the same moment a required actor is scripted to die). Confirm exactly ONE `mission_fail_sequence` banner is shown and the project stops cleanly — no duplicated banner, no leftover tasks.
- [x] 9.12 Manually verify orphan-task cleanup: start a mission, trigger a fail, and while the 1-second banner is still visible, trigger `LDSTOP`. Confirm the banner disappears and `OnAMissionFlag = 0` without any `mission_fail_sequence` log chatter after the stop.
