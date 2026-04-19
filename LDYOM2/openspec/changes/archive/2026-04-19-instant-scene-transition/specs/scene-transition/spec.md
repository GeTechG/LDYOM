## ADDED Requirements

### Requirement: Default animated scene transition

The project player SHALL play a cinematic fade-out, brief wait, and fade-in whenever a scene-switch objective runs without an explicit instant opt-out. This preserves the existing 500ms fade-out + 600ms wait + fade-in behavior as the default for all scene swaps.

#### Scenario: Default fade transition

- **WHEN** a `goto_scene` objective runs with `instantTransition = false`
- **THEN** the project player MUST fade the screen to black over ~500ms, wait ~600ms, load the target scene, and then fade back in on the next objective

#### Scenario: Default fade on conditional scene switch

- **WHEN** a `goto_scene_if_var` objective resolves to a target scene with `instantTransition = false`
- **THEN** the project player MUST apply the same default fade-out/wait/fade-in sequence as `goto_scene`

### Requirement: Per-objective instant transition opt-out

`goto_scene` and `goto_scene_if_var` objective Data structs SHALL expose an `instantTransition` boolean field that defaults to `false`. When set to `true`, the project player MUST skip the fade-out/wait block and MUST leave the `isFaded` runtime flag clear so the next scene's first objective does not force a fade-in.

#### Scenario: Instant transition skips fade

- **WHEN** a `goto_scene` objective runs with `instantTransition = true` and the screen is currently visible
- **THEN** the project player MUST switch scenes without any fade-out, wait, or fade-in animation

#### Scenario: Instant transition preserves current visibility state

- **WHEN** a `goto_scene` objective runs with `instantTransition = true` and the screen is already faded to black from a prior cutscene
- **THEN** the project player MUST NOT add an extra fade and MUST NOT force a fade-in on the next objective, leaving visibility exactly as it was

#### Scenario: Instant flag is per-call

- **WHEN** two `goto_scene` objectives in the same project have different `instantTransition` values
- **THEN** each objective MUST honor its own flag independently without affecting other scene-switch sites

### Requirement: Backward-compatible JSON deserialization

The objective Data deserializers SHALL load existing project files that predate this change by defaulting `instantTransition` to `false` when the key is absent.

#### Scenario: Legacy project loads without instantTransition key

- **WHEN** a project JSON containing a `goto_scene` or `goto_scene_if_var` objective without an `instantTransition` field is loaded
- **THEN** deserialization MUST succeed and the objective's `instantTransition` field MUST be `false`

#### Scenario: Legacy project preserves default behavior at runtime

- **GIVEN** a legacy project loaded with no `instantTransition` field
- **WHEN** the scene-switch objective runs
- **THEN** the player MUST execute the default fade-out/wait/fade-in transition

### Requirement: Lua binding exposes instant flag

The `requestSceneTransition` Lua binding on `ProjectPlayer` SHALL accept an optional second boolean argument `instant` that defaults to `false`. The underlying C++ `ProjectPlayer::requestSceneTransition` SHALL carry this flag so Lua callers control the transition style identically to the objective Data field.

#### Scenario: Lua instant transition call

- **WHEN** Lua code calls `projectPlayer:requestSceneTransition("sceneId", true)`
- **THEN** the project player MUST switch scenes without the fade-out/wait/fade-in animation

#### Scenario: Lua backward-compatible single-argument call

- **WHEN** existing Lua code calls `projectPlayer:requestSceneTransition("sceneId")` with no instant argument
- **THEN** the call MUST succeed and MUST behave identically to passing `instant = false` (default fade)

#### Scenario: Lua declaration reflects optional flag

- **WHEN** a script consumes the core Lua declaration files for `requestSceneTransition`
- **THEN** the signature MUST document the optional `instant: boolean` parameter with default `false`

### Requirement: Editor surfaces instant option with artifact warning

The ImGui editor panels for `goto_scene` and `goto_scene_if_var` SHALL render an "Instant transition" checkbox bound to `instantTransition`. A hover tooltip SHALL warn users about visible artifacts that can occur when fade is skipped.

#### Scenario: Checkbox renders in objective editor

- **WHEN** the user opens a `goto_scene` or `goto_scene_if_var` objective in the editor
- **THEN** an "Instant transition" checkbox MUST be visible and MUST reflect the current `instantTransition` value

#### Scenario: Tooltip warns about artifacts

- **WHEN** the user hovers over the "Instant transition" checkbox
- **THEN** a tooltip MUST appear listing the visible artifacts (entity pop-in, camera jumpcut, weather/time snap, scene name banner) that may occur when skipping fade

#### Scenario: Localized label and tooltip

- **WHEN** the editor renders the checkbox label and tooltip
- **THEN** both strings MUST be sourced from localization keys defined in `../ingame/languages/en.json` rather than hard-coded literals
