## ADDED Requirements

### Requirement: Mission-mode start objective

The `core.mission_start` objective SHALL, on execute, set `CTheScripts::OnAMissionFlag = 1`, store a `MissionModeState` (with the author's fail text mode, cp1251+gxt-encoded custom fail text, and fail action) on `ProjectPlayer`, and display the author's title text in the bottom-right using `CMessages::AddBigMessage` with `STYLE_BOTTOM_RIGHT`. If the objective's `fadeIn` flag is true and the screen is currently faded to black, it SHALL fade in over ~500ms before the banner appears.

#### Scenario: Mission start activates flag and banner

- **WHEN** a `core.mission_start` objective runs with a non-empty `titleText`
- **THEN** `CTheScripts::OnAMissionFlag` MUST be `1` immediately after execute returns
- **AND** a bottom-right banner MUST display the title text for `titleTime` seconds

#### Scenario: Mission start caches encoded fail text only for Custom mode

- **WHEN** a `core.mission_start` objective executes with `failTextMode = 1` (Custom) and `failText = "Mission failed."`
- **THEN** the stored `MissionModeState` MUST contain a cached `failTextGxt` that is the cp1251-encoded + gxt-encoded form of the author's `failText`
- **AND** the fail sequence MUST consume `failTextGxt` without re-encoding

- **WHEN** a `core.mission_start` objective executes with `failTextMode = 0` (Default)
- **THEN** the stored `MissionModeState.failTextGxt` MUST be empty (no encoding work performed)
- **AND** the fail sequence MUST render the built-in `M_FAIL` GXT entry instead of reading `failTextGxt`

#### Scenario: Mission start fade-in honours existing fade state

- **WHEN** a `core.mission_start` objective runs with `fadeIn = true` and `ProjectPlayer::isFaded()` is `true`
- **THEN** the player MUST issue `DO_FADE` with flag `1` (fade IN) over ~500ms and MUST set `isFaded` to `false`

- **WHEN** a `core.mission_start` objective runs with `fadeIn = true` and `isFaded()` is already `false`
- **THEN** the player MUST NOT issue a redundant fade and the banner MUST appear immediately

### Requirement: Mission-mode end objective

The `core.mission_end` objective SHALL, on execute, clear `CTheScripts::OnAMissionFlag`, clear the `MissionModeState` on `ProjectPlayer`, and display an end banner in the middle of the screen using `STYLE_MIDDLE`. If `fadeOut` is true and the screen is currently visible, it SHALL fade to black over ~500ms before the banner. If `endTune` is true, the classic SA mission-passed tune (`PLAY_MISSION_PASSED_TUNE(1)`, opcode `0394` — the same opcode DYOM uses on its mission-passed path) SHALL play together with the banner.

#### Scenario: Mission end deactivates flag and banner

- **WHEN** a `core.mission_end` objective runs while `ProjectPlayer::isMissionActive()` is `true`
- **THEN** `CTheScripts::OnAMissionFlag` MUST be `0` after execute returns
- **AND** the `MissionModeState` MUST be reset to its default (inactive)
- **AND** a middle-of-screen banner MUST display the selected end text for `endTime` seconds

#### Scenario: Mission end with fade-out

- **WHEN** a `core.mission_end` objective runs with `fadeOut = true` and `isFaded()` is `false`
- **THEN** the player MUST issue `DO_FADE` with flag `0` (fade OUT), wait ~500ms + 100ms for the fade to complete, set `isFaded` to `true`, then display the end banner

#### Scenario: Mission end plays the mission-passed tune on demand

- **WHEN** a `core.mission_end` objective runs with `endTune = true`
- **THEN** `PLAY_MISSION_PASSED_TUNE(1)` MUST be invoked right before the banner is shown

- **WHEN** a `core.mission_end` objective runs with `endTune = false`
- **THEN** no audio MUST be triggered by the objective

### Requirement: Orphan mission-end is a no-op

When `core.mission_end` executes and `ProjectPlayer::isMissionActive()` is `false`, the objective SHALL return without writing to `OnAMissionFlag`, without touching `MissionModeState`, without displaying a banner, and without fading. The next objective SHALL proceed normally.

#### Scenario: Mission end without prior mission start

- **WHEN** a `core.mission_end` objective runs in a scene that contains no preceding `core.mission_start`
- **THEN** the objective MUST complete without any visible effect
- **AND** `CTheScripts::OnAMissionFlag` MUST remain `0`
- **AND** the following objective MUST execute on the next tick

### Requirement: Nested mission-start halts the project with an author error

When `core.mission_start` executes and `ProjectPlayer::isMissionActive()` is already `true`, the objective SHALL raise a `Notifications::error` toast whose body names the current scene (by `SceneInfo::name`) and the current objective index, emit an `LDYOM_ERROR` log line with the same data, and then call `ProjectPlayer::stopCurrentProject()`. It SHALL NOT call `failCurrentProject()` — this is an authoring error, not a mission failure.

#### Scenario: Second mission_start in the same mission stops the project

- **GIVEN** a project where `core.mission_start` has already executed and `missionMode.active` is `true`
- **WHEN** a second `core.mission_start` objective executes
- **THEN** `Notifications::error` MUST be invoked with a title and body identifying the scene name and objective index
- **AND** `LDYOM_ERROR` MUST log a line containing both the scene name and the objective index
- **AND** `ProjectPlayer::stopCurrentProject()` MUST be called (not `failCurrentProject()`)
- **AND** the mission-fail banner MUST NOT appear (no fail sequence for authoring errors)

### Requirement: Smart `failCurrentProject()` routes through the fail sequence when mission mode is active

`ProjectPlayer::failCurrentProject()` SHALL branch on `missionMode.active`. When `false`, it SHALL behave exactly as `stopCurrentProject()` (preserving today's semantics). When `true`, it SHALL cancel the main run task and spawn a `mission_fail_sequence` coroutine task that: blocks player control with `SET_PLAYER_CONTROL(0, false)`, displays the mission-fail banner in the middle of the screen (`STYLE_MIDDLE`) for `MISSION_FAIL_TEXT_TIME_MS` milliseconds, unblocks player control, and then executes the stored `MissionFailAction` via `std::visit`. The banner SHALL be selected from `missionMode.failTextMode`: `0` renders the built-in GXT key `"M_FAIL"` via `PRINT_BIG` (opcode `00BA`, style `1`); `1` renders the cached author-supplied `failTextGxt` via `CMessages::AddBigMessage` with `STYLE_MIDDLE`. All existing call sites of `failCurrentProject()` SHALL remain unchanged — the decision happens in one place.

#### Scenario: Fail in free-roam project behaves as a hard stop

- **GIVEN** a project with no `core.mission_start` objective (so `missionMode.active` is `false`)
- **WHEN** any fail signal calls `ProjectPlayer::failCurrentProject()` (e.g. timelimit expiry, required-actor death)
- **THEN** the player MUST stop the project immediately with no fail banner
- **AND** the behavior MUST be identical to calling `stopCurrentProject()` directly

#### Scenario: Fail in active mission plays the fail sequence

- **GIVEN** a project where `core.mission_start` has executed and `missionMode.active` is `true`
- **WHEN** any fail signal calls `ProjectPlayer::failCurrentProject()`
- **THEN** the `run_project_player` task MUST be cancelled
- **AND** a `mission_fail_sequence` task MUST run that: disables player control, shows the selected fail text in the middle of the screen for ~1000ms, re-enables player control, then executes the stored fail action
- **AND** the `EndProject` fail action (v1's only supported action) MUST result in `stopCurrentProject()` being called after the banner completes

#### Scenario: Every existing fail caller routes through the new path automatically

- **WHEN** `core.timelimit`'s timer expires during active mission mode
- **OR** the scene-completion timer in `ProjectPlayer::run()` expires during active mission mode
- **OR** a required actor marked in the actor component dies during active mission mode
- **OR** a required vehicle marked in the vehicle component is destroyed during active mission mode
- **THEN** the fail sequence MUST run with the mission's configured fail text and fail action
- **AND** the caller sites MUST NOT have been modified to achieve this

### Requirement: Fail sequence is re-entrancy-safe

The mission-fail coroutine SHALL, before its first `co_await`, set `missionMode.active = false` (after copying the fail text and fail action into local variables for its own use). A second `ProjectPlayer::failCurrentProject()` call arriving while the fail banner is still on-screen MUST therefore fall through to the `stopCurrentProject()` branch and MUST NOT spawn a second `mission_fail_sequence` task.

#### Scenario: Two fail signals in the same tick window

- **GIVEN** a project in active mission mode with a configured fail text and `EndProject` fail action
- **WHEN** two independent fail signals fire in quick succession (for example, a required actor is killed at the same moment a required vehicle is destroyed, both within the ~1000ms banner window)
- **THEN** exactly one `mission_fail_sequence` task MUST exist at any point in time
- **AND** exactly one fail banner MUST be displayed
- **AND** the project MUST stop cleanly after the banner

#### Scenario: Banner data survives re-entry guard

- **WHEN** the fail coroutine runs and flips `missionMode.active = false` at its start
- **THEN** the banner text, player-control restoration, and fail-action dispatch MUST all use local copies of `failTextGxt` and `failAction` captured before the flip
- **AND** the banner and fail action MUST complete correctly even though `missionMode` has been reset (either by the guard itself or by the safety net running inside the fail action)

### Requirement: Safety net removes the fail-sequence task on any stop

`ProjectPlayer::transitionPlayingState(false)` SHALL call `TaskManager::removeTask("mission_fail_sequence")` alongside the other `removeTask` calls in its stop branch. This guarantees that a user triggering `LDSTOP` or the UI stop button mid-banner does not leave an orphaned fail coroutine running after the project has ended.

#### Scenario: LDSTOP during the fail banner

- **GIVEN** a project in active mission mode whose fail sequence is currently displaying the banner
- **WHEN** the user triggers `LDSTOP`
- **THEN** the fail banner MUST be cancelled
- **AND** no `mission_fail_sequence` task MUST remain registered in `TaskManager`
- **AND** `CTheScripts::OnAMissionFlag` MUST be `0`

### Requirement: Mission-fail text source is author-selectable

The `core.mission_start::Data` struct SHALL expose a `failTextMode` integer (default `0`) that selects one of two fail-banner renderings:

- `0` — Default: the fail sequence renders the built-in GXT key `"M_FAIL"` via `plugin::Command<plugin::Commands::PRINT_BIG>("M_FAIL", time, 1)`. The author-supplied `failText` field SHALL be ignored in this mode.
- `1` — Custom: the fail sequence renders the author-supplied `failText` (encoded once during `mission_start::execute()` into `MissionModeState.failTextGxt` via `utf8_to_cp1251` + `gxt_encode`) with `CMessages::AddBigMessage(..., STYLE_MIDDLE)`.

The editor panel SHALL show the `failText` input only when `failTextMode == 1`.

#### Scenario: Default fail text mode uses the built-in GXT

- **GIVEN** a `core.mission_start` objective with `failTextMode = 0`
- **WHEN** a fail signal triggers the fail sequence
- **THEN** the banner text MUST be the game's built-in `"M_FAIL"` GXT entry
- **AND** any value in the author's `failText` field MUST NOT be rendered

#### Scenario: Custom fail text mode uses author-supplied text

- **GIVEN** a `core.mission_start` objective with `failTextMode = 1` and `failText = "You died."`
- **WHEN** a fail signal triggers the fail sequence
- **THEN** the banner MUST display the cp1251+gxt-encoded form of `"You died."`

### Requirement: Mission-end text source is author-selectable

The `core.mission_end::Data` struct SHALL expose an `endTextMode` integer (default `0`) that selects one of three end-banner renderings:

- `0` — Default: renders the built-in GXT key `"M_PASSD"` via `plugin::Command<plugin::Commands::PRINT_BIG>("M_PASSD", time, 1)` (plain "MISSION PASSED!" text).
- `1` — With money: renders the built-in GXT key `"M_PASS"` (which contains the `~1~` number placeholder) via `plugin::Command<plugin::Commands::PRINT_WITH_NUMBER_BIG>("M_PASS", endMoney, time, 1)`. The `endMoney` integer is DISPLAY-ONLY — the objective SHALL NOT grant or deduct any cash; authors chain a `core.money_add` objective to actually award the money.
- `2` — Custom: renders the author-supplied `endText` (encoded once during `mission_end::execute()` into `gameEndText`) with `CMessages::AddBigMessage(..., STYLE_MIDDLE)`.

The editor panel SHALL show only the input relevant to the selected mode (money input for `1`, text input for `2`).

#### Scenario: Default end text mode uses M_PASSD

- **GIVEN** a `core.mission_end` objective with `endTextMode = 0`
- **WHEN** the objective executes inside an active mission
- **THEN** the banner text MUST be the game's built-in `"M_PASSD"` GXT entry
- **AND** `endText` / `endMoney` MUST be ignored

#### Scenario: With-money end text uses M_PASS with a numeric substitution

- **GIVEN** a `core.mission_end` objective with `endTextMode = 1` and `endMoney = 5000`
- **WHEN** the objective executes inside an active mission
- **THEN** the banner MUST render `"M_PASS"` with the `~1~` placeholder substituted with `5000`
- **AND** the player's cash MUST NOT be modified by the objective itself

#### Scenario: Custom end text renders author text

- **GIVEN** a `core.mission_end` objective with `endTextMode = 2` and `endText = "Well done!"`
- **WHEN** the objective executes inside an active mission
- **THEN** the banner MUST display the cp1251+gxt-encoded form of `"Well done!"`

### Requirement: Mission-mode state is runtime-only, never persisted

The `MissionModeState` on `ProjectPlayerState` SHALL be runtime-only. It SHALL NOT be serialized to project JSON. The author-facing fields (`failTextMode`, `failText`, `failAction` on `mission_start::Data`; `endTextMode`, `endText`, `endMoney`, `fadeOut`, `endTune` on `mission_end::Data`) live inside the objective data (which IS serialized) and are copied into `MissionModeState` only when `mission_start` executes.

#### Scenario: MissionModeState is not part of project save files

- **WHEN** a project containing `core.mission_start` and `core.mission_end` objectives is saved to disk
- **THEN** the project JSON MUST NOT contain any fields derived from the live `MissionModeState` struct
- **AND** on reload, `MissionModeState.active` MUST be `false` until `mission_start` executes

### Requirement: Fail-action model supports extensible variant of structs

The `MissionFailAction` type SHALL be implemented as a `std::variant` of per-action POD structs. v1 SHALL ship one alternative: `mission_fail_actions::EndProject` (no payload). Future actions SHALL be added by appending to the variant without breaking existing projects.

The JSON representation SHALL use a tagged envelope `{ "type": "<TYPE>", "data": { ... } }`, where `<TYPE>` is the `TYPE` constant on the struct. `from_json` SHALL throw a descriptive error when it encounters an unknown `type` string.

#### Scenario: EndProject fail action round-trips through JSON

- **WHEN** a `core.mission_start` objective with `failAction = EndProject{}` is saved to JSON and reloaded
- **THEN** the reloaded `failAction` MUST be an `EndProject` variant
- **AND** the serialized JSON MUST contain `"type": "end_project"` somewhere inside the `failAction` value

#### Scenario: Unknown fail-action type raises a clear error

- **WHEN** a project JSON contains a `failAction` with `"type": "does_not_exist"`
- **THEN** `from_json` on `MissionFailAction` MUST throw an exception whose message names the unknown type string

### Requirement: Safety net forces `OnAMissionFlag` off on every project stop

`ProjectPlayer::transitionPlayingState(false)` SHALL unconditionally assign `CTheScripts::OnAMissionFlag = 0` and reset the `MissionModeState` to a default-constructed value. This SHALL run after every project termination regardless of the cause — normal `mission_end`, user-triggered stop, `LDSTOP` cheat, UI stop button, fail-sequence completion, scene-completion timer expiry, or crash recovery.

#### Scenario: LDSTOP cheat mid-mission clears the flag

- **GIVEN** a project where `core.mission_start` has executed
- **WHEN** the user triggers the `LDSTOP` cheat before `core.mission_end` runs
- **THEN** `CTheScripts::OnAMissionFlag` MUST be `0` after the stop completes
- **AND** `ProjectPlayer::isMissionActive()` MUST return `false`

#### Scenario: UI stop button mid-mission clears the flag

- **GIVEN** a project where `core.mission_start` has executed
- **WHEN** the user stops the project from the editor UI before `core.mission_end` runs
- **THEN** `CTheScripts::OnAMissionFlag` MUST be `0` after the stop completes

#### Scenario: Successful mission_end + safety net is idempotent

- **GIVEN** a project that runs `core.mission_start`, normal objectives, then `core.mission_end` and reaches the natural project end
- **WHEN** `transitionPlayingState(false)` executes at project end
- **THEN** writing `OnAMissionFlag = 0` a second time MUST have no adverse effect (flag was already `0` from `mission_end`)

### Requirement: Lua exposes mission state as a read-only property

`ProjectPlayer`'s Lua binding in `src/lua/project_player_binding.cpp` SHALL expose `is_mission` (snake_case, matching the existing `is_playing` / `is_faded` naming convention of the binding) as a read-only property that returns `ProjectPlayer::isMissionActive()`. The core Lua declaration file SHALL declare `---@field is_mission boolean` alongside the existing `is_playing` declaration.

#### Scenario: Lua read of mission state

- **WHEN** Lua code evaluates `project_player.is_mission` during active mission mode
- **THEN** the result MUST be `true`

- **WHEN** Lua code evaluates `project_player.is_mission` before `core.mission_start` runs or after `core.mission_end` runs
- **THEN** the result MUST be `false`

#### Scenario: Lua cannot write mission state

- **WHEN** Lua code attempts `project_player.is_mission = true`
- **THEN** the write MUST fail or be silently ignored (property has no setter)
- **AND** `CTheScripts::OnAMissionFlag` MUST remain unchanged

### Requirement: Legacy projects load unchanged

Existing project JSON files that predate this change MUST continue to load and run identically. Legacy projects contain neither `core.mission_start` nor `core.mission_end`, so `MissionModeState.active` remains `false`, `failCurrentProject()` falls through to its existing `stopCurrentProject()` behavior, and `OnAMissionFlag` is never written by LDYOM.

#### Scenario: Project with no mission objectives behaves as before

- **WHEN** a legacy project (no `core.mission_start` / `core.mission_end`) is loaded and played
- **THEN** the runtime fail behavior MUST be identical to pre-change behavior
- **AND** `CTheScripts::OnAMissionFlag` MUST remain `0` throughout the run
- **AND** no fail banner MUST appear on any fail signal
