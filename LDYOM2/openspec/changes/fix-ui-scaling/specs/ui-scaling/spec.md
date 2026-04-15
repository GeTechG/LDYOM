# UI Scaling Capability

## Purpose

Provide a single, consistent model for scaling the editor's ImGui UI across varying game resolutions and user-chosen display scales.

## Requirements

### R1. Single scale factor

The system SHALL expose a single `uiScale` scalar computed as `screenScale.y * globalScale`, where:

- `screenScale.y = backbufferHeight / 720.0f`
- `globalScale` is the user's `display_scale` setting (range `[0.25, 2.0]`).

`ImGuiConfigurate::getUiScale()` returns this value.

### R2. Automatic font and style scaling

The system SHALL set `ImGui::GetStyle().FontScaleDpi = getUiScale()` once per frame (or on any change). All font rendering and style-variable scaling (padding, rounding, spacing) thereby adapts without per-widget code.

### R3. Framework window scaling

`Window::configurateWindow` SHALL apply `uiScale` to `m_position`, `m_size`, and `m_sizeMin` before calling `SetNextWindowPos` / `SetNextWindowSize` / `SetNextWindowSizeConstraints`.

`m_size` SHALL remain in logical (design-reference) units for the lifetime of the window. Runtime pixel values from `GetWindowSize()` SHALL NOT be written back into `m_size`.

### R4. Live rescale preserves user resize

When `uiScale` changes between frames, `Window::configurateWindow` SHALL set window size for one frame with `ImGuiCond_Always`, computed as `lastPixelSize * (newUiScale / oldUiScale)`. `lastPixelSize` is cached from the prior frame's `GetWindowSize()`.

### R5. Widget sizing convention

Widget authors SHALL size explicit `ImVec2` arguments using, in order of preference:

1. `ui::em(x[, y])` — font-relative, for most layout.
2. Raw value multiplied by `getUiScale()` — for visual elements intentionally decoupled from font (fixed-size icons, thumbnails).
3. Raw pixel value — only for pixel-perfect details (1px separators).

Mixing `* screenScale` with `FontScaleDpi`-driven style values is prohibited — use one of the three forms above.

### R6. Deprecated API

`ImGuiConfigurate::getScreenScale()` and `ImGuiConfigurate::getGlobalScale()` SHALL be marked deprecated. New code SHALL use `getUiScale()` and `ui::em`.

### R7. Settings compatibility

The `display_scale` setting key and its accepted values remain unchanged. Existing user configs continue to work.

## Scenarios

### S1. Fresh install at 100% on 1920×1080

Given `display_scale = 1.0` and backbuffer 1920×1080, when any registered window opens, then its position and size appear identical to the 1280×720 baseline scaled by `1080/720 = 1.5×`.

### S2. User selects 150% at runtime

Given at least one open window sized by the user, when the user selects 150% in QuickSettings, then open windows rescale proportionally in the same frame and no window drifts or misaligns across subsequent frames.

### S3. Low-resolution, high user scale

Given backbuffer 1280×720 and `display_scale = 2.0`, when the entities panel opens, then text, buttons, and sub-regions remain proportional (no clipped text, no overlapping controls).

### S4. Window resized by user, then scale changed

Given the user manually resized a window to 2× its default, when `display_scale` changes from 1.0 to 1.25, then the window's pixel dimensions become approximately the prior pixel size × 1.25, preserving the user's manual resize ratio.

### S5. In-game editing window

Given any `src/rendering/in_game/*` window, when opened at any supported scale combination, then its internal layout (buttons, text wrap, child regions) scales consistently with the rest of the UI.
