## Why

Authoring a mission means hopping between two editors many times per session: `EntitiesWindow` (to add/tune actors, vehicles, objects, pickups, …) and `ObjectivesWindow` (to author the objective list). Today there is no shortcut to either — the user has to press `I` to open the main menu, click "Entities" or "Objectives", and (for entities) pick the right tab. That is three to four clicks for a workflow performed dozens of times a session.

The user wants a single-key path to each editor. The asymmetry between the two surfaces is the design problem: `ObjectivesWindow` is one window; `EntitiesWindow` is one window with **nine modes** (`Actor`, `Vehicle`, `Train`, `Object`, `Pickup`, `Firework`, `Particle`, `Checkpoint`, `Audio`) selected via `setWindowType()`. A single hotkey for "open Entities" cannot pick the mode for the user — but minting nine global hotkeys for the nine modes pollutes the keymap and conflicts with `Ctrl+S` / `Ctrl+Z` / future bindings.

The chosen solution is a **two-tier hybrid**: two new global hotkeys (`P` for entities, `O` for objectives) open and toggle their respective editors, and the entities editor exposes **window-local `Alt+digit` shortcuts** (`Alt+1`–`Alt+9`) to switch between modes once it has focus. The user lands on the editor with one keystroke and switches modes without leaving the keyboard. (`P` was chosen over the more mnemonic `E` because `EntityOrbitCamera` reads bare `E` every frame an entity is selected as the WASDQE move-down axis — pressing `E` to fire the hotkey would also nudge the selected entity downward.)

## What Changes

- Register two new hotkeys in `Hotkeys::initialize()`:
  - `openEntitiesEditor` (default `P`)
  - `openObjectivesEditor` (default `O`)
- Wire callbacks in `Application::process()` with the same shape as the existing `openEditor` callback:
  - Both callbacks no-op when `ProjectPlayer::isPlaying()` is true (consistent with `unlockPlayer`, `teleportToMarker`, `saveScene` gating today).
  - Both callbacks no-op when `ProjectsManager::getCurrentProjectIndex() == -1` (no project loaded — there is nothing to edit).
  - Toggle behavior: if the target window is currently open, **close just that window**. Otherwise, **close every other open editor window and open the target window**. This guarantees `P` always lands on Entities and `O` always lands on Objectives, while a second press cleanly closes the editor — matching the muscle-memory of `openEditor` (`I`) without inheriting its "close everything" behavior, which would defeat the point of a targeted hotkey.
  - For `openEntitiesEditor`, when opening, the window opens with its current `m_windowType` field — i.e. the last mode used in this session, defaulting to `EntitiesWindowType_Actor` on first open per `entities.h:23`. No persistence across game restarts in v1.
- Add window-local Alt+digit shortcuts to `EntitiesWindow::renderContent`: when the entities window is rendering and ImGui is not capturing text input, pressing `Alt+1`–`Alt+9` (pure-Alt chord, no other modifier) switches `m_windowType` to the corresponding `EntitiesWindowType_*` value (`Alt+1=Actor`, `Alt+2=Vehicle`, `Alt+3=Train`, `Alt+4=Object`, `Alt+5=Pickup`, `Alt+6=Firework`, `Alt+7=Particle`, `Alt+8=Checkpoint`, `Alt+9=Audio`) and resets `m_selectedEntityIndex` to `-1` so the visible list refreshes for the new mode. Bare digits stay reserved for `EntityGizmo`'s TRANSLATE/ROTATE bindings.
- The two new global hotkeys must appear in the existing hotkey-editor UI (the `Settings`-backed list driven by `Hotkeys::getHotkeys()`) so users can rebind them; this happens automatically by virtue of being registered in `Hotkeys::initialize()`.
- Localization: add display-name keys for `openEntitiesEditor` and `openObjectivesEditor` in `../ingame/languages/en.json` under whatever path the existing `openEditor` / `toggleDebugInfo` / `teleportToMarker` use (verified in tasks).

## Capabilities

### New Capabilities

- `editor-quickopen-hotkeys`: Covers the global keyboard shortcuts that open and toggle the entity and objective editor windows, including the window-local digit-key navigation that switches `EntitiesWindow` between its nine modes.

### Modified Capabilities

_None._ The existing `hotkey-editor` capability covers the visual and UX behavior of the hotkey *editor popup*; it does not specify which functions are bindable, so adding two more registered hotkeys does not modify it. The `Hotkeys` class itself is unspecced today (no capability owns its registration mechanism), so the new hotkeys are additive without touching any existing requirement.

## Impact

- Code:
  - `src/core/hotkeys.cpp` — two new `emplace_back` entries in `Hotkeys::initialize()` with default scancodes for `P` and `O` (use the existing `0xFFFFFFxx` scancode pattern; verified in tasks against `imHotKey.h`'s `Keys[]` table).
  - `src/core/application.cpp` — two new `addHotkeyCallback` blocks alongside the existing five.
  - `src/rendering/windows/entities.cpp` — add the digit-key handler at the top of `renderContent`, gated on `ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)` and `!ImGui::GetIO().WantTextInput`.
- Localization: `../ingame/languages/en.json` — two new display-name strings for the hotkey editor list.
- APIs: none broken. `Hotkeys`, `WindowManager`, `EntitiesWindow`, `ObjectivesWindow` keep their public surfaces. The two new hotkey names (`openEntitiesEditor`, `openObjectivesEditor`) join the existing five as part of the persisted `hotkeys` map in `Settings`.
- Dependencies/systems: none added. Reuses `Hotkeys`, `WindowManager`, `ProjectPlayer::isPlaying()`, `ProjectsManager::getCurrentProjectIndex()`, ImGui's `IsWindowFocused` and `IsKeyPressed`.
- Backwards compatibility: a `settings.json` without `hotkeys.openEntitiesEditor` / `hotkeys.openObjectivesEditor` keys gets the defaults (`P`, `O`) and is then re-persisted on first run via the existing `saveHotkey(i)` loop in `Hotkeys::initialize()`. No migration required.
