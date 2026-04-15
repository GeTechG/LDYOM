# Design — UI Scaling Fix

## Context

Two independent scale sources exist today:

- `screenScale = ImVec2(width/1280, height/720)` — maps design-reference units to the current game backbuffer.
- `globalScale` — user setting in `{0.25, 0.5, 0.75, 1.0, 1.1, 1.25, 1.5, 1.75, 2.0}`.

Dear ImGui 1.92+ exposes `ImGuiStyle::FontScaleDpi`, which per-window multiplies both fonts and style values (padding, spacing, rounding). It does **not** auto-scale explicit sizes passed to `SetNextWindowSize`, `Button(size)`, `BeginChild(size)`, etc. — those remain the caller's responsibility.

## Scale Model

```
uiScale = screenScale.y * globalScale       // single scalar, chosen once per frame
```

Applied at two levels:

1. **Framework (automatic)**: `ImGui::GetStyle().FontScaleDpi = uiScale` at frame start. Scales fonts and style vars.
2. **Explicit (per caller)**: For any `ImVec2` size/pos passed to ImGui, caller uses either `ui::em(n)` (preferred) or `raw * uiScale` (framework only).

Why `screenScale.y` (not `.x`)? Game aspect ratio can vary; using `.y` keeps vertical layout stable and text readable, which is the dominant concern for form-like UIs.

## `Window::configurateWindow` Rewrite

Current bug: `m_size` is both the user's logical setting *and* a cache of `GetWindowSize()` pixels. The two meanings collide.

New contract: `m_size` is **always logical units** (design-reference pixels), set once by the window author, never overwritten from runtime queries.

```cpp
void Window::configurateWindow() {
    const float s = ImGuiConfigurate::getUiScale();

    ImGuiCond sizeCond = ImGuiCond_FirstUseEver;
    ImVec2 nextSize = m_size * s;

    if (m_lastUiScale > 0.0f && m_lastUiScale != s) {
        // Live rescale: preserve user's manual resize by scaling current rect.
        // Uses GetWindowSize via a stored pixel cache updated in render().
        sizeCond = ImGuiCond_Always;
        const float ratio = s / m_lastUiScale;
        nextSize = ImVec2(m_lastPixelSize.x * ratio, m_lastPixelSize.y * ratio);
    }
    m_lastUiScale = s;

    ImGui::SetNextWindowPos(m_position * s, m_positionCondition, m_pivot);
    ImGui::SetNextWindowSize(nextSize, sizeCond);
    ImGui::SetNextWindowSizeConstraints(m_sizeMin * s, ImVec2(FLT_MAX, FLT_MAX));
}

// In render(), after Begin():
m_lastPixelSize = ImGui::GetWindowSize();   // cached for next live-rescale only
```

Key change: `GetWindowSize()` is cached into `m_lastPixelSize`, never back into `m_size`.

## `ui::em` Helper

```cpp
// src/utils/ui_scale.h (or rendering/imgui_widgets/ui_scale.h)
namespace ui {
    inline float em(float n)             { return ImGui::GetFontSize() * n; }
    inline ImVec2 em(float x, float y)   { return {em(x), em(y)}; }
}
```

Usage convention:

| Case | Pattern |
|---|---|
| Widget size (Button, Image) | `ui::em(8, 0)` instead of `ImVec2(120, 0)` |
| Child region size | `ui::em(0, -2.5f)` (0 = fill width, negative = from bottom) |
| Column width | `ImGui::SetNextItemWidth(ui::em(6))` |
| Window pos/size in `Window::configurateWindow` | `m_position * uiScale` (framework only) |
| 1px separator/line | keep raw `1.0f` |
| Icon thumbnail (fixed visual size) | `128.0f * uiScale` (not font-relative — decouples from text) |

## Calibration

Convert existing pixel values using 1em ≈ 16px (default font size at 1.0 scale in the codebase today, since `fontSize = height/48` and `height=720 → 15`). A simple rule for migration: `new = old / 16`, then round to a human-friendly number (e.g., `450 → em(28)`, `120 → em(7.5)`).

## Decisions

- **Stop persisting user-resized window size** — ephemeral is fine (user Q1 confirmed). Simplifies model.
- **Keep option #2 for live rescale** — scale current pixel rect by uiScale delta, preserving manual resize.
- **Keep `ImGuiConfigurate::getScreenScale()` / `getGlobalScale()` for now** — mark deprecated, migrate call sites to `getUiScale()` / `ui::em` incrementally.
- **Font-relative as default** over explicit `uiScale` for widget sizes — matches existing codebase drift (`object_selector`, `scene_settings`, `carrec_path_editor` already do this) and is the Dear ImGui idiom.

## Non-goals

- Changing the `display_scale` setting schema or values.
- Supporting per-monitor DPI (Windows multi-monitor DPI awareness) — out of scope.
- Saving user window sizes across sessions.
- Touching ImGuizmo or ImNodes layouts (they have their own scale handling).
