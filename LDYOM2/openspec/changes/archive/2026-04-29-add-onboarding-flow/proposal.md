## Why

New LDYOM2 users land directly in the Project Manager with no orientation, and the editor exposes advanced behavioral toggles (auto-attach objective-specific components, auto-link spawn objectives) whose defaults strongly shape the editing experience but are invisible until discovered. First-run users have no signposted path to pick a sensible starting profile, and there is no shared mechanism for one-shot in-editor hints that should appear once and never again. Adding a guided onboarding flow plus a hint-suppression primitive lowers the entry barrier and lays groundwork for contextual help across editor windows.

## What Changes

- Add a `Welcome` window (medium ~600x400, non-modal `Window` subclass) shown on the first `openEditor` press when no project is loaded and onboarding is incomplete. It explains the editor briefly and presents two preset cards: **Simple** and **Advanced**.
- Gate the existing `openEditor` hotkey callback in `application.cpp`: when `currentProjectIndex == -1` and `onboarding.completed` is false, open `welcome` instead of `project_manager`. All other entry paths are unchanged.
- Selecting a preset writes editor defaults via `Settings`:
  - **Simple** -> `editor.entities.auto_attach_objective_specific = true`, `editor.entities.auto_link_spawn_objective = true`
  - **Advanced** -> both `false`
  Then closes Welcome and opens `quick_settings` in a new **onboarding mode**.
- Extend `QuickSettings` with an `m_onboardingMode` flag. In that mode the bottom button reads "Continue"; clicking it sets `onboarding.completed = true`, closes the window, and opens `project_manager`.
- Closing Welcome via Esc/X is non-resumable: the next `openEditor` simply reopens Welcome (no skip, no partial-progress state).
- Introduce a `Hints` singleton (`src/core/hints.{h,cpp}`) with `instance()`, `shouldShow(key) const`, and `markShown(key)`, persisted under the `hints.shown.<key>` namespace in `Settings`. Demonstrate the pattern with one example popup hint inside an existing editor window.
- Add localization keys for Welcome copy, the "Continue" label, and the example hint body to `en.json` and `ru.json` (placeholder text, final copy to be authored separately).
- No Lua bindings, no UI for resetting hints, no migration of existing settings.

## Capabilities

### New Capabilities

- `onboarding-flow`: First-run experience that gates editor entry behind a Welcome window, applies a chosen preset (Simple/Advanced) to editor default settings, walks the user through `quick_settings` in onboarding mode, and persists a one-shot completion flag.
- `editor-hints`: Suppressible-hint primitive (`Hints` singleton + `hints.shown.*` settings convention) that any editor window can use to show a popup or note exactly once per user.

### Modified Capabilities

None. `quick_settings` is not currently covered by an existing spec, so its onboarding-mode behavior is captured under `onboarding-flow`.

## Impact

- **New code**: `src/rendering/windows/welcome.{h,cpp}`, `src/core/hints.{h,cpp}`, registration entry in `src/rendering/windows/init.cpp`.
- **Modified code**: `src/core/application.cpp` (openEditor gate), `src/rendering/windows/quick_settings.{h,cpp}` (onboarding-mode flag + Continue button + completion side effect), one editor window file (demo `Hints` usage).
- **Settings keys**:
  - New: `onboarding.completed` (bool, default `false`); `hints.shown.<key>` namespace (bool, default `false`).
  - Written by presets (existing keys, no schema change): `editor.entities.auto_attach_objective_specific`, `editor.entities.auto_link_spawn_objective`.
- **Localization**: new keys in `../ingame/LDYOM/languages/en.json` and `ru.json` (`welcome.*`, `quick_settings.continue`, `hint.example.body`).
- **Dependencies / build**: none. No Lua binding changes, no third-party additions.
- **User-visible behavior**: existing users with projects loaded or with `onboarding.completed=true` see no change. New installs see Welcome -> preset -> quick_settings (Continue) -> project_manager on first editor open.
