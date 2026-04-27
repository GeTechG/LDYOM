---
change: add-editor-quickopen-hotkeys
updated: 2026-04-28T00:00:00Z
status: completed
---

## In-Flight
none

## Decisions
- [task 1.1] Final scancode literals: `P` = `0xFFFFFF19` (default for `openEntitiesEditor`), `O` = `0xFFFFFF18`. The original `E` choice was abandoned because `EntityOrbitCamera::handleInput` reads bare `E` continuously as the WASDQE move-down axis — pressing the hotkey nudged the selected entity downward. `F2` was tried as an interim default and discarded in favor of letter-key `P` per user preference.
- [task 1.3 / 5.x] Localization keys live under the `hotkey_editor` object as `hk_<functionName>` and `hk_<functionName>_desc`. Four keys appended after `hk_saveScene_desc` in `../ingame/LDYOM/languages/en.json`.
- [task 4.x] Tab navigation in entities editor uses `Alt+1..Alt+9` chord, gated explicitly on `io.KeyAlt && !KeyCtrl && !KeyShift && !KeySuper && !WantTextInput && IsKeyPressed(digit)`. `IsKeyChordPressed` was tried and abandoned because its routing layer swallowed first-chord presses. The `IsWindowFocused` and `IsAnyItemActive` guards were also dropped because `EntityInfoPanel` is a sibling ImGui window — focus leaves entities the moment the user touches the info panel or in-game gizmo.
- [task 4.5] Added `!io.KeyAlt` guard in `EntityGizmo::render` so `Alt+digit` chords don't fall through to bare-digit gizmo handlers.
- [task 4.6 / 4.7] `EntitiesWindow::open()` no longer resets `m_selectedEntityIndex` — it re-fires `setSelectedEntityIndex(saved)` to re-activate orbit camera + info panel for the previously selected entity (mirrors `ObjectivesWindow`'s implicit persistence). `setSelectedEntityIndex` hardened to normalize out-of-range indices to `-1`.

## Gotchas
- Locale path is `C:\Users\User\Documents\LDYOM\ingame\LDYOM\languages\en.json` (extra `LDYOM` segment vs the CLAUDE.md hint).
- en.json had no trailing comma on `hk_saveScene_desc`; Section 5 added one.
- `window_manager.h` is at `src/rendering/window_manager.h` (not `src/core/`).
- Window ids: `"entities"` (init.cpp:34), `"objectives"` (init.cpp:30).
- `EntityGizmo::render` runs whenever an entity is selected; its bare 1/2/G/E bindings dictate what hotkey/chord defaults are safe.
- All clang IDE diagnostics observed during the change are pre-existing (clangd lacks include paths). Project builds clean with MSVC.
- Existing users with a previous `settings.json` keeping `hotkeys.openEntitiesEditor = "E"` will retain that bind on upgrade — settings override defaults. Either delete the key or rebind through the hotkey-editor popup.

## Open Questions
none

## Next Steps
1. Run `/openspec-archive-change add-editor-quickopen-hotkeys` to finalize.
