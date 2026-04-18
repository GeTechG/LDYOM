# hotkey-editor Specification

## Purpose
TBD - created by archiving change redesign-hotkey-editor-visuals. Update Purpose after archive.
## Requirements
### Requirement: Visual Key Rendering via ImDrawList

The hotkey editor SHALL render every keycap using `ImDrawList` primitives composed of three layers drawn in back-to-front order: (1) an outer rectangle providing the key's background fill and border stroke, (2) an inner "face" rectangle inset from the outer rectangle with its own rounding and fill, and (3) a centered label (text or icon glyph). Keycaps MUST NOT be rendered with `ImGui::Button`.

#### Scenario: Normal letter key renders three layers

- **WHEN** the editor renders a standard letter key such as `A`
- **THEN** the renderer issues an outer filled+stroked rounded rectangle, an inset inner filled rounded rectangle on top of it, and a centered text label drawn last over the inner face

#### Scenario: Wide modifier key uses the same three layers

- **WHEN** the editor renders a wide key such as `Space` (width 260) or `Backspace` (width 80)
- **THEN** the three-layer composition is preserved and the outer/inner rectangles stretch to the key's scaled width without any additional widget falling back to `ImGui::Button`

#### Scenario: Full row renders as a group of keycaps

- **WHEN** the editor draws one of the six QWERTY rows from `ImHotKey::Keys[y]`
- **THEN** every entry whose `lib` is non-null is drawn using the three-layer keycap composition at its per-row offset and width

### Requirement: Key State Visualization

