## ADDED Requirements

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

Each keycap SHALL visually distinguish between three states: (a) normal (not part of the current binding), (b) pressed/bound (its `keyDown[scanCodePage1]` entry is true), and (c) recording (this frame the user pressed a matching physical key via the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` iteration). The three states MUST use distinct colors sourced from the `KeyboardStyle` struct.

#### Scenario: Clicking a keycap toggles pressed state

- **WHEN** the user clicks a keycap in the editor
- **THEN** its `keyDown` entry is flipped and the next frame renders that key with the `pressed` color instead of the `normal` color

#### Scenario: Physical key press marks a key as recording

- **WHEN** the edit popup is open and the user presses a physical key whose `ImGuiKey` lies in the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` range
- **THEN** that frame the corresponding keycap renders with the `recorded` color and its `keyDown` entry is toggled for subsequent frames

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

A `KeyboardStyle` struct SHALL be declared inside the `ImHotKey` namespace (local to `src/utils/imHotKey.h`) exposing at minimum: colors for `normal`, `pressed`, `recorded`, `border`, `face`, and `label`; an `outerRounding` value; an `innerRounding` value; a `borderThickness` value; and a face inset. The struct MUST be default-initialized with hardcoded values used by the renderer. It MUST NOT be exposed through `settings.json`, the `Hotkeys` class, or any user-visible configuration in this change.

#### Scenario: Renderer reads all visual constants from the struct

- **WHEN** a keycap is drawn
- **THEN** every color, rounding radius, border thickness, and inset used by the three-layer composition is read from a `KeyboardStyle` instance rather than hardcoded inline

#### Scenario: Default-constructed style drives default appearance

- **WHEN** the editor is opened without any style override
- **THEN** rendering uses the default-initialized `KeyboardStyle` values and no settings file or runtime knob is consulted

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
