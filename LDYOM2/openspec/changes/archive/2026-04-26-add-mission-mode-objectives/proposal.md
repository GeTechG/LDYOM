## Why

LDYOM2 projects need a way to explicitly frame a run as "a mission" — flipping GTA's native `CTheScripts::OnAMissionFlag` so the game treats the player as on-mission (affects gang wars, ped groups, save behavior, radar/radio nuances), showing a bottom-right mission title, and — most importantly — routing every downstream fail signal (time ran out, required actor killed, required vehicle destroyed, scene completion timer) through a single configurable fail sequence with a fail-text banner and a fail action, instead of silently terminating the project.

Today `ProjectPlayer::failCurrentProject()` is literally `{ stopCurrentProject(); }` — there is no distinction between "mission failed" and "project stopped." The existing fail call sites (`timelimit.h`, `actor.cpp:363`, `vehicle.cpp:866`, the scene-completion timer in `project_player.cpp:74`) carry real fail semantics that currently collapse into "stop." The mission-mode feature gives those signals somewhere meaningful to go and gives authors control over the presentation.

## What Changes

- Add a new `core.mission_start` objective that sets `CTheScripts::OnAMissionFlag = 1`, registers a mission-mode runtime state on `ProjectPlayer` (with a user-authored fail text and a fail action), optionally fades in, and prints a bottom-right title banner.
- Add a new `core.mission_end` objective that optionally fades out, prints a bottom-right end banner, clears `OnAMissionFlag`, and clears the mission-mode runtime state.
- Introduce a `MissionModeState` inside `ProjectPlayerState` and a `MissionFailAction` variant (today: `EndProject` only; extensible to `RestartScene`, `GotoScene`, etc.).
- Make `ProjectPlayer::failCurrentProject()` smart: if `missionMode.active` is true, run the mission-fail sequence (block player control → display `failText` in bottom-right → wait → execute fail action); otherwise fall through to the existing `stopCurrentProject()` behavior. All existing fail call sites keep calling `failCurrentProject()` verbatim — branching happens in one place.
- Add a safety net in `ProjectPlayer::transitionPlayingState(false)` that unconditionally clears `OnAMissionFlag` and resets the mission-mode state, so an interrupted project (LDSTOP cheat, UI stop button, missing `mission_end`) never leaves the flag on.
- Expose `ProjectPlayer.isMission` (read-only) to Lua via `src/lua/project_player_binding.cpp` and update the core Lua declaration files accordingly.
- Guard against nested `mission_start`: if a second `mission_start` runs while `missionMode.active` is already `true`, raise a `Notifications::error` toast naming the current scene and the problematic objective index, log via `LDYOM_ERROR`, and stop the project.
- Guard against orphan `mission_end`: if `mission_end` runs while `missionMode.active` is `false`, it does nothing (no flag change, no text) and the player proceeds to the next objective.
- Add localization keys under `objectives.core.mission_start` and `objectives.core.mission_end` in `../ingame/languages/en.json`, plus display names in the core addon's `../ingame/addons/core/languages/en.json`.

## Capabilities

### New Capabilities

- `mission-mode`: Covers the project player's mission-mode lifecycle — toggling `CTheScripts::OnAMissionFlag`, displaying title/fail/end banners, routing fail signals through a configurable fail sequence, and cleaning up on any project termination path.

### Modified Capabilities

_None — `openspec/specs/` contains no existing capability that frames `ProjectPlayer`'s fail path or mission flag. The new capability stands alone; interactions with `scene-transition` are incidental (both write to `isFaded`) and already governed by that capability's fade-state invariants._

## Impact

- Code:
  - `src/core/project_player.h` — `MissionFailAction` variant, `MissionModeState`, extend `ProjectPlayerState`, add `isMissionActive()` accessor.
  - `src/core/project_player.cpp` — smart `failCurrentProject()`, fail-sequence task, safety-net clear in `transitionPlayingState(false)`, include changes.
  - `src/data/objectives/mission_start.h` — new header.
  - `src/data/objectives/mission_end.h` — new header.
  - `src/core/objectives_manager.cpp` — register the two new builders alongside `cutscene`, `clock`, etc.
  - `src/lua/project_player_binding.cpp` — expose `isMission`.
- Localization: `../ingame/languages/en.json` (editor labels, tooltip text, runtime banner defaults if any).
- Addon strings: `../ingame/addons/core/languages/en.json` (objective display names).
- Lua declarations: `../ingame/definitions/core/` (new `ProjectPlayer.isMission` field; objective Data schemas).
- APIs: `ProjectPlayer::failCurrentProject()` gains mission-aware behavior but keeps its signature. `ProjectPlayer::isMissionActive()` is new. Lua gets `projectPlayer.isMission` read-only. No breaking changes to existing projects — new objectives are opt-in, legacy projects never set `OnAMissionFlag` and therefore never trigger the new fail path.
- Dependencies/systems: none added. Uses existing `CTheScripts::OnAMissionFlag`, `CMessages::AddBigMessage`, `plugin::Commands::DO_FADE`, `plugin::Commands::SET_PLAYER_CONTROL`, `Notifications::error`, `TaskManager`.
