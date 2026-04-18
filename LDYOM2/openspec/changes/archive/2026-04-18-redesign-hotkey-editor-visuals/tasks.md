## 1. Style scaffolding

- [x] 1.1 Declare `KeyboardStyle` struct inside the `ImHotKey` namespace in `src/utils/imHotKey.h` with fields: `KeyNormal`, `KeyPressed`, `KeyRecorded`, `KeyBorder`, `KeyFace`, `KeyLabel` (all `ImU32`), plus `OuterRounding`, `InnerRounding`, `BorderThickness`, `FaceInset` (all `float`).
- [x] 1.2 Add a file-scope `static const KeyboardStyle` default instance with hardcoded values chosen for readable contrast against the modal background (normal darker, face slightly lighter, pressed warm accent, recorded cool accent, border neutral mid-tone).
- [x] 1.3 Add a small helper (e.g. `ScaledThickness(baseKeyWidth)` or inline lambda) so `BorderThickness`/`FaceInset` scale with the viewport-adaptive `baseKeyWidth` factor already computed in `Edit()`.

## 2. Core key rendering

- [x] 2.1 Inside `Edit()`, replace the `ImGui::PushStyleColor` + `ImGui::Button` block (current lines ~350-359) with an `ImGui::InvisibleButton(unique_id, ImVec2(width, baseKeyHeight * 0.8f))` call that preserves the existing `SameLine`/`Indent` layout math untouched.
- [x] 2.2 After the `InvisibleButton`, grab `ImGui::GetItemRectMin()`/`ImGui::GetItemRectMax()` and fetch the window draw list via `ImGui::GetWindowDrawList()`.
- [x] 2.3 Draw layer 1 (outer rect): `AddRectFilled` with key background color + `OuterRounding`, then `AddRect` stroke using `KeyBorder` and `BorderThickness`.
- [x] 2.4 Draw layer 2 (inner face rect): compute inset rect from outer using `FaceInset`, call `AddRectFilled` with `KeyFace` color and `InnerRounding`.
- [x] 2.5 Draw layer 3 (label): render `Key::lib` via `AddText` centered on the face rect using `CalcTextSize`; color from `KeyLabel`.
- [x] 2.6 Preserve the existing click semantic: when `ImGui::IsItemClicked()` (or the `InvisibleButton` return value) fires, flip `keyDown[key.scanCodePage1]` (same toggle behavior as the old Button).

## 3. State coloring

- [x] 3.1 Pick the outer fill color each frame by state: `KeyPressed` when `keyDown[key.scanCodePage1]` is true, otherwise `KeyNormal`.
- [x] 3.2 Add a transient "recorded-this-frame" signal: in the existing `ImGuiKey_Tab..ImGuiKey_KeypadEqual` loop, remember the scancode toggled this frame (local `bool recordedThisFrame[512]`) and, if set, override the outer color with `KeyRecorded` for one frame.
- [x] 3.3 Verify that the three visual states (normal / pressed / recorded) are distinguishable on screen — manually confirm by clicking and by pressing the matching physical key.

## 4. Special key icons

- [x] 4.1 Add a `static` helper `DrawKeyIcon(ImDrawList*, const ImRect& faceRect, const char* lib, ImU32 color)` that dispatches on `lib` and returns `true` if it drew an icon, `false` otherwise.
- [x] 4.2 Implement the `Up`/`Down`/`Left`/`Right` branches using `AddTriangleFilled` with three corner points computed from the face rect center and a size of `max(4.0f, faceRect.GetHeight() * 0.4f)`.
- [x] 4.3 Implement `Shift` as an upward chevron — either two `AddLine` strokes or a single `AddTriangleFilled` pointing up, sized like the arrows.
- [x] 4.4 Implement `Ret` (Enter) as a return-arrow glyph: an `AddPolyline` L-shape plus an `AddTriangleFilled` arrowhead at the left terminus.
- [x] 4.5 Implement `Tab` as a right-pointing arrow: `AddLine` shaft + `AddTriangleFilled` arrowhead.
- [x] 4.6 In the label-drawing step, call `DrawKeyIcon(...)` first; if it returns `false`, fall back to text drawing.
- [x] 4.7 Add the minimum-size fallback: if `faceRect.GetHeight() * 0.4f < 4.0f`, skip icon drawing and render the text label instead.

## 5. Long label handling

- [x] 5.1 For any text label, compare `ImGui::CalcTextSize(lib).x` against the face rect width minus a small inset; if it overflows, apply `PushClipRect` on the face rect before `AddText` so text cannot bleed into neighbor caps.
- [x] 5.2 Verify visually at the enforced `minWidth=960, minHeight=400` popup size that `Backspace`, `Caps Lock`, `Space`, `PrSn`, `ScLk`, `PgU`, `PgD` still read legibly; if overflow persists, TODO: add a small abbreviation table (`Backspace` → `Bksp`, `Caps Lock` → `Caps`) gated behind a width-below-threshold check.

## 6. Cleanup and integration

- [x] 6.1 Remove the now-unused `PushStyleColor(ImGuiCol_Button, ...)` / `PopStyleColor` / `ImGui::Button(...)` lines inside the key-render loop; confirm no other `Button` in `Edit()` (Clear / Set / Done buttons) was affected.
- [x] 6.2 Confirm the `Keys[6][18]` table, the `Hotkey`/`Key` structs, `GetHotKey`, `GetHotKeyLib`, `GetKeyForScanCode`, `GetKeyForName`, `GetOrderedScanCodes`, and the `Edit()` signature were not touched outside the render loop.
- [x] 6.3 Grep the codebase for callers of `ImHotKey::Edit` / `GetHotKey` / `GetHotKeyLib` and confirm none need to change; confirm `src/core/hotkeys.{h,cpp}` compiles unchanged.
- [x] 6.4 Confirm no changes to `settings.json` parsing or to `stringToKeyCombo`.

## 7. Validation

- [x] 7.1 Build `Debug` via `cmake --build build --config Debug` and fix any compile errors.
- [x] 7.2 Back up an existing `settings.json` with pre-redesign bindings (for example `"Ctrl + S"`), launch the game, and verify the bindings still load and fire in-game identically.
- [x] 7.3 Open the Hotkey Editor modal; click a key in every row (letter, modifier, arrow, Enter, Tab, Space) and verify `keyDown[]` toggles visually via the pressed color.
- [x] 7.4 Bind a new hotkey end-to-end via mouse clicks: Clear → click modifiers + letter → Set → Done → reopen and confirm the new binding is shown and persisted.
- [x] 7.5 Bind a new hotkey end-to-end via physical key presses: verify the recorded color appears on the matched keycap the frame the physical key is pressed.
- [x] 7.6 Resize the game window to the minimum viewport and to a large viewport; verify key geometry, icons, and labels all scale cleanly with no overflow and no sub-pixel icon collapse.
- [x] 7.7 Verify `settings.json` content written by the redesigned build is byte-compatible with the pre-redesign build's parser (restore the backup from 7.2 after validation).
