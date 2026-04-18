## Context

`src/utils/imHotKey.h` is a header-only fork of CedricGuillemet/ImHotKey used by `src/core/hotkeys.{h,cpp}` to let the user rebind shortcuts via an on-screen keyboard modal. Each keycap is drawn with `ImGui::Button` tinted by a single `PushStyleColor(ImGuiCol_Button, ...)`, which produces a flat, homogeneous grid that does not read as a keyboard — pressed state is only a faint tint, there is no bezel/face separation, and special keys (Shift, arrows, Enter, Tab) are rendered as plain text abbreviations ("Shift", "Up", "Ret").

The `mgerhardy/imgui_keyboard` project demonstrates a noticeably nicer look by composing each keycap from `ImDrawList` primitives in three layers (outer border rect, inner "face" rect, centered label) with a dedicated `KeyboardStyle` struct, and by drawing icon glyphs for modifier/navigation keys via draw-list primitives. We want that visual quality, but none of its surrounding infrastructure: we already have a working scancode storage model, a working recording flow, a working `Keys[6][18]` QWERTY table, and a `settings.json` format callers depend on. The scope is therefore a visual swap inside `Edit()`, nothing else.

Stakeholders: every LDYOM user who opens the Hotkey Editor (single surface, single modal, no other consumers).

## Goals / Non-Goals

**Goals:**

- Replace `ImGui::Button`-per-key rendering with `ImDrawList`-primitive keycaps composed of outer rect + inner face rect + centered label, with clear visual distinction between normal / hovered / pressed / recorded states.
- Render special keys (Shift, Up/Down/Left/Right, Enter, Tab) as vector icons (triangles, chevrons, return arrow, double arrow) drawn with `ImDrawList` primitives.
- Introduce a local `KeyboardStyle` struct inside the `ImHotKey` namespace holding colors (normal, pressed, recorded, border, face), outer rounding, inner rounding, border thickness, and face inset. Default-initialized; not wired to `ImGuiStyle`.
- Preserve byte-for-byte the public surface: filename `imHotKey.h`, `Keys[6][18]` table, `Edit` / `GetHotKey` / `GetHotKeyLib` / `GetKeyForScanCode` / `GetKeyForName` / `GetOrderedScanCodes` signatures, scancode-packed-in-`unsigned int` storage, `"Ctrl + S"` settings string format, viewport-adaptive sizing math, and the `ImGuiKey_Tab..ImGuiKey_KeypadEqual` iteration during recording.
- Preserve byte-for-byte the `Hotkeys` class in `src/core/hotkeys.{h,cpp}` and all its callers.

**Non-Goals:**

- Multi-layout support. QWERTY only — no AZERTY/Dvorak/international rows.
- Adding `mgerhardy/imgui_keyboard` or any other new dependency (Conan or vendored).
- User-configurable keyboard theming, exposing `KeyboardStyle` through `ImGuiStyle`, or persisting style overrides.
- Any change to `stringToKeyCombo` parsing, `settings.json` schema, or the runtime hotkey detection path in `GetHotKey`.
- Icon fonts (Font Awesome, Material Icons, etc.) — icons are drawn as vector primitives.
- Renaming the file or restructuring the namespace.

## Decisions

### 1. `ImDrawList` primitives over `ImGui::Button`

**Choice:** Render each keycap with `ImDrawList::AddRectFilled` (outer), `ImDrawList::AddRect` (outer border), `AddRectFilled` again (inner face), and `AddText` for the label. Use `ImGui::InvisibleButton` sized to the outer rect for hit-testing and hover/active state.

**Why:**
- Gives us two overlapping filled rects with different rounding, which is what produces the perceived "bevel" / keycap look. A single `ImGui::Button` cannot paint two rects with different rounding radii.
- Lets us pick per-state colors (normal / hovered / pressed-toggled / recorded-this-session / border / face) independently rather than being locked to `ImGuiCol_Button` / `ButtonHovered` / `ButtonActive`.
- Icon keys (Shift chevron, arrow triangles) need to draw on top of the face rect, which is straightforward on a draw list and awkward inside a `Button` label.

**Alternatives considered:**
- Keep `ImGui::Button`, paint a second rect over it via `GetWindowDrawList()` — works but we still pay for Button's text rendering and state colors we are fighting against, and we cannot get the rounded-inner-face look cleanly.
- `ImGui::ButtonEx` with custom flags — still single-rect, still same colors, same problem.

### 2. Local `KeyboardStyle` struct, not extending `ImGuiStyle`

**Choice:** Declare `struct KeyboardStyle { ImU32 KeyNormal; ImU32 KeyPressed; ImU32 KeyRecorded; ImU32 KeyBorder; ImU32 KeyFace; ImU32 KeyLabel; float Rounding; float FaceRounding; float BorderThickness; float FaceInset; };` at namespace scope in `imHotKey.h`, with a `static const KeyboardStyle` default instance used by `Edit()`.

**Why:**
- The keyboard is one modal on one screen; it does not need to participate in global theming.
- Keeping the struct local keeps `ImGuiStyle` untouched — no upstream-merge pain and no risk of breaking other ImGui surfaces.
- Defaults are fine for now; if a user ever wants to re-theme, adding an overload that takes a `const KeyboardStyle&` is mechanical.

**Alternatives considered:**
- Add `ImGuiCol_KeyboardKeyNormal`, etc., to `ImGuiCol_` — violates ImGui's extension contract and would need to be patched in our vendored copy.
- Accept colors as function parameters — pollutes `Edit()`'s signature, which is frozen.

### 3. Do not pull in `mgerhardy/imgui_keyboard`

**Choice:** Rewrite the keycap block directly in `imHotKey.h`.

