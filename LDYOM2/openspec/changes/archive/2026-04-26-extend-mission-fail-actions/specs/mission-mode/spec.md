## ADDED Requirements

### Requirement: Fail sequence runs an optional fade-out before the action dispatches

The `mission_fail_sequence` coroutine SHALL, after the banner-hold `co_await` and before the action visit, read `fadeOut` from the active variant alternative. If `fadeOut` is `true` and `ProjectPlayer::isFaded()` is `false`, the sequence SHALL issue `DO_FADE` with flag `0` (fade OUT) over `MISSION_FADE_TIME_MS` milliseconds, `co_await MISSION_FADE_TIME_MS + 100`, and `setFaded(true)`. The black covers the action's transition or stop, so the player does not see a flash of the source scene before navigation, nor a flash of free-roam after `EndProject`.

#### Scenario: Fade-out covers an EndProject stop

- **GIVEN** a `mission_start` objective with `failAction = EndProject{ fadeOut = true }` (the default)
- **WHEN** any fail signal triggers the fail sequence
- **THEN** the fail banner MUST display for `MISSION_FAIL_TEXT_TIME_MS`
- **AND** the screen MUST fade to black before `stopCurrentProject()` runs
- **AND** the screen MUST fade back in once the project has stopped (covered by the stop-path fade-in requirement)

#### Scenario: Fade-out covers a navigational transition

- **GIVEN** a `mission_start` objective with `failAction = RestartScene{ fadeOut = true }` or `GotoScene{ ..., fadeOut = true }`
- **WHEN** any fail signal triggers the fail sequence
- **THEN** the screen MUST fade to black after the banner and before the destination scene loads
- **AND** the destination scene's first objective (or its `mission_start.fadeIn`) MUST handle the fade-in

#### Scenario: Fade-out is skipped if already faded

- **GIVEN** a fail signal fires while `ProjectPlayer::isFaded()` is already `true` (e.g. mid-cutscene fadeout)
- **WHEN** the fail sequence reaches the fade-out step
- **THEN** no redundant `DO_FADE` MUST be issued
- **AND** the action visit MUST proceed immediately after the banner-hold completes

#### Scenario: fadeOut=false skips the fade entirely

- **GIVEN** any fail-action variant alternative with `fadeOut = false`
- **WHEN** the fail sequence dispatches that action
- **THEN** the screen MUST NOT fade
- **AND** the action MUST run immediately after the banner-hold

### Requirement: RestartScene fail action restarts the current scene without stopping the project

When the active `MissionFailAction` is `RestartScene`, after the fail banner (and optional fade-out), the fail sequence SHALL preserve `ProjectPlayer::isPlaying() = true`, NOT call `transitionPlayingState(false)`, write `CTheScripts::OnAMissionFlag = 0`, call `clearMissionMode()`, tear down the source scene's per-scene runtime state (cancel Lua tasks, clear the project tasklist, reset and re-load the same scene), and re-spawn `run_project_player` task pointing at `runScenesLoop` so the scene loop resumes without re-running project startup. The destination scene's own `mission_start` objective (if present) re-arms mission mode.

#### Scenario: RestartScene reloads the current scene from objective 0

- **GIVEN** a project where `currentSceneId = "S1"` is in active mission mode with `failAction = RestartScene{}`
- **WHEN** any fail signal triggers the fail sequence
- **THEN** `ProjectPlayer::isPlaying()` MUST remain `true` throughout the fail sequence
- **AND** `transitionPlayingState(false)` MUST NOT be called
- **AND** `currentSceneId` MUST equal `"S1"` after the navigational arm completes
- **AND** the new scene loop MUST execute objective index 0 first

#### Scenario: RestartScene re-arms via destination's mission_start

- **GIVEN** scene `S1` whose objective 0 is `core.mission_start`
- **WHEN** a `RestartScene` fail action fires from inside `S1`
- **THEN** the navigational arm MUST clear `OnAMissionFlag` and `MissionModeState`
- **AND** the new scene loop's objective 0 (the `mission_start`) MUST re-arm mission mode by setting `OnAMissionFlag = 1` and writing a fresh `MissionModeState`

