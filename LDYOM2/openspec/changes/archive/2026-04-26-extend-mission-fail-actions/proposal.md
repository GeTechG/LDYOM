## Why

Mission mode v1 (archived 2026-04-26) shipped the lifecycle, smart `failCurrentProject()` routing, and a `MissionFailAction` variant — but the variant ships with only one alternative: `EndProject`. Authors today have exactly one fail outcome: "you lose, the project stops." Real mission design wants retry-this-checkpoint and jump-to-a-bail-out-scene semantics.

The variant scaffolding was sized exactly for this extension; v2 adds the navigational arms and lifts the deferred sol2 Lua binding (the binding was deferred precisely because v1 had only one variant alternative — that justification expires when a second alternative lands).

A second, smaller goal: bake the post-banner fade-out into the fail sequence as the default for all arms. The user wants the canonical fail UX to be "banner → black → action, transition unseen" — so a stop is hidden under black, a restart is hidden under black, a goto is hidden under black.

## What Changes

- Add two new `MissionFailAction` variants: `RestartScene` (no payload) and `GotoScene { sceneId, optional<objectiveId> }`. Each variant — including the existing `EndProject` — gains a `fadeOut` bool field defaulting to `true`.
- Rework the fail sequence: after the banner, if `fadeOut` is true the sequence runs `DO_FADE(MISSION_FADE_TIME_MS, 0)`, awaits `MISSION_FADE_TIME_MS + 100`, sets `isFaded = true`, then dispatches the action. The black covers the action's transition or stop.
- For navigational arms (`RestartScene`, `GotoScene`), the project does NOT stop. `isPlaying` stays `true`, `transitionPlayingState(false)` is NOT called. The fail-sequence reuses the existing scene-transition machinery (matching what `core.goto_scene` does) and re-spawns the scene loop.
- Refactor `ProjectPlayer::run()`: extract the scene-loop body into a static `runScenesLoop()` coroutine. `run()` becomes startup + `co_await runScenesLoop()`. Navigational fail arms call `addTask("run_project_player", runScenesLoop)` directly, skipping the startup so the player is not faded-in/faded-out a second time.
- Consume `pendingObjectiveJump` at the top of each scene's objective loop (before index 0), so `GotoScene { ..., objectiveId }` can land directly on the requested objective. The post-objective consumption point at `project_player.cpp:145` stays.
- Before a navigational arm performs its transition, the fail sequence sets `CTheScripts::OnAMissionFlag = 0` and calls `clearMissionMode()`. The destination scene's `mission_start` re-arms mission mode if present. If the destination has no `mission_start`, the player lands off-mission predictably (symmetric with `mission_end`).
- For `EndProject` with `fadeOut = true`, ensure the player is faded back in after the project stops. Add fade-in logic to the project's stop path so the screen is restored when the player returns to free roam under fade.
- Expose `MissionFailAction` to Lua: register per-alternative usertypes (`MissionFailEndProject`, `MissionFailRestartScene`, `MissionFailGotoScene`), and bind `mission_start::Data::failAction` as a property whose setter accepts any of the three usertypes and whose getter returns the active alternative as its usertype. Update the core Lua declaration files.
- Editor combo on `mission_start`'s panel: dropdown lists "End project", "Restart scene", "Go to scene". When `GotoScene` is selected, render below it a scene combo and an objective combo (the latter is optional — a "first objective" sentinel is the default). Render a `Fade out` checkbox always.
- Add localization keys for the new combo entries, `GotoScene` payload labels, and the shared `Fade out` label.

## Capabilities

### Modified Capabilities

- `mission-mode`: extends the fail-action variant from one alternative to three; updates the fail-sequence pipeline to optionally fade-out before action dispatch; navigational arms preserve `isPlaying = true`; adds `pendingObjectiveJump` consumption at scene start; adds Lua read/write for `failAction`; ensures fade-in on stop when the project ends faded.

### New Capabilities

_None — extends `mission-mode`._

## Impact

- Code:
  - `src/core/project_player.h` — extend `mission_fail_actions` namespace with `RestartScene` and `GotoScene` structs (each with their own `to_json`/`from_json` and `fadeOut` field); add `fadeOut` field to `EndProject`; widen `MissionFailAction = std::variant<EndProject, RestartScene, GotoScene>`; update `adl_serializer<MissionFailAction>::from_json` to dispatch on three type tags; declare `runScenesLoop()` as a private static `ktwait` method.
  - `src/core/project_player.cpp` — extract `runScenesLoop()` from `run()`; rework `missionFailSequence()` to: snapshot data + flip `active=false`, banner, optional fadeOut, control-restore, dispatch; navigational dispatch arms set `OnAMissionFlag = 0` + `clearMissionMode()` then run scene-load logic + `addTask("run_project_player", runScenesLoop)`; add `pendingObjectiveJump` consumption at the top of the scene's objective loop; ensure fade-in on the stop path when `isFaded` is true.
  - `src/data/objectives/mission_start.h` — editor panel: extend the combo to three entries; render `GotoScene` payload (scene + objective combos) conditionally; render `fadeOut` checkbox; expose new field via `SOL_LUA_FOR_EACH` (or the equivalent variant-aware path — see design.md).
  - `src/lua/project_player_binding.cpp` — register the three variant alternative usertypes; bind `mission_start::Data::failAction` as a property with custom getter/setter that round-trips between Lua tables/usertypes and the C++ variant.
- Localization: `../ingame/languages/en.json` — new keys `objectives.core.mission_start.fail_action_restart_scene`, `fail_action_goto_scene`, `fail_action_goto_scene_target`, `fail_action_goto_scene_objective`, `fail_action_first_objective`, `fade_out`.
- Lua declarations: `../ingame/LDYOM/addons/definitions/core/objectives.lua` — declare `MissionFailEndProject`, `MissionFailRestartScene`, `MissionFailGotoScene` types; update `ObjectiveMissionStartData.failAction` to the union of the three (or string-tagged `@alias`).
- APIs: `MissionFailAction`'s variant alphabet grows from 1 to 3. JSON forward-compat: legacy `failAction = { type: "end_project", data: {} }` loads as `EndProject{ fadeOut = true }` (default applied). Legacy `mission_start` JSON missing the new fields loads with the documented defaults.
- Migration: small UX behavior change — projects authored with `EndProject` and a custom fail text now fade out before the project stops (was: stop in the bright). Authors who want the v1 behavior toggle the new `Fade out` checkbox to off.
- Dependencies/systems: none added.
