# Tasks — Fix UI Scaling

## 1. Foundation

- [x] 1.1 Add `ImGuiConfigurate::getUiScale()` returning `screenScale.y * globalScale`.
- [x] 1.2 Set `ImGui::GetStyle().FontScaleDpi = getUiScale()` in `ImGuiConfigurate::update` (instead of just `globalScale`) and refresh on `setGlobalScale`.
- [x] 1.3 Create `ui::em(float)` and `ui::em(float, float)` helpers in `src/utils/ui_scale.h` (or equivalent location). Include `<imgui.h>`.
- [x] 1.4 Mark `getScreenScale()` / `getGlobalScale()` as deprecated in a comment; keep for transition.

## 2. Fix `Window` framework

- [x] 2.1 In `src/rendering/window.h`: add `ImVec2 m_lastPixelSize = {0,0}`; rename `m_lastGlobalScale` → `m_lastUiScale`.
- [x] 2.2 In `src/rendering/window.cpp::configurateWindow`: switch to single `uiScale` factor; live-rescale uses `m_lastPixelSize * ratio`.
- [x] 2.3 In `src/rendering/window.cpp::render`: cache `ImGui::GetWindowSize()` into `m_lastPixelSize`. **Do not** write it into `m_size`.
- [x] 2.4 Verify derived classes (`ModalPopupWindow` etc.) still behave correctly — `ModalPopupWindow::render` had the same feedback bug, fixed to cache into `m_lastPixelSize`.

## 3. Audit windows under `src/rendering/windows/`

- [x] 3.1 `entity_info_panel.cpp` — window pos/padding → `uiScale`; `adaptiveWidth`/`iconWidth`/`textOffset` → `ui::em`. Unused `screenScale` locals removed.
- [x] 3.2 `entities.cpp` — `SetNextWindowPos(DisplaySize, pivot(1,1))` pins to corner (no offset), OK. Child size and Dummy height → `ui::em`. Unused `screenScale` removed.
- [x] 3.3 `objectives.cpp` — same corner-pin pattern, OK. Unused `screenScale` removed.
- [x] 3.4 `quick_settings.cpp` — `(450, 250)` size and `(640, 360)` pos are logical, kept as-is.
- [x] 3.5 Audit: `carrec_paths`, `global_vars_window`, `entities_menu`, `main_menu`, `scenes` migrated to `ui::em` (widget sizes) and raw style vars (style-var paddings — auto-scaled by `FontScaleDpi`). `scene_settings`, `project_manager`, `node_editor`, `tools_window`, `init`, `info_window`, `addons_settings`, `create_new_*` — no hardcoded pixel scaling found; clean.

## 4. Audit in-game editing windows (`src/rendering/in_game/`)

- [x] 4.1 `actor_paths_editing.cpp` — content sizes to `ui::em`.
- [x] 4.2 `carrec_path_editor.cpp` — already uses `GetFontSize()`; no-op.
- [x] 4.3 `cutscene_objective_editing.cpp`.
- [x] 4.4 `object_paths_editing.cpp`.
- [x] 4.5 `object_transform_editing.cpp`.
- [x] 4.6 `vehicle_manual_editing.cpp` — already uses `GetFontSize()`; no-op.
- [x] 4.7 `debug_info.cpp` — no scaling present; no-op.

## 5. Audit popups (`src/rendering/popups/`)

- [x] 5.1 `object_selector.cpp` — replace `ImVec2(600, 500)` (line 102); keep `displaySize * 0.6` but verify behavior at non-1.0 globalScale; icon `128.f * scale` → `128.f * scale * uiScale`.
- [x] 5.2 `weapon_selector.cpp`, `skin_selector.cpp`, `vehicle_selector.cpp` — same treatment as object_selector.

## 6. Audit widgets

- [x] 6.1 `imgui_widgets/imgui_widgets.cpp:179` — `SCL_PX * 400` review; convert to `ui::em`.
- [x] 6.2 `theme_loader.cpp` — removed `* screenScale` multipliers (fixes double-scaling bug with `FontScaleDpi`).

## 7. Validation

- [x] 7.1 Build Debug and launch.
- [x] 7.2 Test matrix: display_scale ∈ {0.5, 1.0, 1.5, 2.0} × game resolution ∈ {1280×720, 1920×1080, 2560×1440}.
- [x] 7.3 For each combo, open: ProjectManager, MainMenu, Entities, Objectives, EntityInfoPanel, QuickSettings, SceneSettings, a selector popup (object/skin/weapon/vehicle), one in-game editing window. Check positioning, size, no overflow/clipping.
- [x] 7.4 Change `display_scale` at runtime from QuickSettings. Verify all open windows rescale without misalignment or drift.
- [x] 7.5 Manually resize a window, then change display_scale — verify manual size is preserved (scaled proportionally).

## 8. Cleanup

- [x] 8.1 No dead `m_lastGlobalScale` references remain (verified via grep).
- [ ] 8.2 Commit with `oco --yes` (ensure `lms server status` first).
