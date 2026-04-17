## 1. Vendor ImGuiNotify

- [x] 1.1 Add `https://github.com/TyomaVader/ImGuiNotify` as a git submodule at `thirdparty/ImGuiNotify` and pin a known-good commit SHA.
- [x] 1.2 Verify the submodule contains `src/imgui_notify.cpp` (or equivalent) and public headers; note their exact paths for CMake wiring.
- [x] 1.3 Add an entry to `.gitmodules` and document the submodule in the root build docs (only if a build doc already exists; do not create a new one).

## 2. Build integration

- [x] 2.1 In `CMakeLists.txt`, define a `IMGUINOTIFY_FILES` list (mirroring the `IMGUIZMO_FILES` / `IMNODEFLOW_FILES` pattern) and append it to the `add_library(${PROJECT_NAME} MODULE ...)` source list.
- [x] 2.2 Add `thirdparty/ImGuiNotify/<include-dir>` to `target_include_directories(${PROJECT_NAME} PRIVATE ...)`.
- [x] 2.3 Add a CMake guard that fails configure with a clear message (e.g., `message(FATAL_ERROR "thirdparty/ImGuiNotify submodule missing — run git submodule update --init")`) when the submodule directory is absent.
- [x] 2.4 Run `cmake --build . --config Debug` on a clean checkout and confirm `ldyom.asi` links.

## 3. Font atlas coverage

- [x] 3.1 Identify the exact Font Awesome 6 codepoints ImGuiNotify uses for Success / Info / Warning / Error icons.
- [x] 3.2 Extend the font-atlas setup in `src/fonts/` to merge those FA6 Solid glyphs into the primary editor font.
- [x] 3.3 Launch the editor; verify the test toasts (added in task 7.1) render their icons without tofu boxes.

## 4. C++ facade

- [x] 4.1 Create `src/core/notifications.h` with `enum class NotificationType { Success, Info, Warning, Error }`, `kDefaultDuration`, and declarations for `push()`, `success()`, `info()`, `warning()`, `error()`.
- [x] 4.2 Create `src/core/notifications.cpp` that includes `imgui_notify.h`, translates `NotificationType` into the backend enum, and forwards arguments.
- [x] 4.3 Add `Notifications::renderAll()` that calls `ImGui::RenderNotifications()` inside the backend's required wrapper, wrapped in `ZoneScoped` (Tracy) for visibility.
- [x] 4.4 Grep confirm `imgui_notify.h` appears in exactly one `.cpp` file (`src/core/notifications.cpp`).

## 5. Render pipeline wiring

- [x] 5.1 Call `Notifications::renderAll()` from `Application::renderFrames()` immediately after `WindowManager::instance().render();`.
- [x] 5.2 Ensure the call is inside the existing `ZoneScoped` region or that `renderAll` has its own zone for Tracy.
- [x] 5.3 Verify an empty queue renders nothing visible and no per-frame allocation churn (quick Tracy trace or visual confirmation).

## 6. UI-scaling compatibility

- [x] 6.1 At runtime, toggle `display_scale` between 1.0 and 1.5 while a toast is visible; confirm size and padding track the rest of the UI.
- [x] 6.2 If toasts do NOT rescale, locate the hardcoded `ImVec2` values in the vendored library and wrap them with `ImGuiConfigurate::getUiScale()` via a local patch; note the patch location for future upgrade PRs.

## 7. Lua binding and declaration

- [x] 7.1 In the LuaManager binding setup (wherever the `ldyom` table is populated), register `ldyom.notify(type, title, message [, duration_ms])`:
  - Parse `type` as `"success" | "info" | "warning" | "error"` (case-insensitive).
  - Raise a Lua error on unknown type strings.
  - Default duration: 3000 ms when omitted.
- [x] 7.2 Add a smoke test at the bottom of a dev Lua file (or use an existing test addon) calling all four types; verify each renders with its icon.
- [x] 7.3 Update the Lua declaration in `../ingame/definitions/core/ldyom.lua` (or the correct declaration file for the `ldyom` table) with `---@param` and `---@overload` annotations for the new `notify` binding.
- [x] 7.4 Update `../ingame/languages/en.json` ONLY if user-visible text is introduced in C++ (titles/messages in editor code). Skip if all editor-side messages come from existing localization keys.

## 8. Editor call sites (first use)

- [x] 8.1 Add at least one real call site in existing editor flows so the change ships with visible value — recommended: success toast on project save (in the project-save path) and error toast on save failure.
- [x] 8.2 Confirm the new call sites use the facade (`Notifications::success(...)`), not the backend directly.

## 9. Verification

- [x] 9.1 Run `openspec verify-change add-imgui-notify` (via `/openspec-verify-change`) and resolve any reported issues.
- [x] 9.2 Manually execute each spec scenario (facade isolation grep, zero-cost empty queue, icon coverage for all four types, Lua success call, Lua invalid-type error, clean build, missing-submodule error, rescale propagation).
- [x] 9.3 Commit with `oco --yes` after `lms server status` confirms LM Studio is running.
