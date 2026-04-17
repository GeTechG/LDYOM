## Why

The editor currently has no unified, non-blocking way to surface transient feedback to the user (e.g., "Project saved", "Build failed", "Scene loaded"). Action results are either silently applied, logged to spdlog (invisible in-game), or rely on modal `ImGui::OpenPopup` calls that interrupt workflow. A toast-style notification system would give contextual feedback without blocking the editor, matching the UX of modern creative tools.

[ImGuiNotify](https://github.com/TyomaVader/ImGuiNotify) is a small, actively maintained ImGui-native toast library that fits the project's existing stack (ImGui 1.92, Font Awesome icons, header-only drop-in).

## What Changes

- Vendor **ImGuiNotify** as a `thirdparty/` submodule (same pattern as `ImGuizmo` and `ImNodeFlow`).
- Add its sources and include directory to CMake so `imgui_notify` links into the main `ldyom` target.
- Provide Font Awesome 6 glyph ranges to the ImGui font atlas (the library renders type icons via FA6 codepoints) using existing `src/fonts/` infrastructure.
- Add a thin C++ notification facade (`core/notifications.h`) that wraps `ImGui::InsertNotification` so call sites don't depend on the raw library API — lets us swap the backend later and centralizes defaults (duration, position).
- Render the notification queue each frame from `Application::renderFrames()`, after `WindowManager::render()`, so toasts overlay all editor windows.
- Expose the facade to Lua as `ldyom.notify(type, title, message)` and update the declaration file in `../ingame/definitions/core`.

Not a breaking change — purely additive.

## Capabilities

### New Capabilities

- `notifications`: Toast-style non-modal user feedback (types, titles, messages, duration, rendering order, Lua API).

### Modified Capabilities

None. Existing `ui-scaling`, `entity-orientation`, and `object-scale` specs are unaffected.

## Impact

- **Build**: new `thirdparty/ImGuiNotify` submodule; `CMakeLists.txt` gains a source list and include path; no new Conan requirements.
- **Fonts**: `src/fonts/` font-atlas setup extended to merge Font Awesome 6 glyph range used by ImGuiNotify icons.
- **Runtime**: one new render call per frame (`ImGui::RenderNotifications()` inside a full-screen transparent window). Negligible cost when queue is empty.
- **Code**: new files `src/core/notifications.{h,cpp}` and Lua binding in existing `LuaManager` setup.
- **Addons**: `../ingame/definitions/core/ldyom.lua` (or equivalent declaration file) updated with the new `notify` binding so editor addon authors get autocomplete.
- **Docs**: no user-facing doc file required; Lua declaration serves as the contract.