### Requirement: GotoScene fail action transitions to a chosen scene with optional starting objective

When the active `MissionFailAction` is `GotoScene { sceneId, objectiveId }`, after the fail banner (and optional fade-out), the fail sequence SHALL run the same project-preserving navigational continuation as `RestartScene` — `isPlaying = true`, no `transitionPlayingState(false)`, clear `OnAMissionFlag` + `MissionModeState`, tear down the source scene, load `sceneId` — and additionally, if `objectiveId` is set, call `requestObjectiveJump(*objectiveId)` BEFORE re-spawning the run task so the scene loop's start-of-scene consumption point lands on the requested objective.

If `sceneId` does not exist in `ScenesManager::instance().getScenesInfo()`, the navigational arm SHALL log `LDYOM_ERROR` naming the unknown id and fall through to `stopCurrentProject()` instead of crashing.

#### Scenario: GotoScene with no objectiveId starts from objective 0

- **GIVEN** a `mission_start` with `failAction = GotoScene{ sceneId = "S2", objectiveId = nullopt }`
- **WHEN** the fail sequence's visit arm runs
- **THEN** `currentSceneId` MUST equal `"S2"` after the navigational arm completes
- **AND** the scene's objective at index 0 MUST be the first to execute

#### Scenario: GotoScene with objectiveId lands on the requested objective

- **GIVEN** a `mission_start` with `failAction = GotoScene{ sceneId = "S2", objectiveId = "<UUID-of-objective-3-in-S2>" }`
- **WHEN** the fail sequence's visit arm runs
- **THEN** scene `S2` MUST be loaded
- **AND** the scene loop's first objective execution MUST be the one whose `id` matches the supplied UUID
- **AND** objectives 0..2 in `S2` MUST be skipped on this entry to the scene

#### Scenario: GotoScene with unresolvable objectiveId falls back to objective 0

- **GIVEN** a `mission_start` with `failAction = GotoScene{ sceneId = "S2", objectiveId = "<UUID-not-in-S2>" }`
- **WHEN** the fail sequence's visit arm runs
- **THEN** scene `S2` MUST be loaded
- **AND** the scene loop's start-of-scene consumption point MUST treat the unresolvable id as a miss and start at objective index 0
- **AND** no exception or crash MUST occur

#### Scenario: GotoScene with stale sceneId stops the project

- **GIVEN** a `mission_start` with `failAction = GotoScene{ sceneId = "<deleted-scene-uuid>", objectiveId = nullopt }`
- **WHEN** the fail sequence's visit arm runs
- **THEN** `LDYOM_ERROR` MUST log a line naming the unknown scene id
- **AND** `stopCurrentProject()` MUST be called as a safe fallback
- **AND** no `loadScene` call MUST be made with the invalid id

### Requirement: Pending objective jump is consumed at scene start

`runScenesLoop()` SHALL read `m_state.pendingObjectiveJump` at the top of each scene's objective phase, before the first objective in the loaded scene runs. If the pending value resolves to an objective in the current scene's objective list, the for-loop SHALL start at that objective's index instead of `0`. The pending value SHALL be cleared on read whether or not it resolves. The post-objective consumption point at `project_player.cpp:145` SHALL remain unchanged; at most one of the two consumption points fires per scene-load because both clear the optional on read.

#### Scenario: Start-of-scene consumption with a resolvable id

- **GIVEN** a scene `S` with objectives `[A, B, C]`
- **AND** `pendingObjectiveJump = "<UUID-of-B>"` set before `S` begins running
- **WHEN** `runScenesLoop()` enters the objective phase for `S`
- **THEN** the first executed objective MUST be `B`
- **AND** `pendingObjectiveJump` MUST be `nullopt` after the consumption point runs

#### Scenario: Start-of-scene consumption with an unresolvable id

- **GIVEN** a scene `S` with objectives `[A, B, C]`
- **AND** `pendingObjectiveJump = "<UUID-not-in-S>"` set before `S` begins running
- **WHEN** `runScenesLoop()` enters the objective phase for `S`
- **THEN** the first executed objective MUST be `A` (start at index 0 — fallback)
- **AND** `pendingObjectiveJump` MUST be `nullopt` after the consumption point runs

