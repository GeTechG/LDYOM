## Context

`src/utils/imHotKey.h` now renders keycaps with `ImDrawList` primitives (delivered by `redesign-hotkey-editor-visuals`) but its `DefaultKeyboardStyle` is a file-scope constant with hardcoded `IM_COL32(...)` values. The project's other UI surfaces read from `ImGui::GetStyle().Colors[]` which is populated by `ThemeLoader` from JSON files in `ingame/LDYOM/themes/` (currently: `default`, `deep_dark`, `grey`). As a result, the hotkey editor is the only modal in LDYOM that doesn't adapt to the active theme — switching from `default` to `deep_dark` leaves the keyboard looking pasted-in.

Alongside the color desync, the editor carries a cluster of UX rough edges that compound the "feels different from the rest of the app" impression: silent physical-key capture, a dead viewport-size branch, a vanishing `Set` button, no dirty state, no conflict warnings, no Escape key, and cramped single-line entries in the left panel. Each of these is minor individually; together they are why the modal feels older than the rest of LDYOM2's UI.

This change is a single-file follow-up that (a) wires the keyboard's visual constants to `ImGuiStyle`, and (b) resolves the cluster of interaction papercuts — all inside `imHotKey.h`'s `Edit()` function and a few new static helpers.

Stakeholders: any LDYOM user who opens the Hotkey Editor. One surface, one caller (`src/rendering/windows/quick_settings.cpp`).

## Goals / Non-Goals

**Goals:**

- `KeyboardStyle` colors are sourced from the active `ImGuiStyle` at the start of every `Edit()` call. No color literal remains in the keyboard renderer except the HSV offset used to derive `KeyFace` from `KeyNormal`.
- Add a `KeyHover` color (mapped to `ImGuiCol_ButtonHovered`) and use it when `ImGui::IsItemHovered()` is true on a keycap.
- An explicit **Record** mode that (a) shows a visible "Listening…" indicator, (b) replaces the combo on physical key capture instead of toggling `keyDown[]`, and (c) auto-stops on non-modifier key.
- `viewportSize` actually comes from `ImGui::GetMainViewport()->WorkSize` so the adaptive sizing math in `Edit()` works at large resolutions.
- `Set` is always visible; it is `BeginDisabled()`-wrapped when constraints fail, with a tooltip explaining why.
- Left panel rows show an unsaved-changes asterisk and a red "conflicts with …" bullet when applicable, as two independent decorations.
- Switching the selected hotkey while the current one has pending edits opens a confirm-discard popup.
- Icon-only caps (Shift, Ret, Tab, Up/Down/Left/Right) emit an ImGui tooltip showing their `Key::lib` text on hover.
- Escape closes the modal.
- All 500+ lines of existing API, storage format, and caller behavior are untouched.

**Non-Goals:**

- Exposing `KeyboardStyle` in the theme JSON schema. Colors are fully derived from existing `ImGuiCol_*` slots; no theme file changes.
- User-configurable keymap layouts (AZERTY, Dvorak).
- Keyboard-driven navigation of the left-panel list.
- Drag-and-drop reordering of hotkeys.
- Per-hotkey descriptions in more than one line (already wrapping via `TextWrapped`).
- Any change to `Hotkeys`, `stringToKeyCombo`, `settings.json`, or the `functionKeys` packing.
- Any new dependency (Conan or vendored).
- Any change to the special-key icon *set* — the icons themselves remain Shift chevron, arrow triangles, Ret return-arrow, Tab right-arrow.

## Decisions

### 1. Per-frame theme sampling, not cached instance

**Choice:** Build a `KeyboardStyle style = BuildKeyboardStyleFromTheme();` local at the top of `Edit()` and pass it down (or read it via a `static thread_local` updated each call). No file-scope `DefaultKeyboardStyle`.

**Why:**
- Theme can change at runtime via `ThemeLoader::loadThemeFromJson()`. If we cache on first call we desync until the modal is reopened — worse UX than the hardcoded version.
- Sampling is cheap: 6 `ImVec4 → ImU32` conversions + one HSV round-trip per `Edit()` call. Called once per frame only while the modal is open.

**Alternatives considered:**
- Cache + dirty-flag on theme change: requires a hook in `ThemeLoader::applyTheme()` to invalidate. More surface for a non-problem.
- Sample inside the per-key loop: wasteful, same answer every iteration.

### 2. `KeyFace` derived via HSV lift, not mapped to `FrameBg`

**Choice:** `KeyFace = HSV(KeyNormal) with V += 0.08, clamped to [0, 1]`.

**Why:**
- `ImGuiCol_FrameBg` is often much darker (inputs) or lighter (sliders) than `ImGuiCol_Button` depending on theme; using it would produce an inverted bevel in some themes.
- A fixed +V offset in HSV space reliably reads as "raised" on both dark and light palettes.
- The offset is purely a rendering detail, local to `imHotKey.h` — it doesn't pollute any theme file.

