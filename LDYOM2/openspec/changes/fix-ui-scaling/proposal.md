# Fix UI Scaling for Non-100% Display Scale

## Why

The editor's UI breaks at display scales other than 100%. Windows land in wrong positions, sizes don't adapt cleanly, and content inside several windows uses raw pixel values that ignore the user's `display_scale` setting. Root cause: the scale model conflates two independent factors (`screenScale` from game resolution, `globalScale` from user zoom) and applies them inconsistently across the codebase.

On top of that, `Window::configurateWindow` has a feedback bug where `GetWindowSize()` (post-scale pixels) is written back into `m_size` and then re-multiplied by `screenScale` on the next frame, causing progressive drift.

## What Changes

1. **Collapse two scale factors into one `uiScale`** = `screenScale.y * globalScale`. Apply via `ImGui::GetStyle().FontScaleDpi = uiScale`, letting ImGui auto-scale fonts and style vars per-window.

2. **Fix `Window::configurateWindow` feedback bug.** Stop writing `GetWindowSize()` into `m_size`. Keep the ratio-based live-rescale logic (preserve user's manual resize when globalScale changes) but drive it off the single `uiScale` delta.

3. **Adopt a sizing convention**:
   - **Font-relative (`ui::em(n)`)** — default for layout (widget sizes, child heights, padding between items).
   - **`uiScale` factor** — only for framework-level window positioning in `Window::configurateWindow`.
   - **Raw pixels** — only for pixel-perfect details (1px separators, line thicknesses).

4. **Audit and migrate all windows** to the new convention. Replace hardcoded pixel sizes and ad-hoc `* screenScale` multiplications.

## Impact

- **Affected code**: `src/rendering/window.{cpp,h}`, `src/imgui_hook/utils/imgui_configurate.{cpp,h}`, all files under `src/rendering/windows/`, `src/rendering/in_game/`, `src/rendering/popups/`, `src/rendering/imgui_widgets/`.
- **New utility**: `ui::em(float)` / `ui::em(float, float)` helper (likely in `src/rendering/imgui_widgets/` or a new `src/utils/ui_scale.h`).
- **User-visible**: windows position and size correctly at 25%–200% scale on 1280×720, 1920×1080, and 2560×1440 resolutions.
- **Breaking**: no public API change; `ImGuiConfigurate::getScreenScale()` and `getGlobalScale()` remain for backward compatibility during migration but become discouraged for new code.
- **Settings**: no new keys; existing `display_scale` is reused.