#### Scenario: Mid-scene jump still works via post-objective consumption

- **GIVEN** a scene `S` with objectives `[A, B, C]`
- **AND** an objective in `S` calls `requestObjectiveJump(<UUID-of-B>)` during its `execute()`
- **WHEN** that objective returns
- **THEN** the post-objective consumption at `project_player.cpp:145` MUST consume the pending value
- **AND** the next executed objective MUST be `B`
- **AND** the start-of-scene consumption point MUST NOT have fired (the optional was empty when the scene loaded)

### Requirement: Fail-action variant exposes per-arm `fadeOut` field

Each alternative of `MissionFailAction` (`EndProject`, `RestartScene`, `GotoScene`) SHALL expose a `bool fadeOut = true` member. The default is `true` for all alternatives so the canonical fail UX (banner → black → action) applies out of the box. Authors can disable fade per-arm in the editor or via Lua. Each alternative serializes its own `fadeOut` field through its `to_json` / `from_json`.

#### Scenario: All three alternatives default fadeOut to true

- **GIVEN** a default-constructed `EndProject{}`, `RestartScene{}`, and `GotoScene{}`
- **THEN** each MUST have `fadeOut == true`

#### Scenario: fadeOut round-trips through JSON

- **WHEN** a `mission_start` with `failAction = RestartScene{ fadeOut = false }` is saved and reloaded
- **THEN** the reloaded `failAction` MUST equal `RestartScene{ fadeOut = false }`
- **AND** the on-disk JSON MUST contain `"fadeOut": false` somewhere inside the `data` object of the `failAction` envelope

### Requirement: Stop path restores fade when project ends faded

The `stopping_project_player` task SHALL, after `playerLeaveAnyVehicle()` and before `m_state.isPlaying = false`, check `ProjectPlayer::isFaded()`. If `true`, the task SHALL issue `DO_FADE` with flag `1` (fade IN) over `MISSION_FADE_TIME_MS`, `co_await MISSION_FADE_TIME_MS`, and `setFaded(false)`. This guarantees the screen is restored regardless of the cause of the fade — fail-sequence's pre-action fadeOut, an in-flight cutscene fade, or any future authored fade left active at stop.

#### Scenario: EndProject with fadeOut clears the screen on stop

- **GIVEN** a fail sequence runs with `EndProject{ fadeOut = true }`
- **WHEN** the visit arm calls `stopCurrentProject()` (which spawns `stopping_project_player`)
- **THEN** the stop task MUST fade the screen back in before `transitionPlayingState(false)` runs
- **AND** the player MUST land in free roam under bright (no leftover black)

#### Scenario: User-stop mid-cutscene-fadeout also restores the screen

- **GIVEN** a project mid-`core.cutscene` whose internal fade-out has just set `isFaded = true`
- **WHEN** the user triggers `LDSTOP` or the UI stop button
- **THEN** the stop task MUST fade in before `transitionPlayingState(false)` runs

#### Scenario: Stop with isFaded=false skips the fade-in

- **GIVEN** a project whose fail sequence ran with `EndProject{ fadeOut = false }` (no fade-out happened)
- **WHEN** `stopping_project_player` runs
- **THEN** no redundant `DO_FADE` MUST be issued
- **AND** the project MUST stop in the same visual state it was in pre-fail

### Requirement: Lua exposes `mission_start::Data::failAction` as a read/write property

`mission_start::Data::failAction` SHALL be bound to Lua via a sol2 `sol::property` whose getter returns the active variant alternative as its registered usertype (`MissionFailEndProject`, `MissionFailRestartScene`, or `MissionFailGotoScene`), and whose setter accepts any of the three usertypes and assigns into the variant. Assigning a value of any other type SHALL raise a `sol::error` whose message names the three accepted types. The variant's three alternatives SHALL be registered as sol2 usertypes with their fields bound by name (`fadeOut` on all; `sceneId` and `objectiveId` on `GotoScene`). `objectiveId` SHALL map between Lua `nil` and `std::nullopt`.

#### Scenario: Lua read of EndProject failAction

