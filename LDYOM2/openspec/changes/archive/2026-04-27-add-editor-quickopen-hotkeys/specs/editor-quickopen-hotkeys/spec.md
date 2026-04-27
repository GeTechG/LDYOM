## ADDED Requirements

### Requirement: Two global hotkeys for the entity and objective editors

The `Hotkeys` system SHALL register two new bindings, `openEntitiesEditor` (default `P`) and `openObjectivesEditor` (default `O`), at startup. Both bindings SHALL appear in the existing hotkey-editor popup driven by `Hotkeys::getHotkeys()` and SHALL be persisted to and restored from `settings.json` under the `hotkeys.openEntitiesEditor` and `hotkeys.openObjectivesEditor` keys via the same load-or-default loop in `Hotkeys::initialize()` as the existing five bindings.

#### Scenario: Defaults are applied on first run

- **WHEN** the project starts and `settings.json` does not contain `hotkeys.openEntitiesEditor`
- **THEN** the binding's `functionKeys` MUST be set to the scancode-packed value for `P`
- **AND** the next save cycle MUST write `hotkeys.openEntitiesEditor` to `settings.json` with that value

- **WHEN** the project starts and `settings.json` does not contain `hotkeys.openObjectivesEditor`
- **THEN** the binding's `functionKeys` MUST be set to the scancode-packed value for `O`
- **AND** the next save cycle MUST write `hotkeys.openObjectivesEditor` to `settings.json` with that value

#### Scenario: User-set bindings round-trip through settings

- **WHEN** the user opens the hotkey-editor popup, rebinds `openEntitiesEditor` to `F3`, and presses `Set`
- **THEN** `settings.json` MUST contain `hotkeys.openEntitiesEditor = "F3"` after the next save
- **AND** restarting the application MUST restore `F3` as the active combination for `openEntitiesEditor`

#### Scenario: Bindings are visible in the editor popup

- **WHEN** the hotkey-editor popup is opened
- **THEN** its left-panel list MUST include rows for `openEntitiesEditor` and `openObjectivesEditor`
- **AND** each row MUST render the function's localized name on line one and the current binding preview on line two (per the existing two-line-row requirement of `hotkey-editor`)

### Requirement: Toggle-to-target window behavior

When `openEntitiesEditor` fires, if `WindowManager::isWindowOpen("entities")` is `true` the system SHALL call `WindowManager::closeWindow("entities")` and SHALL NOT touch any other window. Otherwise the system SHALL call `WindowManager::closeAllWindows()` followed by `WindowManager::openWindow("entities")`. The same logic SHALL apply to `openObjectivesEditor` against the `"objectives"` window id. Symmetric behavior is possible because the default `P` does not collide with the `EntityOrbitCamera`'s WASDQE movement bindings (an earlier draft used bare `E` and had to ship asymmetric "open-only" semantics for entities; that workaround is removed now that the default is `P`, which the orbit camera never reads).

#### Scenario: First press opens the entities editor

- **WHEN** no editor windows are open and the user presses the `openEntitiesEditor` combination
- **THEN** `WindowManager::isWindowOpen("entities")` MUST become `true`
- **AND** no other window's open state MUST change

#### Scenario: Second press closes the entities editor

- **WHEN** the entities window is open and the user presses `openEntitiesEditor` again
- **THEN** `WindowManager::isWindowOpen("entities")` MUST become `false`
- **AND** every other window's open state MUST be unchanged

#### Scenario: Press while a different editor is open replaces it

- **WHEN** the `objectives` window is open, the `entities` window is closed, and the user presses `openEntitiesEditor`
- **THEN** `WindowManager::isWindowOpen("objectives")` MUST become `false`
- **AND** `WindowManager::isWindowOpen("entities")` MUST become `true`
- **AND** any other open window MUST also be closed by the same action

#### Scenario: Objectives hotkey behaves symmetrically

