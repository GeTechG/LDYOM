# onboarding-flow Specification

## Purpose
TBD - created by archiving change add-onboarding-flow. Update Purpose after archive.
## Requirements
### Requirement: First-run gate redirects openEditor to Welcome

When the `openEditor` hotkey is pressed, the editor SHALL inspect runtime state and the `onboarding.completed` setting before deciding which window to open. If, and only if, no project is currently loaded (`ProjectsManager::currentProjectIndex == -1`) AND `Settings::getSetting<bool>("onboarding.completed", false)` returns `false`, the system MUST open the `welcome` window. In all other situations the existing `project_manager` window MUST be opened, preserving today's behavior. No other entry point (menu actions, Lua, hotkeys other than `openEditor`) SHALL be modified.

#### Scenario: First-run user opens editor with no project loaded

- **WHEN** `currentProjectIndex == -1`
- **AND** `onboarding.completed` is `false` (or absent from `settings.json`)
- **AND** the user presses the `openEditor` hotkey
- **THEN** the `welcome` window MUST be opened
- **AND** the `project_manager` window MUST NOT be opened by this hotkey press

#### Scenario: Returning user with completed onboarding bypasses Welcome

- **WHEN** `onboarding.completed` is `true`
- **AND** `currentProjectIndex == -1`
- **AND** the user presses the `openEditor` hotkey
- **THEN** the `project_manager` window MUST be opened
- **AND** the `welcome` window MUST NOT be opened

#### Scenario: User with project loaded bypasses Welcome regardless of flag

- **WHEN** `currentProjectIndex != -1` (a project is loaded)
- **AND** `onboarding.completed` is `false`
- **AND** the user presses the `openEditor` hotkey
- **THEN** the `project_manager` window MUST be opened
- **AND** the `welcome` window MUST NOT be opened

### Requirement: Welcome window content and layout

The `Welcome` window SHALL be a non-modal `Window` subclass at approximately 600x400 pixels (medium size), shown as a single screen. It SHALL render, in order: an intro paragraph, a short choice-explanation paragraph, and exactly two preset cards labeled **Simple** and **Advanced**. Each preset card SHALL display its label and a short description. The window SHALL provide no other navigation controls (no skip, no "later", no direct link to project_manager).

#### Scenario: Welcome shows intro and exactly two preset cards

- **WHEN** the `welcome` window is open
- **THEN** it MUST display intro copy from the `welcome.intro` localization key
- **AND** it MUST display preset-choice explanation from the `welcome.preset_choice_explain` localization key
- **AND** it MUST display exactly two clickable preset cards: one labeled by `welcome.preset.simple.label` with description `welcome.preset.simple.description`, and one labeled by `welcome.preset.advanced.label` with description `welcome.preset.advanced.description`
- **AND** it MUST NOT display any "Skip", "Later", or direct "Open Project Manager" control

### Requirement: Preset selection writes editor defaults

When the user clicks a preset card, the system SHALL write the corresponding values to settings via `Settings::setSetting<bool>` and persist them. The **Simple** preset SHALL set `editor.entities.auto_attach_objective_specific = true` and `editor.entities.auto_link_spawn_objective = true`. The **Advanced** preset SHALL set both keys to `false`. After writing the preset, the system MUST close the `welcome` window and open the `quick_settings` window in onboarding mode.

#### Scenario: Simple preset enables both entity-creation toggles

- **WHEN** the user clicks the Simple preset card in `welcome`
- **THEN** `Settings::setSetting<bool>("editor.entities.auto_attach_objective_specific", true)` MUST be invoked
- **AND** `Settings::setSetting<bool>("editor.entities.auto_link_spawn_objective", true)` MUST be invoked
- **AND** the `welcome` window MUST be closed
- **AND** the `quick_settings` window MUST be opened in onboarding mode

#### Scenario: Advanced preset disables both entity-creation toggles

- **WHEN** the user clicks the Advanced preset card in `welcome`
- **THEN** `Settings::setSetting<bool>("editor.entities.auto_attach_objective_specific", false)` MUST be invoked
- **AND** `Settings::setSetting<bool>("editor.entities.auto_link_spawn_objective", false)` MUST be invoked
- **AND** the `welcome` window MUST be closed
- **AND** the `quick_settings` window MUST be opened in onboarding mode