- **GIVEN** a `mission_start::Data` with `failAction = EndProject{ fadeOut = true }`
- **WHEN** Lua code evaluates `data.failAction`
- **THEN** the result MUST be a `MissionFailEndProject` usertype instance
- **AND** `result.fadeOut` MUST be `true`

#### Scenario: Lua write of GotoScene failAction round-trips

- **WHEN** Lua executes `data.failAction = MissionFailGotoScene.new(); data.failAction.sceneId = "S2"; data.failAction.objectiveId = nil; data.failAction.fadeOut = true`
- **THEN** subsequent reads of `data.failAction` MUST return a `MissionFailGotoScene` usertype with the assigned values
- **AND** saving to JSON MUST emit `{ "type": "goto_scene", "data": { "sceneId": "S2", "fadeOut": true } }` (with `objectiveId` either omitted or null)

#### Scenario: Lua write of an unsupported type raises an error

- **WHEN** Lua executes `data.failAction = {}` (plain table) or `data.failAction = "foo"` (string)
- **THEN** a `sol::error` MUST be raised
- **AND** the error message MUST name the three accepted types

## MODIFIED Requirements

### Requirement: Smart `failCurrentProject()` routes through the fail sequence when mission mode is active

`ProjectPlayer::failCurrentProject()` SHALL branch on `missionMode.active`. When `false`, it SHALL behave exactly as `stopCurrentProject()` (preserving today's semantics). When `true`, it SHALL cancel the main run task and spawn a `mission_fail_sequence` coroutine task that: blocks player control with `SET_PLAYER_CONTROL(0, false)`, displays the mission-fail banner in the middle of the screen (`STYLE_MIDDLE`) for `MISSION_FAIL_TEXT_TIME_MS` milliseconds, optionally fades the screen to black if the active fail action's `fadeOut` is `true` (with the `!isFaded` guard), unblocks player control, and then dispatches the stored `MissionFailAction` via `std::visit`. The banner SHALL be selected from `missionMode.failTextMode`: `0` renders the built-in GXT key `"M_FAIL"` via `PRINT_BIG`; `1` renders the cached author-supplied `failTextGxt` via `CMessages::AddBigMessage` with `STYLE_MIDDLE`. The visit arms are:

- `EndProject` → calls `stopCurrentProject()` (project teardown).
- `RestartScene` → preserves `isPlaying = true`, clears `OnAMissionFlag` + `MissionModeState`, reloads the current scene, and re-spawns `run_project_player` at `runScenesLoop`.
- `GotoScene { sceneId, objectiveId }` → preserves `isPlaying = true`, clears `OnAMissionFlag` + `MissionModeState`, loads the target scene, optionally calls `requestObjectiveJump(*objectiveId)`, and re-spawns `run_project_player` at `runScenesLoop`. If `sceneId` is unresolvable, falls through to `stopCurrentProject()` with `LDYOM_ERROR`.

All existing call sites of `failCurrentProject()` SHALL remain unchanged — the decision happens in one place.

#### Scenario: Fail in free-roam project behaves as a hard stop

- **GIVEN** a project with no `core.mission_start` objective (so `missionMode.active` is `false`)
- **WHEN** any fail signal calls `ProjectPlayer::failCurrentProject()` (e.g. timelimit expiry, required-actor death)
- **THEN** the player MUST stop the project immediately with no fail banner
- **AND** the behavior MUST be identical to calling `stopCurrentProject()` directly

#### Scenario: Fail in active mission with EndProject runs the fail sequence and stops

- **GIVEN** a project where `core.mission_start` has executed and `missionMode.active` is `true`, with `failAction = EndProject{}`
- **WHEN** any fail signal calls `ProjectPlayer::failCurrentProject()`
- **THEN** the `run_project_player` task MUST be cancelled
- **AND** the fail sequence MUST run banner → optional fade-out → control restore → `stopCurrentProject()`

#### Scenario: Fail in active mission with RestartScene preserves the project

- **GIVEN** a project in active mission mode with `failAction = RestartScene{}`
- **WHEN** any fail signal calls `failCurrentProject()`
- **THEN** the fail sequence MUST run, ending with the navigational arm
- **AND** `ProjectPlayer::isPlaying()` MUST remain `true` throughout
- **AND** the current scene MUST reload with the run loop re-spawned

#### Scenario: Fail in active mission with GotoScene transitions to the chosen scene

- **GIVEN** a project in active mission mode with `failAction = GotoScene{ sceneId = "S2", objectiveId = nullopt }`
- **WHEN** any fail signal calls `failCurrentProject()`
- **THEN** the fail sequence MUST run, ending with the navigational arm
- **AND** `currentSceneId` MUST equal `"S2"` after the arm completes
- **AND** `ProjectPlayer::isPlaying()` MUST remain `true`

#### Scenario: Every existing fail caller routes through the new path automatically

- **WHEN** `core.timelimit`'s timer expires during active mission mode
- **OR** the scene-completion timer in `ProjectPlayer::run()` expires during active mission mode
- **OR** a required actor marked in the actor component dies during active mission mode
- **OR** a required vehicle marked in the vehicle component is destroyed during active mission mode
- **THEN** the fail sequence MUST run with the mission's configured fail text and fail action
- **AND** the caller sites MUST NOT have been modified to achieve this

### Requirement: Fail-action model supports extensible variant of structs

The `MissionFailAction` type SHALL be implemented as a `std::variant` of per-action POD structs. v2 SHALL ship three alternatives: `mission_fail_actions::EndProject` (no navigational payload), `mission_fail_actions::RestartScene` (no navigational payload), and `mission_fail_actions::GotoScene { sceneId, optional<objectiveId> }`. All three alternatives SHALL carry a `bool fadeOut = true` field. Future actions SHALL be added by appending to the variant without breaking existing projects.

The JSON representation SHALL use a tagged envelope `{ "type": "<TYPE>", "data": { ... } }`, where `<TYPE>` is the `TYPE` constant on the struct. `from_json` SHALL throw a descriptive error when it encounters an unknown `type` string.

#### Scenario: All three fail actions round-trip through JSON

- **WHEN** a project containing `mission_start` objectives configured with each of `EndProject{ fadeOut = false }`, `RestartScene{}`, and `GotoScene{ sceneId = "S", objectiveId = "O", fadeOut = true }` is saved and reloaded
- **THEN** each reloaded `failAction` MUST be the same variant alternative with the same field values
- **AND** the serialized JSON MUST contain `"type": "end_project"`, `"type": "restart_scene"`, and `"type": "goto_scene"` respectively

#### Scenario: Unknown fail-action type raises a clear error

- **WHEN** a project JSON contains a `failAction` with `"type": "does_not_exist"`
- **THEN** `from_json` on `MissionFailAction` MUST throw an exception whose message names the unknown type string

### Requirement: Legacy projects load unchanged

Existing project JSON files that predate this change MUST continue to load. Legacy projects fall into two cases:

1. **No mission objectives** — load and run identically. `MissionModeState.active` remains `false`, `failCurrentProject()` falls through to `stopCurrentProject()`, `OnAMissionFlag` is never written.
2. **v1 mission objectives with `failAction = { type: "end_project", data: {} }`** — load with `EndProject{ fadeOut = true }` (the new default applied because the JSON does not include the `fadeOut` field). The runtime fail behavior gains a fade-out before the project stop. Authors who want the v1 visual (no fade) toggle the new `Fade out` checkbox to off.

#### Scenario: Project with no mission objectives behaves as before

- **WHEN** a legacy project (no `core.mission_start` / `core.mission_end`) is loaded and played
- **THEN** the runtime fail behavior MUST be identical to pre-change behavior
- **AND** `CTheScripts::OnAMissionFlag` MUST remain `0` throughout the run
- **AND** no fail banner MUST appear on any fail signal

#### Scenario: v1 EndProject-only mission gains fade-out by default

- **WHEN** a v1-saved project containing `core.mission_start` with `failAction = { type: "end_project", data: {} }` is loaded
- **THEN** the in-memory `failAction` MUST equal `EndProject{ fadeOut = true }`
- **AND** the next fail signal MUST run the fail sequence with banner → fade-out → stop → fade-in
- **AND** the project on save MUST emit `"data": { "fadeOut": true }` for the action
