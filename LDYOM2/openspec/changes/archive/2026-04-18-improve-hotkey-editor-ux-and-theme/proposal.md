## Why

The `redesign-hotkey-editor-visuals` change gave us a 3-layer DrawList keycap, but it left the editor with two unresolved problems that became obvious once the new visuals shipped:

1. **Colors are hardcoded** (charcoal/amber/blue in `DefaultKeyboardStyle`). The project already has a full theme system (`ThemeLoader` + `ImGui::GetStyle().Colors[]`) and ships three themes (`default`, `deep_dark`, `grey`). The hotkey editor is the one surface in the app that ignores the loaded theme — it looks foreign the moment the user switches away from the default palette.

2. **Interaction papercuts** that together make the modal feel less polished than the rest of LDYOM2:
   - No visible "we're listening" state — physical keys silently toggle `keyDown[]` with no feedback that recording is happening.
   - The `viewportSize = ImVec2()` branch in `Edit()` is dead code (always zero), so the advertised "80% of viewport" adaptive sizing never fires and the popup is permanently clamped to 960×400.
   - The `Set` button disappears when `keyDownCount == 0` or `> 4`, leaving users to guess why.
   - Switching hotkeys in the left list silently discards pending edits.
   - Two hotkeys can bind the same combo with no warning.
   - Icon-only caps (Shift/Ret/Tab/arrows) give no affordance for what the icon means.
   - No way to tell at a glance which list entry has unsaved changes.
   - Escape doesn't close the modal.

## What Changes

### Theme coupling (replaces hardcoded colors)

- Replace the file-scope `static const KeyboardStyle DefaultKeyboardStyle` with a `static KeyboardStyle BuildKeyboardStyleFromTheme()` helper sampled once per `Edit()` call from `ImGui::GetStyle().Colors[]`.
- Color mapping: `KeyNormal → ImGuiCol_Button`; `KeyPressed → ImGuiCol_Header`; `KeyRecorded → ImGuiCol_CheckMark`; `KeyBorder → ImGuiCol_Border`; `KeyLabel → ImGuiCol_Text`; `KeyFace → ImGuiCol_Button lightened by +0.08 in HSV value`.
- **Add `KeyHover`** field to `KeyboardStyle` mapped to `ImGuiCol_ButtonHovered`; render with this color when `ImGui::IsItemHovered()` is true on a key's `InvisibleButton`.

### Interaction improvements

- **Explicit recording mode.** Add a `Record` button alongside `Clear`/`Set`/`Done`. While recording:
  - The `ImGuiKey_Tab..ImGuiKey_KeypadEqual` listener *replaces* the current combo instead of toggling `keyDown[]`.
  - The popup draws a visible accent border + a `"Listening…"` label.
  - Recording auto-stops the frame a non-modifier key is captured; clicking `Record` again while active also stops it.
  - Click-to-toggle on individual keys is disabled while recording (to avoid mixed input).
- **Fix dead viewport math.** Source `viewportSize` from `ImGui::GetMainViewport()->WorkSize` so the "80% of viewport, with 960×400 floor" intent actually applies.
- **Disable, don't hide, the `Set` button** when `keyDownCount == 0` or `> 4`; attach an ImGui tooltip explaining the constraint.
- **Unsaved-combo indicator.** Track the original `functionKeys` on hotkey-select; when `keyDown[]` diverges, prefix the left-list entry with an asterisk. Switching hotkeys while dirty opens a small "Discard unsaved changes?" confirm popup.
- **Conflict detection.** Compute the packed `functionKeys` from current `keyDown[]` every frame; for any *other* hotkey in the list with the same packed value, show a red bullet + tooltip naming the conflicting function on that entry.
- **Tooltips on icon caps.** `ImGui::SetItemTooltip(Key::lib)` on Shift/Ret/Tab/Up/Down/Left/Right `InvisibleButton`s so the user can identify the glyph.
- **Two-line left-panel rows.** Render each entry as: bold function name on line 1, current binding in `TextDisabled` color on line 2. `Selectable` spans both lines.
- **Escape closes modal.** `if (ImGui::IsKeyPressed(ImGuiKey_Escape)) ImGui::CloseCurrentPopup();` inside the open-popup block.

### Preserved

- All contracts locked by `redesign-hotkey-editor-visuals`: the filename `src/utils/imHotKey.h`, header-only shape, the `Keys[6][18]` QWERTY table, the `HotKey`/`Key` structs, the `Edit` / `GetHotKey` / `GetHotKeyLib` / `GetKeyForScanCode` / `GetKeyForName` / `GetOrderedScanCodes` signatures, the 32-bit `functionKeys` scancode packing, the `"Ctrl + S"` settings string format, `stringToKeyCombo` parsing, and the `Hotkeys` class in `src/core/hotkeys.{h,cpp}`.
- The three-layer keycap composition (outer rect, inner face rect, centered label/icon).
- The special-key icon set (Shift chevron, Up/Down/Left/Right triangles, Ret return-arrow, Tab right-arrow).
- No new dependencies, no icon font, no persisted style overrides, no change to `settings.json` schema, no addition to `ImGuiStyle`.

### Explicitly out of scope

- Persisting `KeyboardStyle` overrides to the theme JSON files.
- Multi-layout support (still QWERTY-only).
- Keyboard navigation of the left list (arrow keys / focus). Possible future change.
- Drag-to-reorder of hotkeys.
- Localized key labels on the keycaps themselves (still ASCII from `Keys[][]`).

## Capabilities

### Modified Capabilities

- `hotkey-editor`: the visual-rendering contract from the previous change is extended with a theme-coupling rule (colors MUST come from `ImGui::GetStyle().Colors[]`), a new hover state, and a set of interaction requirements (explicit recording mode, dirty-state indicator, conflict detection, disabled-not-hidden `Set`, viewport-adaptive sizing actually functional, Escape handling, tooltips on icon keys).

### New Capabilities

None.

## Impact

- **Affected code:** `src/utils/imHotKey.h` only. Every change is inside `Edit()` or helper functions in the `ImHotKey` namespace.
- **Unaffected code:** `src/core/hotkeys.{h,cpp}`, `src/rendering/windows/quick_settings.cpp` (the one caller of `ImHotKey::Edit`), `settings.json`, theme JSON files, all localization files.
- **Localization:** adds 6 new keys under `hotkey_editor.*` in `../ingame/languages/en.json` and `ru.json`: `record`, `recording`, `set_disabled_empty_tooltip`, `set_disabled_toomany_tooltip`, `unsaved_discard_title`, `unsaved_discard_message`, `conflict_tooltip`.
- **Dependencies:** no additions. Uses existing ImGui primitives (`GetStyle`, `ColorConvertRGBtoHSV`/`HSVtoRGB`, `IsItemHovered`, `SetItemTooltip`, `BeginPopupModal`, `IsKeyPressed`).
- **Risk:** low-to-medium. Recording-mode semantics are the biggest behavior change; a bug there could make recording feel worse than it does today. Mitigation: keep the pre-change click-toggle path available when *not* in recording mode (i.e. the old flow still works as a fallback).
- **Rollback:** single-file revert of `src/utils/imHotKey.h` + revert of the 6 new localization keys. No data migration.
