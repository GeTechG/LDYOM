## Context

LDYOM2 is a 32-bit ASI plugin hooking into GTA SA's D3D9 pipeline. ImGui is the sole UI toolkit; build deps are split between Conan-managed libraries (e.g., `imgui`, `spdlog`, `glm`) and vendored git submodules under `thirdparty/` (`ImGuizmo`, `ImNodeFlow`, `sol2`, `LuaJIT`). Windows are managed through a central `WindowManager` that owns registration and rendering order, and the per-frame root is `Application::renderFrames()` called from the D3D9 `EndScene` hook.

The editor already relies on Font Awesome glyphs for iconography elsewhere (see `src/fonts/`), and it exposes a growing Lua API via `LuaManager` / sol2 for user addons. There is currently no way for the editor (or addons) to deliver transient, non-blocking feedback to the user — everything is either spdlog output (invisible in-game), a modal popup, or nothing at all.

[ImGuiNotify](https://github.com/TyomaVader/ImGuiNotify) is MIT-licensed, header-heavy with one `.cpp`, and has no external dependencies beyond ImGui itself and Font Awesome codepoints for its type icons. Its API is `ImGui::InsertNotification(...)` plus a `ImGui::RenderNotifications()` call once per frame.

## Goals / Non-Goals

**Goals:**

- Add toast-style notifications that render over all editor ImGui windows.
- Keep the ImGuiNotify backend dependency isolated behind a local facade so we can replace or extend it later without ripping through call sites.
- Expose the facade to Lua so addon authors (core addon + user addons) can emit notifications.
- Integrate with the existing `ui-scaling` capability so toasts honor the user's `display_scale`.
- Zero-cost when the notification queue is empty.

**Non-Goals:**

- A full notification center / history panel. We render transient toasts only; if we want persistence later, it goes in a separate change.
- Custom notification types beyond the four built-ins (Success / Info / Warning / Error) in v1. Can be added incrementally.
- Notifications during project playback cutscenes (ProjectPlayer coroutines) — the scope is the editor's ImGui frame. Playback-only feedback stays out of this change.
- Addon-authored custom icons or colors — only the library's built-in styling.

## Decisions

### D1. Vendoring strategy: git submodule under `thirdparty/`

Choose submodule over Conan package.

**Why**: ImGuiNotify is not a Conan-ready package (no `conanfile`, no tagged Conan-center recipe at the time of writing). The project already has a working precedent for header-ish ImGui add-ons vendored this way (`thirdparty/ImGuizmo`, `thirdparty/ImNodeFlow`). Adding a custom Conan recipe for one small library is overhead; the submodule path gives us pinned SHA, fast iteration, and consistent CMake wiring.

**Alternative considered**: Writing a local Conan recipe. Rejected for the same reason: too much scaffolding for a ~1-file library that we will upgrade rarely.

### D2. Thin facade in `src/core/notifications.{h,cpp}`

Introduce `Notifications::push(...)`, `success/info/warning/error`, and an `enum class NotificationType`. All `imgui_notify.h` includes live in `notifications.cpp` only.

**Why**: Two reasons.

1. **Backend substitution freedom** — if ImGuiNotify goes unmaintained, we can swap backends by rewriting a single `.cpp`. A dozens-of-call-sites migration is not something we want to sign up for.
2. **Stable Lua API** — we want `ldyom.notify(...)` to be a promise we can keep across backend changes. Binding `ImGui::InsertNotification` directly would leak a third-party symbol into addon contract surface.

**Alternative considered**: Call `ImGui::InsertNotification` directly. Rejected — the cost of writing a trivial facade is much smaller than the cost of a future backend swap.

### D3. Render once per frame, after `WindowManager::render()`

`Application::renderFrames()` gains a single trailing call `Notifications::renderAll()` which internally calls `ImGui::RenderNotifications()` inside the library's required wrapper window.

**Why**: Layering order in ImGui is dominated by frame-order; calling last means toasts naturally draw above the editor's normal windows. We deliberately do not try to top modals — ImGui modal dimming should still occlude toasts, which matches user expectations ("a blocking dialog is more important than a transient toast").

**Alternative considered**: A dedicated `Window` registered in `WindowManager`. Rejected because the notification overlay is a full-screen transparent canvas with no interaction — the `Window` abstraction (position, size, resize memory) doesn't apply and would add complexity.

### D4. Font atlas: merge Font Awesome 6 Solid into the primary font

Extend the font loading in `src/fonts/` to include an FA6 Solid range that covers the codepoints ImGuiNotify uses for its type icons (check, info-circle, exclamation-triangle, ban / times-circle). We already ship FA6 elsewhere in the editor, so the glyph data is on hand.

**Why**: ImGuiNotify's default styling prints FA codepoints as part of the title line. Without glyph coverage, users would see tofu boxes next to every toast. Merging into the primary font (instead of using a dedicated "icon-only" font pushed per-toast) keeps the library working out-of-the-box without touching its internals.

**Alternative considered**: Patch ImGuiNotify to drop icons. Rejected — we want the icons, and patching a vendored lib invites merge pain on upgrade.

### D5. UI-scaling integration via existing FontScaleDpi path

Because ImGuiNotify sizes itself using ImGui style values (padding, rounding, window size in `ImVec2`), and the project already routes scale through `ImGui::GetStyle().FontScaleDpi` every frame (see `ui-scaling` R2), toasts inherit scaling automatically — **as long as** the notification code does not hardcode `ImVec2` values bypassing style. We will verify this at implementation time; if the vendored library has hardcoded sizes, we wrap them with `getUiScale()` via a small local patch or a PR upstream.

**Why**: Aligns with existing convention. Avoids a parallel scaling path.

### D6. Lua API shape: `ldyom.notify(type, title, message [, duration_ms])`

`type` is a string, not an integer or a sub-table, so Lua addon code reads naturally.

**Why**: Consistent with how other Lua APIs in the project accept string discriminators (component/objective type names). Strings are self-documenting at call sites; `ldyom.notify("error", ...)` beats `ldyom.notify(3, ...)`.

**Alternative considered**: Expose a `NotificationType` enum as a Lua table. Rejected for v1 — adds a new symbol to the `ldyom` table for a four-value set; string is simpler. If diagnostics push us toward an enum later (for LSP completion), that's a backwards-compatible addition.

## Risks / Trade-offs

- **Library maintenance risk**: ImGuiNotify is a personal project with sporadic commits. → Mitigated by D2 (facade) — swap cost is one `.cpp`. Pin a known-good SHA in the submodule.
- **ImGui version drift**: ImGuiNotify may lag behind ImGui 1.92's docking/API changes. → We pin the submodule and test the clean Debug build before merging. If it doesn't compile against 1.92, we either cherry-pick fixes or fall back to a different library (alternatives: `imgui_toast` fork, or hand-rolling ~200 lines — it's not a large feature).
- **UI-scaling corner**: If the vendored library writes raw `ImVec2` values that dodge `FontScaleDpi`, toasts will look wrong at non-1.0 scales. → Detected at manual test time (S2-style scenario from the spec). Mitigation: local patch wrapped with `getUiScale()`, or small upstream PR.
- **Font atlas size**: Merging more FA6 glyphs slightly increases the atlas texture. → Negligible for a ~4-glyph set; no action needed.
- **Lua error semantics**: Raising on unknown `type` strings is stricter than silently coercing to `Info`. → Intentional: silent coercion hides typos in addon code. Document in the Lua declaration.

## Migration Plan

None required — this is purely additive. No existing feature depends on its absence. Rollback is "delete the submodule folder, revert the CMake diff, remove `notifications.{h,cpp}`."

## Open Questions

- Should the notification position be configurable (top-right vs. bottom-right) via the Settings system now, or deferred? → Recommend deferred; ship with the library default (top-right) and revisit once real usage surfaces a preference.
- Do we want a short visible-in-Tracy zone (`ZoneScoped`) around `renderAll` for performance visibility? → Yes, trivial to add given `ZoneScoped` is already used in `renderFrames`. Included in the task list.
