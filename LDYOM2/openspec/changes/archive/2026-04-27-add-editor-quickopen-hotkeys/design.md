## Context

LDYOM2's input layer has two independent hotkey systems:

1. `Hotkeys` (`src/core/hotkeys.{h,cpp}`) — a flat list of `(name, keyCombo, callback)` registered at startup, persisted to `settings.json`, and editable via the hotkey-editor popup. Used for global, mission-author–facing shortcuts (`I`, `F4`, `F5`, `F6`, `Ctrl+S` today). Detection runs every frame in `Hotkeys::update()` and gated on `!ImGui::GetIO().WantTextInput`.
2. `WindowManager::registerHotkey` — a per-window-open shortcut map keyed on `HotkeyInfo{ImGuiKey, ctrl, alt, shift}`. Maps a key chord to a single window id and opens that window. Not editable, not persisted.

The two existing surfaces this change targets are:

- `EntitiesWindow` (`src/rendering/windows/entities.{h,cpp}`) — single window with a runtime `m_windowType` of nine values that filters which entities are shown and what `addNewEntityFromTemplate` is called with. Mode is set externally via `setWindowType()` (called from the main menu's category buttons).
- `ObjectivesWindow` (`src/rendering/windows/objectives.{h,cpp}`) — single mode-less window listing the current scene's objectives.

Today neither editor has a direct shortcut. The closest precedent is `openEditor` (`I`) which opens `project_manager` or `main_menu` depending on whether a project is loaded, and which closes everything if any window is already open.

The user wants two new shortcuts, one per editor, with the entities editor sub-divided by mode. Two design pressures collide: keymap real-estate (nine entity modes is too many global keys) versus immediacy (the user does not want to navigate a popup to pick a mode). The two-tier hybrid resolves this: one global hotkey gets you to the editor, then digit keys switch modes once you are there.

## Goals / Non-Goals

**Goals:**

- Two new global hotkeys, default `E` and `O`, opening the entities and objectives editors respectively in a single keystroke.
- Window-local `Alt+1`…`Alt+9` switch `EntitiesWindow` between its nine modes when the window is focused.
- Toggle behavior: pressing the same hotkey while its target editor is open closes that editor.
- All new bindings appear in the existing hotkey-editor popup and are user-rebindable through it.
- Both global hotkeys gracefully no-op during project playback and when no project is loaded.
- Zero new code paths in `Hotkeys`, `WindowManager`, or any other manager — the change is registration plus callback wiring plus a small input handler in `EntitiesWindow`.

**Non-Goals:**

- No persistence of the last-used `EntitiesWindowType` or `m_selectedEntityIndex` across game restarts. Both fields already persist across open/close *within a session* by virtue of the `EntitiesWindow` instance being long-lived. Cross-session persistence would require new `Settings` keys and a load/save handshake — deferred until users complain.
- No quick-pick popup (variant B from the explore phase). The hybrid was chosen instead; if users find digit keys insufficient (e.g. they cannot remember the number-to-mode mapping), a `Shift+E` popup can be added in a follow-up change without touching the v1 surface.
- No global digit hotkeys (e.g. `Shift+1..9` to switch modes from anywhere). Modes are only switchable while the entities editor has focus. Global digits would conflict with future bindings and with text-input fields.
- No multi-key chords (`E,a` → Add Actor, etc.). The current `Hotkeys::stringToKeyCombo` parses a `+`-joined combo into a packed scancode; sequence support would require a separate state machine in `Hotkeys::update()`. Out of scope.
- No changes to `openEditor`'s (`I`) behavior. The new hotkeys live alongside it.
- No changes to the `WindowManager::registerHotkey` per-window hotkey path; the new bindings go through `Hotkeys` because they need playback gating, project-loaded gating, and the toggle logic, none of which `WindowManager::processHotkeys` provides.

## Decisions

### Two hotkeys (`E`, `O`), not one with a popup

Three viable shapes were on the table during exploration:

- **A — nine global hotkeys, one per mode.** Rejected: pollutes the keymap, conflicts with future bindings, and makes the hotkey-editor list nine entries longer for a feature most users will operate on three or four modes.
- **B — one hotkey, opens a quick-pick popup listing the nine modes.** Rejected for v1: adds a new ImGui surface and an extra keystroke for the common case (open + pick). Salvageable as a follow-up if users ask.
- **C — one hotkey opens the editor in last-used mode; digit keys switch modes inside.** This is what we ship.

The user's framing during exploration was explicit: "один хоткей для целей, другой для сущностей, но нужно как разделить у сущностей" — one hotkey for objectives, one for entities, with the entities side needing a sub-divider. Variant C's window-local digits *are* that sub-divider, but they live inside the editor instead of inside a popup, which means zero new UI and zero extra keystrokes once the user lands on the editor.

### Toggle behavior: target-window toggle, not global "close all"

`openEditor` (`I`) treats any open window as "the editor is up" and closes everything. That makes sense for `I` because `I` opens whichever menu is contextually right — you do not know what is up, you just want the editor visible or hidden.

For the entities/objectives hotkeys we know exactly which window is the target. Three toggle shapes were considered:

- **(a)** If any window is open → close all; else → open target. (Mirrors `I`.)
- **(b)** If target window is open → close it; else → open target without touching others.
- **(c)** If target window is open → close it; else → close everything else and open target.

Shape (a) is wrong: pressing the entities hotkey while `objectives` is open would close `objectives` without ever opening `entities`. The user would press twice and end up on entities, but the first press is a wasted action.

Shape (b) leaves the previous editor open behind the new one. `WindowManager` allows multiple open windows (its API is per-id), but the editor surfaces visually overlap and behavior would be confusing — pressing the entities hotkey while on `objectives` would stack two editors.

Shape (c) is what we ship for both hotkeys. Pressing the entities hotkey always lands on entities. Pressing it again cleanly closes. Same for objectives. The "close everything else" half is one call to `WindowManager::closeAllWindows()` followed by `openWindow(...)` — consistent with how the existing main-menu category buttons behave (they close the menu and open the next surface).

The "everything else" being closed includes `project_manager`, `main_menu`, `scenes`, `addons_settings`, etc. — all of which are valid editor surfaces but not co-occupants with the entities/objectives editors today. If a future feature wants two windows visible at once, it can call the `WindowManager` API directly; the hotkey path is for "I want to see this *one* editor right now."

An earlier draft used bare `E` as the entities default and had to ship an asymmetric "open-only" variant (no close on second press) because `EntityOrbitCamera` binds bare `E` to "move entity / camera down" via WASDQE — pressing `E` would both fire the hotkey and nudge the selected entity downward in the same frame. The current default is `P`, a letter the orbit camera never reads, which restores the symmetric toggle. If a user manually rebinds `openEntitiesEditor` back to a movement-conflicting key (`W`, `A`, `S`, `D`, `Q`, or `E`), the conflict re-emerges; this is documented but not blocked.

### Window-local `Alt+digit` shortcuts, not bare digits

Mode switching lives inside `EntitiesWindow::renderContent` and is gated on:

```cpp
io.KeyAlt && !io.KeyCtrl && !io.KeyShift && !io.KeySuper && !io.WantTextInput
```

The handler is implicitly scoped to "the entities window is open and rendering" (it lives inside `renderContent`, which only runs while the window is rendered). We deliberately do NOT add `IsWindowFocused` or `IsAnyItemActive` checks: `EntityInfoPanel` is a separate sibling ImGui window (`ImGui::Begin("##EntityInfoPanel", ...)` in `entity_info_panel.cpp:63`), and as soon as the user clicks the info panel, drags a slider, or interacts with the in-game viewport (gizmo manipulator), focus leaves the entities window. With a focus gate the chord would silently no-op in exactly the situation where the user wants it most — switching tabs while editing an entity. The implicit "window is rendering" scope is enough; the explicit Alt-modifier requirement keeps the chord from interfering with text input or other windows.

The chord is detected via an explicit `io.KeyAlt && !io.KeyCtrl && !io.KeyShift && !io.KeySuper && ImGui::IsKeyPressed(ImGuiKey_N, false)` check rather than `ImGui::IsKeyChordPressed(ImGuiMod_Alt | ImGuiKey_N)`. The chord-helper relies on ImGui's shortcut-routing system (`Shortcut`), which applies a `RouteFocused` priority that can swallow the first matching frame when input focus is in flux — observed during testing as "the first Alt+digit press does nothing, the second one works." The explicit IO-flag check sidesteps the routing layer entirely and fires reliably on the first press. The `!io.KeyCtrl/KeyShift/KeySuper` clauses keep the chord pure-Alt so future bindings (e.g. `Ctrl+Alt+digit`) do not accidentally trigger mode switches.

#### Why `Alt+digit` instead of bare digits

Bare digits `1`–`9` collide with `EntityGizmo`'s in-game manipulator (`src/rendering/in_game/entity_gizmo.cpp:36-39`), which already binds bare `1` and `2` to `TRANSLATE` and `ROTATE`. The gizmo runs whenever an entity is selected — exactly the state the user is in while editing — so bare-digit mode switching would ride on top of every gizmo operation toggle. We route mode switching through `Alt+digit` to keep the gizmo's bare-digit convention (which mirrors Blender / Maya / Unity) intact, and add a complementary `!io.KeyAlt` guard inside `EntityGizmo::render` so that pressing `Alt+1` while the gizmo is active does not also fire `TRANSLATE`. `Alt+digit` is the established tab-switch convention in browsers and IDEs, so the discoverability cost is mild.

Why local instead of registering nine entries in `Hotkeys`:

- `Hotkeys::update()` runs every frame regardless of which window is up. A global `Alt+1` binding would fire when the user uses `Alt+1` for anything outside the entities editor. Local handling scopes the chord to the editor surface where it makes sense.
- Local shortcuts cost zero entries in the user-facing hotkey-editor list. Nine fixed mode bindings would clutter the popup for a feature that is essentially "second-level navigation."
- Mapping is implicit and discoverable via the editor's existing tab UI (the main-menu category buttons already enumerate modes in the `1..9` order — Actor first, Audio last). The same order is used here so that muscle memory transfers from clicks to keys.

The mapping is hardcoded to the `EntitiesWindowType` enum order. If the enum gains a tenth value the digit beyond `9` is silently unsupported until either a `0` slot is added or the design is revisited.

### Restore last-edited entity on reopen (symmetric with objectives)

`ObjectivesWindow` provides implicit selection persistence: it has no `open()` override and never resets `m_selectedObjectiveIndex`, so reopening the objectives editor lands on whatever was selected last. `EntitiesWindow::open()` originally did the opposite — `setSelectedEntityIndex(-1)` — because the editor's heavyweight side-effects (`EntityOrbitCamera` and `EntityInfoPanel`) are not idempotent across the close/open boundary: `close()` deactivates them, and a no-op `open()` would not bring them back even if `m_selectedEntityIndex` were preserved.

To match the objectives behavior the override now persists `m_selectedEntityIndex` across close/reopen and, when reopening with a saved selection, re-fires `setSelectedEntityIndex(m_selectedEntityIndex)` to re-activate the orbit camera and info panel. `setSelectedEntityIndex` is hardened to normalize out-of-range indices to `-1` (covering the case where entities were removed while the window was closed) so the restore path can never land in a "selected-but-not-actually-selected" state. First-open-of-session is unchanged: `m_selectedEntityIndex` defaults to `-1`, the restore branch short-circuits, and the editor lands with no selection — same baseline as before.

### Hotkey defaults: `P` and `O`

`O` is the first letter of "Objectives" — mnemonic and unconflicted.

`P` is the entities default. The original choice was `E` (mnemonic for "Entities"), but `E` is read by `EntityOrbitCamera::handleInput` (`src/rendering/in_game/entity_orbit_camera.cpp:437,480`) every frame an entity is selected, as the "move down" axis of the WASDQE manipulator (alongside `Q`=up, `W/A/S/D`=horizontal). The hotkey system fires on the press transition, but the orbit camera reads `IsKeyDown(ImGuiKey_E)` continuously; pressing `E` to open or close the editor would also nudge the currently selected entity downward for the few frames the key remained physically held. Adding a `!io.KeyCtrl/KeyShift/...` gate to the orbit camera does not help (the user is pressing bare `E`, no modifier).

`P` was chosen as a free letter key that does not appear in the `EntityOrbitCamera`'s movement set (`W`, `A`, `S`, `D`, `Q`, `E`), the `EntityGizmo`'s digit set (`1`, `2`, `G`), or any of the existing six `Hotkeys` bindings (`I`, `F4`, `F5`, `F6`, `Ctrl+S`, `O`). The orbit camera and gizmo never read `P`, so pressing the hotkey does not bleed into entity movement or gizmo operations. Users who want a different default (e.g. `F2` for a function-key style consistent with `F4`/`F5`/`F6`, or rebind back to `E` despite the movement collision) can do so through the hotkey-editor popup; the rebind path is unchanged.

Conflict check against GTA San Andreas: GTA SA's default `P` binding opens the pause/map menu in single-player gameplay. When the editor is closed and the user is mid-gameplay, pressing `P` will both fire the LDYOM hotkey (open entities editor, gated by playback / project-loaded) and GTA's native pause. This is the same mid-gameplay-overlap pattern already established by `I` (LDYOM main menu) and is acceptable — users in active gameplay typically have no project loaded or are in playback mode, both of which gate the hotkey to a no-op.

The scancode literals follow the existing pattern in `Hotkeys::initialize()`: `0xFFFFFFxx` where `xx` is the scancode-page-1 value from `ImHotKey::Keys[]` (`0xFFFFFF19` for `P`, `0xFFFFFF18` for `O`).

### Why register through `Hotkeys` and not `WindowManager`

`WindowManager::registerHotkey` already supports "press a key to open a window." It would be one line per binding. But it lacks:

- Playback gating (`ProjectPlayer::isPlaying()`)
- Project-loaded gating (`ProjectsManager::getCurrentProjectIndex() == -1`)
- Toggle behavior (it only opens, never closes)
- User rebinding through the hotkey-editor popup
- Persistence across sessions

Re-implementing those four features in the `WindowManager` path would duplicate `Hotkeys`. Going through `Hotkeys` reuses the entire ladder for free. The `WindowManager` hotkey path remains useful for non-editor windows that want a fixed dev-only chord, but it is the wrong surface for user-facing, rebindable, gated bindings.

## Risks / Trade-offs

- **`E` / `O` are common letters.** A user with a custom workflow that uses `E` for something else has to rebind. The hotkey-editor popup makes that one-click. Acceptable.
- **Digit-key handler runs on every frame the entities editor is rendered.** Cost is one `IsWindowFocused` call plus up to nine `IsKeyPressed` checks. Negligible compared to the rest of `renderContent`.
- **No cross-session memory of last-used mode.** First open per game session always lands on Actor. Mitigation: the in-session memory is free (`m_windowType` already persists); cross-session can be added in a follow-up change with one `Settings` key. Deferred per non-goal.
- **Toggle's "close all other editors" can surprise a user who had two windows open intentionally.** The set of users affected is small (multi-window editing is not a documented workflow today) and the recovery is one click on the closed window's main-menu entry. Acceptable.
- **Hotkey-editor popup will list two new entries.** This is the intended cost — the bindings need to be rebindable. The popup already handles arbitrary `Hotkeys` entries without per-binding code.

## Migration Plan

None required. New `settings.json` keys (`hotkeys.openEntitiesEditor`, `hotkeys.openObjectivesEditor`) are absent on first run after upgrade and get default values via the standard load-or-default loop in `Hotkeys::initialize()`. The first `saveHotkey(i)` call after defaults are applied writes the keys back to `settings.json`. No old keys are removed or renamed.

## Open Questions

_None at design time. The implementation tasks resolve the remaining concrete details (exact scancode literals, exact localization paths)._
