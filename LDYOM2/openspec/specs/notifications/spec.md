## ADDED Requirements

### Requirement: Notification facade API

The system SHALL provide a `Notifications` facade in `src/core/notifications.h` that wraps the ImGuiNotify backend. Call sites MUST NOT `#include <imgui_notify.h>` directly; all access goes through the facade. This isolates backend-specific types from the rest of the codebase.

The facade SHALL expose at least:

- `Notifications::push(NotificationType type, std::string title, std::string message, std::chrono::milliseconds duration = kDefaultDuration)`
- Convenience helpers: `success(title, message)`, `info(title, message)`, `warning(title, message)`, `error(title, message)`.
- `NotificationType` enum with values `Success`, `Info`, `Warning`, `Error` mapping to the backend's types.

`kDefaultDuration` SHALL be 3000 ms.

#### Scenario: Caller uses facade only

- **WHEN** any source file under `src/` (excluding `src/core/notifications.cpp`) is grepped for `imgui_notify.h`
- **THEN** no matches SHALL be found; the include is confined to the facade implementation.

#### Scenario: Convenience helper dispatches correctly

- **WHEN** `Notifications::error("Save failed", "disk full")` is called
- **THEN** a notification with `NotificationType::Error`, the given title and message, and `kDefaultDuration` is enqueued for rendering.

### Requirement: Per-frame rendering integration

The notification queue SHALL be rendered once per frame inside `Application::renderFrames()`, **after** `WindowManager::instance().render()`, so toasts draw on top of all editor windows but beneath ImGui's own modal popups as naturally layered by ImGui's draw order.

The system SHALL render notifications only while the editor UI is active (i.e., inside the normal ImGui frame pipeline driven by `ImguiHook`). No background rendering outside the frame callback is permitted.

#### Scenario: Toast renders above regular windows

- **GIVEN** an `Entities` window is open and focused
- **WHEN** `Notifications::info("Hello", "World")` is called during the frame
- **THEN** the toast overlay appears on top of the `Entities` window in the same frame.

#### Scenario: Empty queue is zero-cost

- **WHEN** a frame runs with no pending notifications
- **THEN** no visible overlay is drawn and per-frame CPU overhead from the notification subsystem is negligible (single function call that returns early).

### Requirement: Font Awesome icon glyph coverage

The font-atlas setup in `src/fonts/` SHALL merge the Font Awesome 6 Solid glyph range required by ImGuiNotify's type icons into the default ImGui font. Missing glyphs MUST NOT fall back to tofu boxes in any built-in notification type (Success, Info, Warning, Error).

#### Scenario: All four built-in types render with icons

- **WHEN** one notification of each built-in type is pushed and rendered
- **THEN** each toast displays its type-specific Font Awesome icon without missing-glyph placeholders.

### Requirement: Lua binding

`LuaManager` SHALL bind the facade so addon scripts can emit notifications via:

```lua
ldyom.notify(type, title, message)
ldyom.notify(type, title, message, duration_ms)
```

`type` accepts the string values `"success" | "info" | "warning" | "error"` (case-insensitive). Unknown type strings SHALL produce a Lua error rather than a silent downgrade.

The declaration file at `../ingame/definitions/core/ldyom.lua` (or the equivalent file that exposes the `ldyom` table) SHALL be updated with `---@param` annotations so editor tooling reports correct autocomplete and diagnostics.

#### Scenario: Lua success call reaches the facade

- **WHEN** a Lua addon runs `ldyom.notify("success", "Saved", "project.json")`
- **THEN** a `Success` notification with the given title and message is enqueued.

#### Scenario: Invalid type raises

- **WHEN** Lua code runs `ldyom.notify("fatal", "X", "Y")`
- **THEN** a Lua runtime error is raised and no notification is enqueued.

### Requirement: Vendoring and build integration

ImGuiNotify SHALL be vendored under `thirdparty/ImGuiNotify/` (git submodule or direct checkout), following the pattern used by `thirdparty/ImGuizmo` and `thirdparty/ImNodeFlow`.

`CMakeLists.txt` SHALL:

- Add the library's source file(s) to the `ldyom` target source list.
- Add its include directory via `target_include_directories(... PRIVATE ...)`.
- NOT introduce a new `find_package` or Conan requirement for ImGuiNotify itself.

#### Scenario: Clean build succeeds

- **WHEN** a fresh `conan install` followed by `cmake --build . --config Debug` runs on a clean checkout with the submodule initialized
- **THEN** the build succeeds and `ldyom.asi` links without unresolved symbols related to ImGuiNotify.

#### Scenario: Submodule missing surfaces a clear error

- **GIVEN** the `thirdparty/ImGuiNotify/` directory is absent
- **WHEN** CMake configure runs
- **THEN** configure fails with a message that identifies the missing submodule, not a cryptic "file not found" on an internal header.

### Requirement: UI scale compatibility

Notifications SHALL render at the current `ImGuiConfigurate::getUiScale()` so their size and padding track the rest of the editor. The notification window position and size calculations MUST use `uiScale`-multiplied pixel values consistent with the `ui-scaling` capability (R5 widget-sizing convention).

#### Scenario: Rescale propagates to toasts

- **GIVEN** a toast is visible on screen
- **WHEN** the user switches `display_scale` from 1.0 to 1.5 in QuickSettings
- **THEN** the visible toast's size and padding grow proportionally in the next frame without clipping or drift.