Each keycap SHALL visually distinguish between four states: (a) normal (not part of the current binding and not hovered), (b) hovered (pointer over the keycap and the key is not bound or recording), (c) pressed/bound (its `keyDown[scanCodePage1]` entry is true), and (d) recording (this frame the user pressed a matching physical key via the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` iteration). The four states MUST use distinct colors sourced from the `KeyboardStyle` struct with precedence Recorded > Pressed > Hover > Normal.

#### Scenario: Hovering a non-bound keycap shows the hover color

- **WHEN** the pointer moves over a keycap whose `keyDown` entry is false and which was not toggled this frame
- **THEN** the outer fill renders with `KeyboardStyle::KeyHover`

#### Scenario: Hover does not override pressed or recorded state

- **WHEN** the pointer hovers a keycap that is bound (`keyDown[] == true`) or was toggled by a physical key this frame
- **THEN** the outer fill remains `KeyPressed` or `KeyRecorded` respectively, not `KeyHover`

#### Scenario: Clicking a keycap toggles pressed state

- **WHEN** the user clicks a keycap in the editor AND recording mode is not active
- **THEN** its `keyDown` entry is flipped and the next frame renders that key with the `pressed` color instead of the `normal` color

#### Scenario: Physical key press marks a key as recording

- **WHEN** the edit popup is open and the user presses a physical key whose `ImGuiKey` lies in the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` range
- **THEN** that frame the corresponding keycap renders with the `recorded` color

### Requirement: Special Key Icons

For a fixed set of special keys the label layer SHALL draw a vector icon rather than the `Key::lib` text: `Up`, `Down`, `Left`, `Right` render as triangles pointing in the corresponding direction; `Shift` renders as an upward chevron; `Ret` (Enter) renders as a return-arrow glyph; `Tab` renders as a right-arrow glyph. All other keys SHALL continue to render their `Key::lib` string as text.

#### Scenario: Arrow keys render as triangles

- **WHEN** the `Up`, `Down`, `Left`, or `Right` key is drawn
- **THEN** the label layer is a filled triangle pointing in the matching direction instead of the text `"Up"`/`"Down"`/`"Left"`/`"Right"`

#### Scenario: Shift renders as a chevron

- **WHEN** a `Shift` keycap is drawn (either left or right Shift entry in row 4)
- **THEN** the label layer is an upward chevron glyph instead of the text `"Shift"`

#### Scenario: Enter and Tab render as arrow glyphs

- **WHEN** the `Ret` keycap is drawn
- **THEN** its label is a return-arrow glyph
- **WHEN** the `Tab` keycap is drawn
- **THEN** its label is a right-arrow glyph

#### Scenario: Non-special keys keep text labels

- **WHEN** any key other than the arrow/Shift/Enter/Tab set is drawn (for example `A`, `Esc`, `F5`, `Space`, `Caps Lock`)
- **THEN** the label layer renders the `Key::lib` string as centered text

### Requirement: KeyboardStyle Struct

A `KeyboardStyle` struct SHALL be declared inside the `ImHotKey` namespace (local to `src/utils/imHotKey.h`) exposing at minimum: colors for `normal`, `hover`, `pressed`, `recorded`, `border`, `face`, and `label`; an `outerRounding` value; an `innerRounding` value; a `borderThickness` value; and a face inset. Color values MUST be derived from the active `ImGui::GetStyle().Colors[]` at the start of every `Edit()` invocation, not from hardcoded constants. The struct MUST NOT be exposed through `settings.json`, the `Hotkeys` class, the theme JSON schema, or any user-visible configuration.

#### Scenario: Renderer reads all visual constants from the struct

- **WHEN** a keycap is drawn
- **THEN** every color, rounding radius, border thickness, and inset used by the three-layer composition is read from a `KeyboardStyle` instance rather than hardcoded inline

#### Scenario: Colors update when the active theme changes

- **WHEN** `ThemeLoader::loadThemeFromJson(...)` replaces `ImGui::GetStyle().Colors[]` while the editor popup is open
- **THEN** the next frame samples the new theme and re-renders all keycaps with colors derived from the new values of `ImGuiCol_Button`, `ImGuiCol_ButtonHovered`, `ImGuiCol_Header`, `ImGuiCol_CheckMark`, `ImGuiCol_Border`, and `ImGuiCol_Text`

#### Scenario: KeyFace is derived from KeyNormal via HSV lift

- **WHEN** `KeyboardStyle` is built for a frame
- **THEN** `KeyFace` is computed by converting `KeyNormal` RGB to HSV, adding a fixed offset to the V channel, clamping to `[0,1]`, converting back to RGB, and packing with alpha=1.0

#### Scenario: Opaque-background colors force alpha 1.0

- **WHEN** the active theme provides a partially-transparent `ImGuiCol_Button` (for example alpha=0.40 in the default theme)
- **THEN** `KeyNormal`, `KeyPressed`, and `KeyFace` are packed into `ImU32` with alpha overridden to `0xFF` so the popup background does not bleed through the keycap fill

### Requirement: Layout Preservation

The visual redesign SHALL reuse the existing `ImHotKey::Keys[6][18]` QWERTY table as the single source of truth for row/column layout, offsets, and widths. Multiple keyboard layouts (AZERTY, Dvorak, etc.) are out of scope. Viewport-adaptive sizing MUST be preserved: key width and key height continue to scale from `rightPanelWidth` and `keyboardHeight` exactly as they do today.

#### Scenario: Existing QWERTY table drives placement

- **WHEN** the editor renders the keyboard
- **THEN** row/column iteration uses `ImHotKey::Keys[y][x]` with `offset`, `width`, `lib`, `scanCodePage1`, and `scanCodePage7` as they exist today and no new layout table is introduced

#### Scenario: Viewport resize scales keys proportionally

- **WHEN** the popup window size changes (driven by the existing `minWidth`/`minHeight`/viewport math)
- **THEN** `baseKeyWidth` and `baseKeyHeight` recompute as they do today and every keycap scales its outer/inner/label geometry proportionally

### Requirement: Interaction Preservation

Click hit-testing and the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` recording loop SHALL produce the same `keyDown[]` state transitions as the pre-redesign `ImGui::Button` implementation. Clicking a keycap MUST toggle `keyDown[key.scanCodePage1]`; pressing a matching physical key during the popup MUST toggle the same entry via `MapVirtualKeyA(vk, MAPVK_VK_TO_VSC)`.

#### Scenario: Mouse click toggles keyDown exactly like the old Button

- **WHEN** the user clicks a keycap whose `scanCodePage1` is `S`
- **THEN** `keyDown[scanCodeOfS]` flips, identical to the previous `ImGui::Button` behavior

#### Scenario: Physical key press toggles the same slot as a click

- **WHEN** the user presses physical `S` while the popup is open
- **THEN** the `ImGuiKey_S` branch resolves to `'S'`, `MapVirtualKeyA` yields the same scancode, and `keyDown[scanCodeOfS]` toggles — producing the same state as a mouse click on the `S` cap

### Requirement: Storage And Runtime Surface Unchanged

The visual redesign SHALL NOT alter: the filename `src/utils/imHotKey.h` or its header-only shape; the signatures of `Edit`, `GetHotKey`, `GetHotKeyLib`, `GetKeyForScanCode`, `GetKeyForName`, and `GetOrderedScanCodes`; the `HotKey` and `Key` struct layouts; the 32-bit `functionKeys` scancode packing (`scanCodes[3]<<24 | scanCodes[2]<<16 | scanCodes[1]<<8 | scanCodes[0]`); the `Hotkeys` class in `src/core/hotkeys.{h,cpp}`; the `settings.json` string format (for example `"Ctrl + S"`) including `stringToKeyCombo` parsing; or the runtime hotkey detection path in `GetHotKey`.

#### Scenario: A hotkey saved before redesign loads identically after

- **WHEN** `settings.json` contains an entry such as `"Ctrl + S"` written by the pre-redesign build
- **THEN** the post-redesign build parses it into the same scancode-packed `functionKeys` value and `GetHotKey` matches it against live input identically

#### Scenario: Public API surface is byte-identical

- **WHEN** an existing caller of `ImHotKey::Edit`, `ImHotKey::GetHotKey`, or `ImHotKey::GetHotKeyLib` is recompiled against the redesigned header without source changes
- **THEN** compilation succeeds with no signature changes and runtime behavior of those functions is unchanged aside from the visual appearance of `Edit`'s popup

#### Scenario: Runtime detection path unchanged

- **WHEN** the user presses a bound combination during gameplay
- **THEN** `GetHotKey` walks `ImGuiKey_Tab..ImGuiKey_KeypadEqual`, builds the same scancode set, and returns the matching `HotKey*` exactly as it does today

### Requirement: Explicit Recording Mode

The editor SHALL provide a user-invocable recording mode separate from always-on physical-key capture. While recording is active: (a) the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` listener SHALL clear `keyDown[]` on the first captured key and then *set* (not toggle) entries corresponding to captured keys; (b) mouse clicks on keycaps SHALL NOT modify `keyDown[]`; (c) the editor SHALL display a visible "Listening…" indicator and an accent-colored outline around the keyboard region; (d) capturing any non-modifier key SHALL auto-stop recording; (e) the user SHALL be able to stop recording manually by clicking the Record control a second time.

#### Scenario: Starting recording clears and replaces the combo

- **WHEN** the user clicks the `Record` button and then presses `Ctrl+S` physically
- **THEN** `keyDown[]` is cleared on the first Ctrl capture, set to Ctrl+S on the S capture, recording auto-stops after S (non-modifier), and no residual toggles from prior manual clicking remain

#### Scenario: Clicking keycaps while recording has no effect

- **WHEN** recording is active and the user clicks a keycap with the mouse
- **THEN** `keyDown[]` is unchanged by the click

#### Scenario: Escape during recording stops recording, not the modal

- **WHEN** recording is active and the user presses `Escape`
- **THEN** recording deactivates and the modal remains open; a second `Escape` while not recording closes the modal

#### Scenario: Recording state resets when the modal closes

- **WHEN** the modal closes via `Done`, `Escape` (non-recording), or any other close path
- **THEN** the internal `recordingActive` flag resets to `false` before the next invocation of `Edit()`

### Requirement: Disabled Set Button With Tooltip

The `Set` button SHALL always be rendered when the modal is open. When `keyDownCount == 0` or `keyDownCount > 4` the button SHALL be rendered in the disabled state with an attached tooltip describing the constraint. The button MUST NOT be hidden or removed under any circumstance while the modal is open.

#### Scenario: Set is greyed out with a tooltip when zero keys are bound

- **WHEN** `keyDownCount == 0`
- **THEN** `Set` renders disabled and hovering it shows a tooltip reading `hotkey_editor.set_disabled_empty_tooltip`

#### Scenario: Set is greyed out with a tooltip when more than four keys are bound

- **WHEN** `keyDownCount > 4`
- **THEN** `Set` renders disabled and hovering it shows a tooltip reading `hotkey_editor.set_disabled_toomany_tooltip`

#### Scenario: Set is enabled for valid combo sizes

- **WHEN** `keyDownCount` is between 1 and 4 inclusive
- **THEN** `Set` renders enabled and clicking it commits the combo as before

### Requirement: Unsaved Combo Indicator And Discard Confirmation

The left-panel hotkey list SHALL mark any entry that the user is currently editing AND whose in-progress combo diverges from its saved `functionKeys`. Marking MUST be done with a leading asterisk on the entry's name line. Selecting a different list entry while the current one is dirty SHALL open a confirm-discard popup offering `Discard` and `Cancel` actions; the selection change MUST NOT apply until the user resolves the popup.

#### Scenario: Asterisk appears when the combo diverges from the saved value

- **WHEN** the user toggles a key on the currently-selected hotkey and the resulting packed `functionKeys` differs from what was saved at selection time
- **THEN** that entry's left-panel row renders with a `"* "` prefix before the function name

#### Scenario: Asterisk clears after pressing Set

- **WHEN** the user presses `Set` on a dirty entry
- **THEN** the saved baseline updates to the new `functionKeys` and the asterisk disappears that frame

#### Scenario: Switching entries while dirty opens the confirm popup

- **WHEN** the user clicks a different hotkey in the left list while the current entry is dirty
- **THEN** the selection switch is deferred, a confirm-discard popup opens, and only `Discard` applies the pending selection while `Cancel` reverts

### Requirement: Conflict Detection

The left-panel hotkey list SHALL decorate any entry whose `functionKeys` matches the `functionKeys` of another entry with a red bullet marker and a tooltip naming the conflicting function(s). Conflict detection SHALL run every frame against the current in-memory list of hotkeys.

#### Scenario: Two hotkeys bound to the same combo both show a conflict bullet

- **WHEN** two entries in `hotkey[]` have identical `functionKeys`
- **THEN** both rows render a red bullet in their leading margin and each row's tooltip names the other conflicting function

#### Scenario: Resolving a conflict clears the bullet on both rows

- **WHEN** the user rebinds one of two conflicting hotkeys to a unique combo and presses `Set`
- **THEN** the next frame neither row shows a conflict bullet

### Requirement: Icon Keycap Tooltips

Keycaps rendered as icon glyphs (`Shift`, `Ret`, `Tab`, `Up`, `Down`, `Left`, `Right`) SHALL display an ImGui tooltip on hover showing the underlying `Key::lib` text. Keycaps rendered as text labels MUST NOT attach a tooltip.

#### Scenario: Hovering an icon keycap shows its name

- **WHEN** the pointer rests on the Up-arrow keycap
- **THEN** an ImGui tooltip renders with the text `Up`

#### Scenario: Non-icon keycaps do not emit tooltips

- **WHEN** the pointer rests on the `A` keycap
- **THEN** no tooltip renders (the label on the cap is the name)

### Requirement: Viewport-Adaptive Sizing Actually Applies

The `Edit()` popup SHALL size itself as `max(minimum, viewport * factor)` using the real main-viewport work size, not a zero-initialized vector. The minimum floor of `(960, 400)` is preserved.

#### Scenario: Large viewport enlarges the popup

- **WHEN** the game runs at 3840×2160 and the user opens the editor
- **THEN** the popup size is approximately `(3072, 1728)` (80% of work size), well above the 960×400 floor

#### Scenario: Small viewport clamps to the floor

- **WHEN** the game runs at a small resolution where 80% of the work size would be below 960×400
- **THEN** the popup clamps to the (960, 400) floor

### Requirement: Two-Line Left Panel Rows

Each entry in the left-panel list SHALL render on two visual lines inside a single `Selectable`: the function's localized name on the first line in the default text color, and the current binding preview on the second line in `ImGuiCol_TextDisabled`. The full two-line region MUST be a single click target.

#### Scenario: Name and binding render on separate lines

- **WHEN** the editor opens with a hotkey bound to `Ctrl+S` labelled `Save`
- **THEN** its row shows `Save` on line one and `Ctrl + S` (or its localized form) on line two in a dimmer color

#### Scenario: Clicking either line selects the entry

- **WHEN** the user clicks the binding preview line
- **THEN** the row selects identically to clicking the name line

#### Scenario: Decorations integrate with the two-line layout

- **WHEN** a row is dirty and in conflict simultaneously
- **THEN** the asterisk prefixes the name line and the red bullet appears in the leading margin without disturbing either line's text

### Requirement: Escape Closes The Modal

The editor SHALL respond to the Escape key with a two-level behavior: if recording is active, Escape SHALL deactivate recording without closing the modal; otherwise Escape SHALL close the modal via `ImGui::CloseCurrentPopup()`.

#### Scenario: Escape outside recording closes the modal

- **WHEN** the popup is open, recording is not active, and the user presses `Escape`
- **THEN** the modal closes identically to pressing `Done`

#### Scenario: Escape during recording stops recording

- **WHEN** the popup is open, recording is active, and the user presses `Escape`
- **THEN** recording deactivates, the modal stays open, and a subsequent `Escape` closes the modal