- **WHEN** the user presses `openObjectivesEditor` under any of the three states above (closed, open, replacing another editor)
- **THEN** the resulting state of the `objectives` window MUST follow the same toggle/replace rules applied to `entities`

### Requirement: Gating during playback and without a project

`openEntitiesEditor` and `openObjectivesEditor` SHALL no-op when `ProjectPlayer::isPlaying()` is `true`, and SHALL no-op when `ProjectsManager::getCurrentProjectIndex() == -1`. A no-op means no window state changes anywhere — neither opening, closing, nor replacing.

#### Scenario: Hotkey is ignored during project playback

- **WHEN** `ProjectPlayer::isPlaying()` is `true` and the user presses `openEntitiesEditor`
- **THEN** the open/closed state of every window managed by `WindowManager` MUST be unchanged after the press

- **WHEN** `ProjectPlayer::isPlaying()` is `true` and the user presses `openObjectivesEditor`
- **THEN** the open/closed state of every window managed by `WindowManager` MUST be unchanged after the press

#### Scenario: Hotkey is ignored when no project is loaded

- **WHEN** `ProjectsManager::getCurrentProjectIndex() == -1` and the user presses `openEntitiesEditor`
- **THEN** the entities window MUST NOT open

- **WHEN** `ProjectsManager::getCurrentProjectIndex() == -1` and the user presses `openObjectivesEditor`
- **THEN** the objectives window MUST NOT open

### Requirement: Window-local Alt+digit shortcuts switch entity mode

When the `EntitiesWindow` is open and ImGui is not capturing text input, pressing the chord `Alt+1` through `Alt+9` (with no other modifier held) SHALL set `m_windowType` to the corresponding `EntitiesWindowType` value using the mapping `Alt+1=Actor`, `Alt+2=Vehicle`, `Alt+3=Train`, `Alt+4=Object`, `Alt+5=Pickup`, `Alt+6=Firework`, `Alt+7=Particle`, `Alt+8=Checkpoint`, `Alt+9=Audio`. After the switch the window SHALL reset `m_selectedEntityIndex` to `-1` so the entity list re-renders for the new mode without a stale selection, and so any active `EntityOrbitCamera` / `EntityGizmo` for the previously selected entity is deactivated. Bare digit keys (without `Alt`) SHALL NOT switch modes — they remain reserved for the in-game `EntityGizmo` operation toggle (`1`=TRANSLATE, `2`=ROTATE). The chord SHALL fire regardless of which sub-window currently has keyboard focus (entities list, the sibling `EntityInfoPanel`, or the in-game viewport), since these are part of the same editing context.

#### Scenario: Pressing Alt+2 switches to Vehicle mode

- **WHEN** the entities window has focus, no item is active, and the user presses `Alt+2`
- **THEN** `m_windowType` MUST become `EntitiesWindowType_Vehicle`
- **AND** `m_selectedEntityIndex` MUST be reset to `-1`
- **AND** the next render MUST list only entities whose `hasComponent("vehicle")` returns `true`

#### Scenario: Bare digit keys do not switch modes

- **WHEN** the entities window has focus, an entity is selected (so `EntityGizmo` is active), and the user presses bare `2` (no `Alt`)
- **THEN** `m_windowType` MUST remain at its previous value
- **AND** `EntityGizmo::m_currentOperation` MAY change to `ROTATE` per the gizmo's existing bare-digit binding

#### Scenario: Alt+digit shortcuts are inert while typing into a field

- **WHEN** the entities window contains a focused `InputText` (rename buffer or similar) and the user presses `Alt+5`
- **THEN** `m_windowType` MUST remain at its previous value
- **AND** the focused text field MUST receive whatever ImGui delivers for the chord (typically nothing — `Alt+digit` is not a printable character)

#### Scenario: Alt+digit shortcuts are inert while a `DragInt` is being dragged

