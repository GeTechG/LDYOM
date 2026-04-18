## Why

The hotkey editor currently renders the on-screen keyboard with flat, solid-colored `ImGui::Button` widgets, which look like generic UI controls rather than a keyboard. This hurts the "recognize-at-a-glance" quality users expect when rebinding shortcuts and makes the editor feel less polished than the rest of LDYOM2's UI. A DrawList-based keycap render (inspired by `mgerhardy/imgui_keyboard`) fixes this without adding a dependency or touching any storage/runtime code paths.

## What Changes

- Replace `ImGui::Button`-per-key rendering in `src/utils/imHotKey.h` with `ImDrawList` primitives using a 3-layer keycap composition: outer rect (background + border), inner "face" rect (slightly smaller, different rounding), and centered label.
- Add a local `KeyboardStyle` struct inside the `ImHotKey` namespace holding: normal/pressed/recorded/border/face colors, outer rounding, inner rounding, border thickness, and face inset. Default-initialized; not user-configurable in this change.
- Render icons instead of text for special keys: chevron for Shift, triangles for Up/Down/Left/Right arrows, and dedicated glyphs for Enter (return arrow) and Tab (double arrow). All other keys keep their `Key::lib` text label.
- Keep hit-testing behavior: clicking a keycap toggles its `keyDown[]` entry (same as today's Button), so recording flow is unchanged.
- Preserve unchanged: `imHotKey.h` filename and header-only shape, the `ImHotKey::Keys[6][18]` QWERTY layout table, the `HotKey`/`Key` structs, scancode-in-`unsigned int` storage model, the `Edit` / `GetHotKey` / `GetHotKeyLib` / `GetKeyForScanCode` / `GetKeyForName` / `GetOrderedScanCodes` function signatures, the viewport-adaptive sizing math, and the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` iteration used during recording.
- Preserve unchanged: the entire `Hotkeys` class (`src/core/hotkeys.{h,cpp}`), the `settings.json` string format (e.g. `"Ctrl + S"`), `stringToKeyCombo` parsing, and runtime hotkey detection.
- Explicitly out of scope: multi-layout support (QWERTY only), dependency on `mgerhardy/imgui_keyboard`, any change to scancode semantics or persistence.

No breaking changes — storage format, public API, and behavior are identical.

## Capabilities

### New Capabilities

- `hotkey-editor`: Visual presentation requirements for the in-editor keyboard used to rebind hotkeys — keycap rendering layers, icon rules for special keys, style struct defaults, and the preserved-surface contract (filename, layout table, storage model, settings format, `Hotkeys` class) that the visual redesign must not disturb.

### Modified Capabilities

None.

## Impact

- Affected code: `src/utils/imHotKey.h` only (header-only rewrite of the keycap render block inside `Edit`, plus a new `KeyboardStyle` struct in the `ImHotKey` namespace).
- Unaffected code: `src/core/hotkeys.{h,cpp}`, `settings.json` schema, localization keys (`hotkey_editor.*` in `../ingame/languages/en.json`), all callers of `ImHotKey::Edit` / `ImHotKey::GetHotKey` / `ImHotKey::GetHotKeyLib`.
- Dependencies: no additions. Uses `ImDrawList` primitives already available via `imgui.h` / `imgui_internal.h` which this header already includes.
- Risk: low — purely a visual swap inside one function; input/recording logic and persistence untouched.