### Requirement: QuickSettings onboarding mode

`QuickSettings` SHALL carry an `m_onboardingMode` boolean flag, default `false`. When the flag is `true`, the window SHALL render a "Continue" button (text from localization key `quick_settings.continue`) in place of the normal "Close" button. Clicking "Continue" SHALL set `onboarding.completed = true` via `Settings::setSetting<bool>`, close the `quick_settings` window, clear the onboarding-mode flag (so a subsequent normal open does not reuse it), and open the `project_manager` window. When the flag is `false`, `QuickSettings` SHALL behave exactly as today (no spec change to non-onboarding behavior).

#### Scenario: Continue button completes onboarding and opens project_manager

- **WHEN** `quick_settings` is open with `m_onboardingMode = true`
- **AND** the user clicks the "Continue" button
- **THEN** `Settings::setSetting<bool>("onboarding.completed", true)` MUST be invoked
- **AND** `quick_settings` MUST be closed
- **AND** `project_manager` MUST be opened
- **AND** the next press of `openEditor` (with no project loaded) MUST open `project_manager` directly (Welcome MUST NOT reappear)

#### Scenario: Non-onboarding open shows Close, not Continue

- **WHEN** `quick_settings` is opened from any path other than the Welcome flow (so `m_onboardingMode == false`)
- **THEN** the bottom button MUST be the existing "Close" button
- **AND** clicking it MUST NOT modify `onboarding.completed`
- **AND** clicking it MUST NOT open `project_manager`

### Requirement: Welcome dismissal is non-resumable

Closing the `welcome` window via Esc or the window-frame close button (X) without selecting a preset SHALL leave `onboarding.completed = false` and SHALL NOT persist any partial-progress state. The next press of `openEditor` (with no project loaded) MUST reopen `welcome` from scratch with no preset preselected.

#### Scenario: Closing Welcome via X reopens it next time

- **WHEN** the `welcome` window is open and the user presses Esc or clicks X without picking a preset
- **THEN** `onboarding.completed` MUST remain `false`
- **AND** no preset SHALL have been written to `editor.entities.*` keys by the dismissal itself
- **AND** the next `openEditor` press (with no project loaded) MUST reopen `welcome`
- **AND** the reopened `welcome` MUST NOT highlight or pre-select any preset

#### Scenario: Reselecting after dismissal overwrites prior intent

- **WHEN** the user previously dismissed `welcome` after hovering preset cards but never clicking one
- **AND** the user reopens `welcome` and clicks a different preset than they were considering
- **THEN** the clicked preset's values MUST be the ones written to settings
- **AND** no remnant of the prior session MUST influence the written values

### Requirement: No skip path through Welcome

The `welcome` window SHALL provide no path to `project_manager` other than the full flow: preset selection, then `quick_settings` in onboarding mode, then "Continue". There SHALL be no button, hotkey, or alternative gesture that opens `project_manager` while leaving `onboarding.completed = false`.

#### Scenario: No control bypasses the preset and Continue steps

- **WHEN** the `welcome` window is rendered
- **THEN** the only controls leading to `project_manager` MUST be the two preset cards (which route through `quick_settings`)
- **AND** there MUST NOT be a "Skip" or "Open Project Manager" control
- **AND** dismissing `welcome` (Esc/X) MUST NOT open `project_manager`

### Requirement: Welcome localization keys

All user-visible strings on the `welcome` window and the onboarding-mode "Continue" button SHALL be served via the project's `_(...)` localization helper, never hard-coded English. The keys defined for this flow SHALL be: `welcome.title`, `welcome.intro`, `welcome.preset_choice_explain`, `welcome.preset.simple.label`, `welcome.preset.simple.description`, `welcome.preset.advanced.label`, `welcome.preset.advanced.description`, and `quick_settings.continue`.

#### Scenario: All Welcome strings come from localization

- **WHEN** the `welcome` window renders
- **THEN** every visible label, paragraph, and card description MUST be obtained via the localization helper using one of the keys listed above
- **AND** the onboarding-mode `quick_settings` button text MUST be obtained via `quick_settings.continue`
- **AND** no string source code on this code path MAY contain a hard-coded user-facing English literal
