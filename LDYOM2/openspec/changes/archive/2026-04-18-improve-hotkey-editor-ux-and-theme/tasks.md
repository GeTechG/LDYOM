## 1. Theme-sampled KeyboardStyle

- [x] 1.1 Delete the file-scope `static const KeyboardStyle DefaultKeyboardStyle` constant in `src/utils/imHotKey.h`.
- [x] 1.2 Add a `KeyHover` field (`ImU32`) to the `KeyboardStyle` struct, right after `KeyNormal`.
- [x] 1.3 Implement `static KeyboardStyle BuildKeyboardStyleFromTheme()` that reads `ImGui::GetStyle().Colors[]` for `ImGuiCol_Button`, `ButtonHovered`, `Header`, `CheckMark`, `Border`, and `Text`; converts to `ImU32` via `ImGui::ColorConvertFloat4ToU32`.
- [x] 1.4 Inside `BuildKeyboardStyleFromTheme`, force alpha=1.0 on the three opaque-background colors (`KeyNormal`, `KeyPressed`, and the pre-HSV source for `KeyFace`) so partially-transparent theme button colors don't leak through the popup.
- [x] 1.5 Compute `KeyFace` from the `KeyNormal` RGBA by: unpacking to HSV via `ImGui::ColorConvertRGBtoHSV`, adding `+0.08` to V (clamped `[0,1]`), repacking via `ImGui::ColorConvertHSVtoRGB` + `ColorConvertFloat4ToU32` with alpha=1.0.
- [x] 1.6 Initialize rounding/border/inset values inside `BuildKeyboardStyleFromTheme` to the same defaults as before (`OuterRounding=4.0f`, `InnerRounding=3.0f`, `BorderThickness=1.0f`, `FaceInset=2.5f`).
- [x] 1.7 In `Edit()`, replace every `DefaultKeyboardStyle.<X>` reference with a local `const KeyboardStyle style = BuildKeyboardStyleFromTheme();` sampled once at the top of the function; reference `style.<X>` everywhere inside the key-render loop.
- [x] 1.8 Build Debug. Open the editor on each of the three shipped themes (`default`, `deep_dark`, `grey`) and confirm keycaps visually match their respective button/border/text colors.

## 2. Hover state

- [x] 2.1 After each keycap's `InvisibleButton`, call `ImGui::IsItemHovered()` and hold the result in a local `bool hovered`.
- [x] 2.2 Update the `fillColor` decision in `Edit()` to: `recorded ? KeyRecorded : (pressed ? KeyPressed : (hovered ? KeyHover : KeyNormal))`. Recorded > Pressed > Hover > Normal.
- [x] 2.3 Verify hover visually in all three themes: hovering a non-bound key should now show the theme's `ButtonHovered` color; hovering a bound key keeps `KeyPressed`.

## 3. Viewport sizing fix

- [x] 3.1 Replace `ImVec2 viewportSize = ImVec2();` with `const ImVec2 viewportSize = ImGui::GetMainViewport()->WorkSize;`.
- [x] 3.2 Build Debug and confirm the popup enlarges past 960×400 at 1440p and 4K resolutions while still clamping at minimum for small windows.

## 4. Explicit recording mode