- **WHEN** the user is mid-drag on a `DragInt` widget inside an entity component editor and presses `Alt+1`
- **THEN** `m_windowType` MUST remain at its previous value
- **AND** the drag widget MUST continue receiving its drag input

#### Scenario: Pressing Alt+digit for the current mode is a no-op

- **WHEN** `m_windowType` is already `EntitiesWindowType_Actor` and the user presses `Alt+1`
- **THEN** `m_windowType` MUST remain `EntitiesWindowType_Actor`
- **AND** the existing selection MAY be preserved (resetting to `-1` is permitted but not required when the mode is unchanged)

#### Scenario: Alt+digit fires while the sibling info panel has focus

- **WHEN** an entity is selected, `EntityInfoPanel` is open and has keyboard focus, and the user presses `Alt+2`
- **THEN** `m_windowType` MUST become `EntitiesWindowType_Vehicle`
- **AND** `m_selectedEntityIndex` MUST be reset to `-1`
- **AND** the previously active `EntityOrbitCamera` / `EntityGizmo` for the deselected entity MUST be deactivated (the player view restored, gizmo cleared, info panel hidden)

#### Scenario: Alt+digit does nothing when the entities window is closed

- **WHEN** the entities window is closed (not rendering) and the user presses `Alt+2`
- **THEN** `m_windowType` MUST be unchanged
- **AND** opening the entities window afterwards MUST land on the previously stored mode, not on Vehicle

### Requirement: Last-used entity mode and selection persist within a session

The `EntitiesWindow` instance SHALL retain both its `m_windowType` value and its `m_selectedEntityIndex` value across `close()` and reopen calls within a single application run, mirroring the implicit persistence already provided by `ObjectivesWindow` for `m_selectedObjectiveIndex`. The `Window::open()` override on `EntitiesWindow` MUST NOT reset `m_windowType` or `m_selectedEntityIndex` to default values; instead, when reopening with a previously-saved selection, it SHALL re-activate the `EntityOrbitCamera` and `EntityInfoPanel` for that entity (since `close()` torn them down). If the saved `m_selectedEntityIndex` is no longer valid (e.g. the entity was removed while the window was closed), `setSelectedEntityIndex` SHALL normalize the field back to `-1` and leave the info panel hidden.

#### Scenario: Reopening the editor lands on the previous mode

- **WHEN** the user opens the entities editor, switches to Vehicle mode (via `Alt+2` or via main-menu category buttons), closes the editor, then presses `openEntitiesEditor`
- **THEN** the entities window MUST open with `m_windowType = EntitiesWindowType_Vehicle`

#### Scenario: Reopening the editor restores the previously selected entity

- **WHEN** the user opens the entities editor, selects entity at index `N` (so `EntityOrbitCamera` and `EntityInfoPanel` activate for it), closes the editor (via `E` or via the close button), then presses `openEntitiesEditor`
- **THEN** the entities window MUST open with `m_selectedEntityIndex = N`
- **AND** the `EntityOrbitCamera` MUST be re-activated targeting the same entity
- **AND** the `EntityInfoPanel` MUST be visible for the same entity

#### Scenario: First open of the session lands on Actor with no selection

- **WHEN** the application has just started and the user presses `openEntitiesEditor` without ever having opened the entities editor in this session
- **THEN** the entities window MUST open with `m_windowType = EntitiesWindowType_Actor`
- **AND** `m_selectedEntityIndex` MUST be `-1` (no entity selected, no orbit camera, no info panel)

#### Scenario: Reopening with a stale selection normalizes to no selection

- **WHEN** the user selects entity at index `N`, closes the editor, deletes that entity through some other path so the entities vector now has fewer than `N+1` entries, then presses `openEntitiesEditor`
- **THEN** `m_selectedEntityIndex` MUST be normalized to `-1`
- **AND** `EntityOrbitCamera` MUST NOT be activated
- **AND** the info panel MUST remain hidden