**Why:**
- Their project uses its own key-enum; we would need a bidirectional bridge between their enum and our `scanCodePage1` values, plus a bridge for the ImGuiKey iteration used during recording.
- They support multi-layout, which we explicitly don't need.
- Their modal surface, selection list, and "Clear / Set / Done" buttons don't map onto our `Edit()` structure.
- Our header is ~500 lines; porting the three-layer draw is ~60 lines. Adding a dependency for 60 lines is a bad trade.

**Alternatives considered:**
- Vendor only the relevant draw helpers from `imgui_keyboard` as a separate header — still requires bridging enums and is strictly more code than inlining.

### 4. Icons via `ImDrawList` primitives, not an icon font

**Choice:** For Shift, Up, Down, Left, Right, Enter, Tab, draw icons using `AddTriangleFilled`, `AddLine`, and `AddPolyline` on the draw list. All other keys keep their `Key::lib` string as the centered label.

**Why:**
- Zero new dependencies (constraint from proposal).
- The seven icons we need are trivially expressible as 1–3 primitives each: arrows = single filled triangle, Shift = upward chevron (two lines or a triangle), Enter = hook-shape polyline, Tab = two horizontal arrows (two triangles + lines).
- Scales naturally with the viewport-adaptive key size — computed against the face rect, no font-atlas regeneration.
- An icon font would require bundling a TTF, merging it into the existing ImGui font atlas, and picking glyph codepoints — overkill for seven symbols.

**Alternatives considered:**
- Unicode glyphs in the label string (`↑` `↓` `⇧` `⏎` `⇥`) — depends on the active font having those codepoints; current UI font may not, and adding fallbacks is more work than the primitives.
- Inline SVG / bitmap atlas — adds an asset pipeline step for seven icons.

### 5. Hit-testing: `ImGui::InvisibleButton` per key, `DrawList` for visuals

**Choice:** For every key, call `ImGui::InvisibleButton(uniqueId, keySize)` first to consume the cursor rect and get `IsItemHovered()` / `IsItemClicked()`; then draw the three layers and optional icon using `ImGui::GetWindowDrawList()` at the item's min/max.

**Why:**
- Preserves existing click semantics: clicking a key toggles `keyDown[key.scanCodePage1]` exactly as today's Button-click does, so the Clear / Set logic at the bottom of `Edit()` is untouched.
- `InvisibleButton` integrates with ImGui's focus / hover / active tracking — free keyboard navigation and tooltips if we want them later.
- Layout math (SameLine, Indent, per-key offsets from `Keys[y][x].offset`) stays exactly as-is; `InvisibleButton` advances the cursor the same way `Button` does.

**Alternatives considered:**
- Manual `ImRect` hit-test against `ImGui::GetMousePos()` — reimplements what `InvisibleButton` already does and loses hover integration.
- `ImGui::Selectable` with zero styling — introduces selection semantics we don't want and still paints its own background.

## Risks / Trade-offs

- **Viewport scaling at small window sizes** → icons may degrade to 2–4 pixel triangles. **Mitigation:** the modal enforces `minWidth = 960, minHeight = 400` (preserved), and we size icons as a fraction of face-rect height with a floor (e.g. `max(4.0f, faceHeight * 0.4f)`); below that, fall back to the `Key::lib` text.
- **DrawList call count at high DPI / large windows** → ~90 keys × ~5 primitives each = ~450 draw-list calls per frame. **Mitigation:** this is well within ImGui's per-frame budget (it routinely handles thousands); no batching changes needed. Measure only if profiling flags it.
- **Multi-char labels ("Backspace", "Caps Lock", "Space") on narrow keys at small viewport** → text may overflow the face rect. **Mitigation:** clip with `PushClipRect` on the face-rect-minus-label-inset, and right-size with `CalcTextSize` — if text is wider than the face, optionally abbreviate ("Bksp", "Caps"), but only as a fallback; at the enforced min window size this is unlikely. **TODO:** decide abbreviation table during implementation if testing shows overflow at 960×400.
- **Hover/pressed state feedback parity with Button** → users who tap-and-hold expect instant color change. **Mitigation:** use `IsItemHovered()` and `IsItemActive()` to branch to `KeyboardStyle::KeyHover` / overlay, matching ImGui's native feel.
- **Localized labels inside keys (non-ASCII)** → not applicable. The `Keys[][]` table holds ASCII `lib` strings; localization only affects the hotkey name on the left list, which is untouched.

## Migration Plan

Single header swap — no settings migration, no API change, no caller update, no data migration.

1. Edit `src/utils/imHotKey.h`: add `KeyboardStyle` struct + defaults, rewrite the keycap render loop inside `Edit()` (~lines 333–363), add icon-draw helpers as `static` functions in the namespace. Everything outside `Edit()` is untouched.
2. Build Debug (`cmake --build build --config Debug`) and manually verify the Hotkey Editor modal in-game: open modal, click every key type (regular, modifier, arrow, Enter, Tab, Space), record a new combo via keyboard, Set, Done, reopen — verify persisted binding still shows correctly and fires in-game.
3. No data migration: `settings.json` entries from before the change (`"Ctrl + S"`) are read by the unchanged `stringToKeyCombo` and unchanged `Hotkeys` class.

**Rollback:** `git revert` the single commit touching `imHotKey.h`. No data or config cleanup required.

## Open Questions

- Exact pressed-vs-recorded color distinction: should a key that the user clicked in this session look different from a key that is part of the currently-saved binding loaded from `settings.json`? Current behavior collapses them into one `keyDown[]` state. Proposal is to keep a single state (simplest, matches today); if UX testing wants two-tone, add a parallel `recordedThisSession[]` later. **Leaning:** keep single state for v1.
- Abbreviation fallback table for narrow-key overflow at min viewport size — defer until implementation reveals whether it's needed.
