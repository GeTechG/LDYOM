---
change: add-onboarding-flow
updated: 2026-04-29T10:10:00Z
status: paused
---

## In-Flight
none — all code changes implemented; remaining tasks are in-editor playtest verifications

## Decisions
- [task 2.3] CMake uses `file(GLOB_RECURSE SRC_FILES "src/*")` (root CMakeLists.txt:58) — new .cpp files under src/ auto-built; cmake reconfigure required to refresh GLOB.
- [task 4.x] QuickSettings exposes `setOnboardingMode(bool)` and `isOnboardingMode() const`; static `renderContent(Window*)` does `static_cast<QuickSettings*>(window)`.
- [task 4.4] `QuickSettings::close()` resets `m_onboardingMode = false` then calls `ModalPopupWindow::close()`.
- [task 3.x] `Welcome : public Window` (per spec verbatim, NOT ModalPopupWindow). Uses `WindowManager::instance().getWindowAs<QuickSettings>("quick_settings")` (returns `std::optional<QuickSettings*>`).
- [task 3.x] `Welcome::applyPreset` mirrors the `project_manager.cpp:67` / `main_menu.cpp:57` pattern: open QuickSettings on top, do NOT close welcome. The `QuickSettings` Continue handler is responsible for closing welcome before opening project_manager. UX bonus: if user dismisses QS via Esc, they land back on welcome and can pick again.
- [task 6.x] Real localization path: `../ingame/LDYOM/languages/{en,ru}.json` (task spec correct; CLAUDE.md "../ingame/languages/" is stale).
- [task 7.x] Demo hint placed in `EntitiesWindow` (`src/rendering/windows/entities.cpp`); hint id = `hint.example.entities_first_open`; one-shot guard = `bool m_hintRequested` member; dismiss button reuses existing `_("ok")` key (no new localization for the button).
- [post-test pivot, second iteration — flaky click detection] Reverted preset cards from `BeginChild` + `IsItemClicked()` (after `EndChild`) to `InvisibleButton` + manual `drawList` rendering. The BeginChild/IsItemClicked pattern is known-flaky in ImGui because child windows aren't reliably treated as a single hoverable "item" — clicks were dropped intermittently, which the user described as "works every other time, as if it reopens onboarding". The InvisibleButton overlay is rock-solid for click detection; visuals are drawn via `drawList->AddRectFilled/AddRect/AddText` over the invisible button.
- [post-test pivot] Original Welcome→QuickSettings handoff tried to close welcome synchronously and open QS deferred — created an impedance mismatch. Pivoted to the established `project_manager` / `main_menu` pattern: parent stays open, modal opens on top, Continue handler closes parent. Final implementation:
  1. `ModalPopupWindow::open()` also sets `m_open=true` synchronously alongside `m_needOpen=true` (`src/rendering/modal_popup_window.cpp`). Defensive robustness fix — kept regardless of the pivot.
  2. `Welcome::applyPreset` only writes settings, sets onboarding mode, and opens QS. Does NOT close itself. (Reverted the m_pendingClose / render() override approach.)
  3. `QuickSettings`'s onboarding-mode Continue handler now also calls `WindowManager::instance().closeWindow("welcome")` before opening project_manager.
  4. `QuickSettings::render()` override (new) detects ImGui-driven dismissal (Esc/X bypassing our `close()` override via `&m_open` ref) and resets `m_onboardingMode` so a subsequent normal open of QS doesn't inherit the onboarding flag. Addresses the design.md risk note line 119.

## Gotchas
- After adding any new .cpp under src/, `cmake -B build` reconfigure is required (GLOB).
- `Settings::getSetting<T>(key, default)` returns the supplied default when the key is absent (settings.h:43-53).
- `modal_popup_window.h` lives at `src/rendering/modal_popup_window.h`, NOT `src/rendering/windows/`. clangd/LSP reports false-positive "file not found" without compile_commands.json — MSVC build is the source of truth; treat clangd diagnostics as noise.
- `Window::close()` is virtual (modal_popup_window.h:14).
- ru.json is intentionally a partial translation — pre-existing keys absent from ru.json fall back to en.json. We did NOT backfill unrelated missing keys.

## Open Questions
none

## Next Steps
All implementation tasks (1.1–7.2, 8.1) complete. Remaining tasks require running the game and verifying in-editor:
1. **Task 2.4** — In-game smoke-test of `Hints::shouldShow`/`markShown` with key "smoke" and verify `settings.json` has `hints.shown.smoke = true` after autosave.
2. **Task 6.3** — Verify both en.json and ru.json load without parse errors and `_(...)` resolves each new key.
3. **Task 7.3** — Verify the demo hint popup appears once on first `EntitiesWindow` open after fresh install; does not reappear after dismissal.
4. **Tasks 8.2–8.7** — Onboarding-flow scenarios:
   - 8.2 Fresh `settings.json` (no `onboarding.completed`, no `hints.shown.*`) → openEditor with no project shows Welcome (not Project Manager).
   - 8.3 Click Simple → QuickSettings opens with Continue → Continue → Project Manager opens; `settings.json` now has `onboarding.completed = true` plus the two `editor.entities.*` keys = true.
   - 8.4 Reset flag, repeat Advanced; the two `editor.entities.*` keys = false.
   - 8.5 Reset flag, dismiss Welcome via Esc → next openEditor reopens Welcome with no preset preselected; `onboarding.completed` still false.
   - 8.6 With `onboarding.completed = true`, openEditor with no project goes straight to Project Manager.
   - 8.7 Demo hint popup appears once, dismisses, never reappears.