- [x] 4.1 Inside `Edit()`, add `static bool recordingActive = false;` (resets when the modal is closed — see task 4.9).
- [x] 4.2 Add a new `Record` button alongside the existing `Clear`/`Set`/`Done` stack in the bottom-right control group, above `Set`. Use localization key `hotkey_editor.record` when inactive and `hotkey_editor.recording` when active.
- [x] 4.3 When clicked: flip `recordingActive`. Deactivating also clears the transient `recordedThisFrame[]` signal and leaves `keyDown[]` as-is.
- [x] 4.4 While `recordingActive == true`, modify the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` loop: on the first captured scancode, `memset(keyDown, 0, sizeof(keyDown))`. Then set (not toggle) `keyDown[imKey] = true`. Also set `recordedThisFrame[imKey] = true`.
- [x] 4.5 Detect modifier-only captures: if the captured `imKey` maps to Ctrl/Alt/Shift, keep recording active. Otherwise set `recordingActive = false` after the capture (auto-stop on non-modifier key).
- [x] 4.6 While `recordingActive == true`, skip the `clicked -> butSwtch = !butSwtch` toggle in the keycap render loop so mouse clicks don't interfere.
- [x] 4.7 Render a "Listening…" overlay while recording: a text label centered above the keyboard region using `ImGuiCol_CheckMark` (or derive a pulsing variant from frame count). Use localization key `hotkey_editor.recording`.
- [x] 4.8 Draw a 2px accent-colored outline (color = `style.KeyRecorded` at 0.5 alpha) around the keyboard child/region rect while recording — use `ImDrawList::AddRect` on the window draw list immediately before `EndGroup`.
- [x] 4.9 Ensure `recordingActive` resets to `false` whenever the popup closes (Done, Escape, clicking outside a modal). Add a reset on the same code path that handles `editingHotkey` selection changes.

## 5. Disabled Set button + tooltips

- [x] 5.1 Remove the `if (keyDownCount && keyDownCount < 5)` conditional wrapper around `Set`. Always render the button.
- [x] 5.2 Wrap `Set` with `ImGui::BeginDisabled(keyDownCount == 0 || keyDownCount > 4);` / `ImGui::EndDisabled();`.
- [x] 5.3 When disabled, attach an `ImGui::SetItemTooltip(_(key).c_str())` using either `hotkey_editor.set_disabled_empty_tooltip` (when `keyDownCount == 0`) or `hotkey_editor.set_disabled_toomany_tooltip` (when `> 4`).
- [x] 5.4 Build Debug and confirm the tooltip appears on hover when disabled, and does not appear when enabled.

## 6. Unsaved-combo dirty state

- [x] 6.1 At the top of `Edit()`, add `static unsigned int savedFunctionKeysAtSelect = 0;` and `static int lastEditingHotkey = -1;` for dirty tracking.
- [x] 6.2 When `editingHotkey` changes (detect via `lastEditingHotkey != editingHotkey`), snapshot `savedFunctionKeysAtSelect = hotkey[editingHotkey].functionKeys;` and update `lastEditingHotkey`.
- [x] 6.3 Compute a local `unsigned int currentPacked = packKeyDownToFunctionKeys(keyDown)` each frame (reuse the same packing logic from the `Set` button path, extracted into a `static unsigned int PackKeyDown(const bool keyDown[512])` helper to avoid duplication).
- [x] 6.4 A row is dirty when `editingHotkey == i` and `currentPacked != savedFunctionKeysAtSelect`.
- [x] 6.5 In the left-panel loop, prefix the `Selectable` label with `"* "` for a dirty row (concatenated into `hotKeyLib`).
- [x] 6.6 When the user clicks a different `Selectable` while dirty, do NOT switch `editingHotkey` immediately. Instead store the pending target in `static int pendingSelection = -1;` and open a new `BeginPopup("unsaved_discard")` with a `Discard` / `Cancel` body using localization keys `hotkey_editor.unsaved_discard_title`, `hotkey_editor.unsaved_discard_message`, `hotkey_editor.discard`, `hotkey_editor.cancel`.
- [x] 6.7 On `Discard` button in the popup: apply `editingHotkey = pendingSelection;` and rebuild `keyDown[]` from the new entry's `functionKeys`. On `Cancel`: clear `pendingSelection` and keep current state. Close the inner popup either way.
- [x] 6.8 On `Set` being pressed: update `savedFunctionKeysAtSelect = hotkey[editingHotkey].functionKeys;` so the row clears its dirty state.

## 7. Conflict detection

- [x] 7.1 After the dirty-state `currentPacked` is computed (task 6.3), iterate once over `hotkey[0..hotkeyCount)` and record which other entries have `functionKeys == currentPacked` or match another entry's `functionKeys`. Build a `std::vector<int> conflictingIndices` reset each frame.
- [x] 7.2 In the left-panel loop, for each row whose index is in `conflictingIndices`, draw a small red bullet via `ImGui::GetWindowDrawList()->AddCircleFilled(...)` at the row's leading margin (offset ~4px from the `Selectable`'s `GetItemRectMin()`, radius ~4px, color = `ImGui::GetColorU32(ImGuiCol_PlotLinesHovered)` or a fallback red `IM_COL32(220, 50, 50, 255)` if the slot is unsuitable — pick via quick eyeball test during implementation).
- [x] 7.3 Attach `ImGui::SetItemTooltip` after the Selectable for conflicting rows that describes which other function shares the combo. Use localization key `hotkey_editor.conflict_tooltip` accepting one `{}` placeholder for the conflicting function's localized name.
- [x] 7.4 Verify: bind two hotkeys to the same combo (e.g. Ctrl+S) via the editor; both rows show the red bullet. Rebinding one resolves the bullet on both.

## 8. Icon-cap tooltips

- [x] 8.1 After each `InvisibleButton` in the keycap render loop, if `key.lib` is in the icon set (`Shift`, `Ret`, `Tab`, `Up`, `Down`, `Left`, `Right`), call `ImGui::SetItemTooltip("%s", key.lib);`.
- [x] 8.2 Non-icon keys (letters, digits, `Esc`, `F1`…, `Space`, `Caps Lock`, etc.) do NOT get a tooltip.
- [x] 8.3 Verify hover on the left Shift, right Shift, Tab, Ret, and the four arrow keys all show their name.

## 9. Two-line left panel

- [x] 9.1 Replace the single `ImGui::Selectable(hotKeyLib, …)` with a `Selectable` that accepts a height equal to `ImGui::GetTextLineHeightWithSpacing() * 2`, label = `""`, and `ImGuiSelectableFlags_AllowOverlap`.
- [x] 9.2 After the Selectable call, rewind the cursor to the Selectable's top-left via `ImGui::SetCursorScreenPos(selectableMin + FramePadding)` and render the function name using `ImGui::Text("%s", functionName)`.
- [x] 9.3 On the next line, render the binding preview (`hotKeyLib` minus the parenthesized wrapper, or the bare `Ctrl + S` form) using `ImGui::PushStyleColor(ImGuiCol_Text, ImGuiCol_TextDisabled)` + `TextUnformatted` + `PopStyleColor`.
- [x] 9.4 Keep the dirty-asterisk prefix and conflict-bullet decoration from tasks 6.5 and 7.2 compatible with the two-line layout (place them on the function-name line).
- [x] 9.5 Verify that the whole two-line region is clickable as a single Selectable and that hover/active states cover both lines.

## 10. Escape handling

- [x] 10.1 Inside the `BeginPopupModal` body, before the end of `Edit()`: add `if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) { if (recordingActive) { recordingActive = false; } else { ImGui::CloseCurrentPopup(); } }`.
- [x] 10.2 Verify Escape during non-recording state closes the modal, and Escape during recording first stops recording (second Escape closes).

## 11. Localization

- [x] 11.1 Add the following keys under `hotkey_editor.*` in `ingame/LDYOM/languages/en.json`: `record` ("Record"), `recording` ("Listening..."), `set_disabled_empty_tooltip` ("Press at least one key to bind."), `set_disabled_toomany_tooltip` ("Maximum 4 keys in a combination."), `unsaved_discard_title` ("Discard unsaved changes?"), `unsaved_discard_message` ("You have unsaved modifications to this hotkey. Discard them?"), `discard` ("Discard"), `cancel` ("Cancel"), `conflict_tooltip` ("Conflicts with: {}").
- [x] 11.2 Add the same keys with Russian translations in `ingame/LDYOM/languages/ru.json`.
- [x] 11.3 Verify no other localization files (addons, etc.) need updates — grep `hotkey_editor` across `ingame/` to confirm.

## 12. Cleanup & integration

- [x] 12.1 Confirm the `Keys[6][18]` table, `HotKey`/`Key` structs, `GetHotKey`, `GetHotKeyLib`, `GetKeyForScanCode`, `GetKeyForName`, `GetOrderedScanCodes`, and `Edit()` signature are all untouched outside the render loop and control-button block.
- [x] 12.2 Grep the codebase for other callers of `ImHotKey::Edit` / `GetHotKey` / `GetHotKeyLib` and confirm none need to change. Confirm `src/core/hotkeys.{h,cpp}` compiles unchanged.
- [x] 12.3 Confirm no changes to `settings.json` parsing, `stringToKeyCombo`, or any `ImGuiCol_` enum additions.
- [x] 12.4 Delete any now-unused helpers left over from the hardcoded-style era.

## 13. Validation

- [x] 13.1 Build Debug cleanly via `cmake --build build --config Debug`.
- [x] 13.2 Back up an existing `settings.json` with pre-change bindings; launch the game; confirm those bindings still load and fire identically.
- [x] 13.3 Theme roundtrip: open the editor under `default`, `deep_dark`, and `grey`; confirm colors match each theme and are clearly distinct.
- [x] 13.4 Record flow: click Record, press Ctrl then S; confirm capture stops after S and `keyDown[]` reflects `{Ctrl, S}` without any extra toggle noise.
- [x] 13.5 Click-toggle flow (recording inactive): click Clear, click Ctrl, click S, click Set; confirm the combo binds as before.
- [x] 13.6 Disabled Set: clear all keys; confirm Set is visible but greyed out with the empty-tooltip. Press 5 keys; confirm the too-many tooltip.
- [x] 13.7 Dirty state: select a hotkey, toggle one key; confirm the asterisk appears. Click another hotkey; confirm the discard popup opens. Cancel; confirm selection stays. Discard; confirm the new selection loads.
- [x] 13.8 Conflict detection: bind two hotkeys to the same combo; confirm red bullets appear on both rows with correct tooltips.
- [x] 13.9 Icon tooltips: hover Shift/Ret/Tab/Up/Down/Left/Right; confirm each shows its name.
- [x] 13.10 Two-line rows: confirm layout, hover highlight, and click target all span both lines.
- [x] 13.11 Escape: verify the two-level behavior (stop recording → close modal).
- [x] 13.12 Resize to minimum viewport and 4K; confirm key geometry, icons, and labels scale and the editor window actually grows beyond 960×400 at 4K.
- [x] 13.13 Confirm pre-change `settings.json` format is still read identically and `Set` writes a byte-compatible value (restore backup from 13.2).