**Alternatives considered:**
- Add 0x10 to each RGB channel: works in dark themes, blows out in light themes where `KeyNormal` is already near-white.
- Tie to `ImGuiCol_ButtonHovered`: semantically wrong — `KeyFace` is an always-on visual detail, not a hovered state.

**Open:** the exact offset (0.06 / 0.08 / 0.10) — pick during implementation after eyeballing all three bundled themes. Default 0.08.

### 3. `KeyHover` via `IsItemHovered()` per-keycap

**Choice:** After the `InvisibleButton`, branch on `ImGui::IsItemHovered()` to pick `style.KeyHover` over `style.KeyNormal`. `KeyPressed` and `KeyRecorded` still take precedence over `KeyHover`.

**Precedence:** Recorded > Pressed > Hover > Normal. Hover wins over Normal only; never over a bound or just-recorded key.

**Why:**
- Cheap and exactly matches `ImGui::Button` feel.
- Users get the feedback they expect when hovering a key they're about to click.

### 4. Recording-mode flag, not continuous listening

**Choice:** Add `static bool recordingActive` inside `Edit()`. While `recordingActive`:
- The `ImGuiKey_Tab..ImGuiKey_KeypadEqual` loop clears `keyDown[]` on the *first* captured key, then sets captured keys in `keyDown[]` without toggling.
- Modifier keys (Ctrl/Alt/Shift) are captured additively.
- The first non-modifier key terminates recording (sets `recordingActive = false`).
- `InvisibleButton` clicks on keycaps are ignored (return early before toggling `keyDown[]`).
- The modal draws an accent-colored outline around the keyboard region and renders a `"Listening…"` label.
- The `Record` button shows as "Stop" while active; clicking it sets `recordingActive = false` without capturing.

**Why:**
- Today's "always listen" model confuses users: they click Ctrl then press S, and both events fire — the click toggles Ctrl on, the S press toggles S on, resulting in the combo they wanted by accident. Remove the accident, make it intentional.
- Auto-stop on non-modifier key matches OS conventions (Windows hotkey dialogs, VS Code keybinding editor) — one final press commits the combo.
- Keeping the non-recording click-toggle path intact means power users who prefer the old flow (construct combos manually) still have it.

**Alternatives considered:**
- Remove click-toggle entirely, always record: loses the ability to use the mouse for accessibility, and changes the existing spec's "Mouse click toggles keyDown" scenario. Kept both.
- Push-to-record button held down: adds mouse tracking logic and a timing model. Worse than a toggle.

### 5. Dirty-state and conflicts are independent decorations

**Choice:** The left-list row gets two optional prefixes:
- `"*"` before the name when the current `keyDown[]` does not match the saved `functionKeys` of the currently-selected hotkey.
- A small red bullet (a `DrawList::AddCircleFilled`) drawn in the row's leading margin when this hotkey shares its packed `functionKeys` with any *other* non-deselected entry. Hovering the bullet shows a tooltip listing the conflicting functions.

**Why:**
- Dirty state is about the currently-edited row; conflicts are about any row. Rendering them as orthogonal decorations lets both appear simultaneously without a combined state machine.
- `Selectable` spans the row; the bullet and the asterisk are inline text that doesn't break the selectable click target.

**Trade-off:** If a user has 20 hotkeys, conflict detection is O(N²) per frame. At N=20 that's 400 comparisons; still trivial. We'll cache per-entry `functionKeys` in a local vector on entry-select to avoid re-reading the struct; beyond that, no optimization needed at this scale.

### 6. Discard-unsaved popup, not an auto-save on switch

**Choice:** When the user clicks another `Selectable` in the left panel and the current row is dirty, open a secondary popup `"Discard unsaved changes?"` with `Discard` / `Cancel` buttons. `Discard` proceeds with the selection switch; `Cancel` reverts the selection.

**Why:**
- Auto-saving silently loses work if the user was mid-experimenting.
- A modal-on-modal is acceptable here: both are transient; ImGui nests them cleanly via `BeginPopupModal` inside the hotkey-editor popup.

**Alternatives considered:**
- Inline warning banner above the list: user might miss it.
- Auto-save: bad default, unrecoverable.

### 7. `Set` disabled-with-tooltip, not `Set` hidden

**Choice:** Always render the `Set` button. Wrap it in `ImGui::BeginDisabled(keyDownCount == 0 || keyDownCount > 4)` and attach a tooltip via `ImGui::SetItemTooltip("hotkey_editor.set_disabled_*_tooltip")` when disabled.

**Why:**
- A vanishing button is worse feedback than a greyed-out one: the user doesn't know whether `Set` exists at all.
- Tooltip explains the 1–4 constraint explicitly.

### 8. Icon keycaps get ImGui tooltips on hover

**Choice:** After the `InvisibleButton` for any keycap whose `lib` is in the icon set (`Shift`/`Ret`/`Tab`/`Up`/`Down`/`Left`/`Right`), call `ImGui::SetItemTooltip(key.lib)`. Non-icon keys skip the tooltip (the letter/label is already on the cap).

