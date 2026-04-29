## 1. Settings keys and constants

- [x] 1.1 Decide and document the canonical settings paths used by this change (`onboarding.completed`, `hints.shown.<key>`, demo key `hint.example.<id>`) in a short comment near the new code; do not introduce a schema file
- [x] 1.2 Confirm `Settings::getSetting<bool>(path, false)` returns `false` for missing keys for all three paths above (smoke-check via existing call sites; no code change expected in `src/core/settings.h`)

## 2. Hints subsystem

- [x] 2.1 Create `src/core/hints.h` declaring `class Hints` with `static Hints& instance()`, `bool shouldShow(std::string_view key) const`, `void markShown(std::string_view key)` and a private default constructor
- [x] 2.2 Create `src/core/hints.cpp` implementing the singleton via Meyers-style local static and routing both methods through `Settings::instance()` with the literal `hints.shown.` prefix
- [x] 2.3 Add `src/core/hints.cpp` to the CMake source list so it builds with the rest of `core/`
- [ ] 2.4 Smoke-test in-game: call `Hints::instance().shouldShow("smoke")` (true), then `markShown("smoke")`, then `shouldShow("smoke")` (false), and verify `settings.json` on disk contains `hints.shown.smoke = true` after autosave

## 3. Welcome window

- [x] 3.1 Create `src/rendering/windows/welcome.h` declaring `class Welcome : public Window` with `int m_selectedPreset = -1`, ctor, `~Welcome() override = default`, `void open() override`, and a `static void renderContent(Welcome*)`
- [x] 3.2 Create `src/rendering/windows/welcome.cpp` implementing ~600x400 sizing with a moderate `sizeMin` and centered pivot, wiring `setRenderCallback<Welcome>(renderContent)` in the ctor, and resetting `m_selectedPreset = -1` in `open()`
- [x] 3.3 Implement `renderContent` to render, in order: `welcome.title`, `welcome.intro`, `welcome.preset_choice_explain`, then two side-by-side preset cards using `welcome.preset.simple.*` and `welcome.preset.advanced.*` localization keys; do not add any Skip / Later / Open Project Manager control
- [x] 3.4 Add a private helper `applyPreset(int preset)` that writes the two `editor.entities.auto_attach_objective_specific` and `editor.entities.auto_link_spawn_objective` settings (Simple = both true, Advanced = both false), closes `welcome`, sets `m_onboardingMode = true` on the registered `QuickSettings` instance, and opens `quick_settings`
- [x] 3.5 Wire each preset card's click to `applyPreset(0)` / `applyPreset(1)`
- [x] 3.6 Register the `Welcome` window in `src/rendering/windows/init.cpp` under id `"welcome"` alongside the existing window registrations

## 4. QuickSettings onboarding mode

- [x] 4.1 Add `bool m_onboardingMode = false;` member to `src/rendering/windows/quick_settings.h` plus a public `void setOnboardingMode(bool value)`
- [x] 4.2 In `src/rendering/windows/quick_settings.cpp`, branch the bottom button: when `m_onboardingMode` is true render a button labeled `_("quick_settings.continue")`; otherwise keep the existing Close button untouched
- [x] 4.3 Implement the Continue click handler: `Settings::instance().setSetting<bool>("onboarding.completed", true)`, close `quick_settings`, then `WindowManager::instance().openWindow("project_manager")`
- [x] 4.4 Reset `m_onboardingMode = false` whenever the window closes (override `close()` or hook the existing close path) so a subsequent normal open does not inherit the flag
- [x] 4.5 Verify the existing non-onboarding Close path is byte-for-byte unchanged when `m_onboardingMode == false`

## 5. Onboarding gate in openEditor

- [x] 5.1 In `src/core/application.cpp` lines 56-69, replace the unconditional `openWindow("project_manager")` branch (when `currentProjectIndex == -1`) with: if `Settings::instance().getSetting<bool>("onboarding.completed", false)` is false, open `welcome`; else open `project_manager`
- [x] 5.2 Leave the `currentProjectIndex != -1` path that opens `main_menu` untouched
- [x] 5.3 Confirm no other call site routes to `project_manager` for first-run users (the gate is local to the `openEditor` callback by design)

## 6. Localization

- [x] 6.1 Add the following keys to `../ingame/LDYOM/languages/en.json` with English placeholder copy: `welcome.title`, `welcome.intro`, `welcome.preset_choice_explain`, `welcome.preset.simple.label`, `welcome.preset.simple.description`, `welcome.preset.advanced.label`, `welcome.preset.advanced.description`, `quick_settings.continue`, `hint.example.body`
- [x] 6.2 Add the same keys to `../ingame/LDYOM/languages/ru.json` with Russian placeholder copy
- [ ] 6.3 Verify both files load without JSON parse errors and that `_(...)` resolves each new key in-editor

## 7. Demo hint usage

- [x] 7.1 Pick one existing editor window (e.g. an entity panel) and add a one-shot popup gated by `if (Hints::instance().shouldShow("hint.example.<id>")) { ... }`
- [x] 7.2 Render the popup body via `_("hint.example.body")` and a dismiss button; in the dismiss handler call `Hints::instance().markShown("hint.example.<id>")` and close the popup
- [ ] 7.3 Verify the popup appears on first open of that window after a fresh install and does not reappear after dismissal in the same session or after restart

## 8. Build and smoke test

- [x] 8.1 Run a Debug build (`cmake --build build --config Debug` or the project's standard Debug task) and resolve any compile/link errors
- [ ] 8.2 Launch SA with the freshly built `.asi`, ensuring `settings.json` has neither `onboarding.completed` nor any `hints.shown.*` keys, and confirm: openEditor with no project shows Welcome (not Project Manager)
- [ ] 8.3 Click Simple, verify QuickSettings opens with a Continue button, click Continue, verify Project Manager opens and `settings.json` now contains `onboarding.completed = true` plus the two `editor.entities.*` keys set to true
- [ ] 8.4 Reset the flag to `false` in `settings.json`, repeat with Advanced, confirm the two `editor.entities.*` keys are written as `false`
- [ ] 8.5 Reset the flag, dismiss Welcome via Esc once, confirm next openEditor reopens Welcome with no preset preselected and `onboarding.completed` still `false`
- [ ] 8.6 With `onboarding.completed = true`, confirm openEditor with no project opens Project Manager directly and never shows Welcome
- [ ] 8.7 Confirm the demo hint popup appears once, dismisses, and does not reappear after restart