**Why:**
- Non-trivial users (especially localized ones) may not immediately recognize the triangle/chevron glyphs.
- Zero-cost for the no-hover case; only evaluated when the mouse is over the cap.

**Alternatives considered:**
- Always tooltip, on every key: noisy and unhelpful for alpha keys.
- Only on long-hover (delay): ImGui's `SetItemTooltip` already respects the style's `HoverDelay*`; good enough.

### 9. `ImGui::GetMainViewport()->WorkSize` for adaptive sizing

**Choice:** `const ImVec2 viewportSize = ImGui::GetMainViewport()->WorkSize;`

**Why:**
- The current `ImVec2()` literal is clearly a bug — comments say "viewport-adaptive" but the math can never exceed the `(960, 400)` floor.
- `WorkSize` (vs `Size`) excludes the main menu bar; that's what we want for a modal.

### 10. Escape closes the modal

**Choice:** Inside the `BeginPopupModal` block, check `if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) { ImGui::CloseCurrentPopup(); }`. Guarded so Escape during active recording exits recording first, then closes on a second press.

**Why:**
- Standard modal behavior. Currently absent.
- Two-level guard prevents Escape from closing the modal while the user is in the middle of recording a combo (they may press Escape intending to abort the capture).

## Risks / Trade-offs

- **Theme-derived `KeyFace` can look flat in themes with very low saturation** (near-greyscale palettes). **Mitigation:** HSV +V offset always produces *some* contrast even at S≈0; if a specific theme still reads flat, we can raise the offset at that theme's level without touching code. Revisit only if reported.
- **Recording mode may steal keypresses from the rest of the UI.** **Mitigation:** only activates inside the open popup and while `recordingActive` is true; otherwise the existing `io.WantTextInput` guard in `GetHotKey` still covers the runtime side.
- **Conflict detection is O(N²) on the visible hotkey list.** **Mitigation:** acceptable at current list size (< 10); if list grows, precompute a `std::map<functionKeys, count>` once per frame in O(N).
- **Nested modal (discard-confirm inside hotkey-editor)** may misbehave under the main popup's `ImGuiWindowFlags_NoResize`. **Mitigation:** tested nested popups are a standard ImGui pattern and work cleanly; `BeginPopupModal` returns false when a parent popup is inactive, so the inner popup ties correctly to the outer's lifecycle.
- **Escape key collision with gameplay key binding.** **Mitigation:** we only consume Escape when `BeginPopupModal` is open; outside the modal, Escape flows normally.
- **Theme color `alpha < 1.0` on `KeyNormal`.** `ImGuiCol_Button` in `default.json` is `[0.44, 0.44, 0.44, 0.40]` — 40% alpha would make the keycap background translucent against the popup. **Mitigation:** when packing to `ImU32` via `ColorConvertFloat4ToU32`, force alpha to 1.0 for `KeyNormal`/`KeyPressed`/`KeyFace` (they're opaque backgrounds); preserve alpha for `KeyBorder` and `KeyLabel`.

## Migration Plan

Single file edit + 6 new localization keys. No settings migration, no API change, no data migration.

1. `src/utils/imHotKey.h`:
   - Add `KeyHover` field to `KeyboardStyle`.
   - Delete `DefaultKeyboardStyle`; add `static KeyboardStyle BuildKeyboardStyleFromTheme()` that reads `ImGui::GetStyle().Colors[...]`, packs to `ImU32`, forces opaque alpha on fill colors, computes `KeyFace` via HSV +V offset.
   - In `Edit()`: sample `KeyboardStyle style = BuildKeyboardStyleFromTheme()` once at the top; fix `viewportSize`; add `recordingActive` state; add Record button; update `ImGuiKey_Tab..ImGuiKey_KeypadEqual` loop to branch on `recordingActive`; wire hover precedence; wrap `Set` with `BeginDisabled`; add tooltips; add dirty-state tracking + confirm popup; add conflict detection + red bullet; add two-line left rows; add Escape handling.

2. `ingame/LDYOM/languages/en.json` + `ru.json`: add the 6 new keys under `hotkey_editor.*`.

3. Build Debug (`cmake --build build --config Debug`). Manually verify each of the feature checkpoints in `tasks.md §7`.

**Rollback:** `git revert` of the commit(s) touching `imHotKey.h` and the two language files. No data cleanup.

## Open Questions

- `KeyFace` HSV offset value — settle on 0.06 / 0.08 / 0.10 during implementation review. Default 0.08.
- Should the `"Listening…"` text be a pulsing accent-colored overlay centered on the keyboard, or a small row above the Record button? Leaning on-keyboard overlay — higher visibility. Defer to implementation.
- Conflict-bullet color: `ImGuiCol_PlotLinesHovered` (reddish in default theme) vs a fixed red literal. Leaning on the ImGuiCol slot so colorblind themes can override it. Defer to implementation.
- Do we want to block the Done button while recording is active? Probably yes — pressing Done mid-recording should either commit or cancel, not both ambiguously. Revisit during implementation.
