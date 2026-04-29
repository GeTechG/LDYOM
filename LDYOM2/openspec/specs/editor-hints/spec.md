# editor-hints Specification

## Purpose
TBD - created by archiving change add-onboarding-flow. Update Purpose after archive.
## Requirements
### Requirement: Hints singleton API

The system SHALL provide a `Hints` class in `src/core/hints.{h,cpp}` exposing a singleton accessor `Hints::instance()` and two member functions: `bool shouldShow(std::string_view key) const` and `void markShown(std::string_view key)`. The class SHALL have no public reset method and SHALL NOT expose any other mutators or persistence APIs in its first version.

#### Scenario: Singleton is reachable and stable

- **WHEN** any caller invokes `Hints::instance()` more than once during a session
- **THEN** every call MUST return a reference to the same instance
- **AND** the public surface MUST consist only of `shouldShow(key)` and `markShown(key)`

#### Scenario: shouldShow returns true for an unseen hint

- **WHEN** `hints.shown.<key>` is absent from `settings.json` (and has not been written this session)
- **AND** a caller invokes `Hints::instance().shouldShow("<key>")`
- **THEN** the call MUST return `true`

### Requirement: markShown persists hint suppression via Settings

`markShown(key)` SHALL persist the boolean `hints.shown.<key> = true` through the `Settings` system using the same path that other settings use, so that the value survives editor restarts. Subsequent calls to `shouldShow(key)` for the same key SHALL return `false`. The persistence MUST use the literal namespace prefix `hints.shown.` followed by the key as supplied (no rewriting, no escaping beyond what `Settings` already performs).

#### Scenario: markShown flips shouldShow to false

- **WHEN** `Hints::instance().shouldShow("hint.example")` returns `true`
- **AND** the caller invokes `Hints::instance().markShown("hint.example")`
- **THEN** the next call to `Hints::instance().shouldShow("hint.example")` MUST return `false`
- **AND** `Settings::getSetting<bool>("hints.shown.hint.example", false)` MUST return `true`

#### Scenario: Suppression survives editor restart

- **WHEN** `markShown("hint.example")` was invoked in a previous session and `Settings::save()` ran
- **AND** the editor restarts and reads `settings.json`
- **THEN** `Hints::instance().shouldShow("hint.example")` MUST return `false` from the very first call this session
- **AND** the persisted JSON MUST contain `hints.shown.hint.example = true`

### Requirement: No reset UI

The editor SHALL NOT expose any built-in user interface to reset, clear, or re-show suppressed hints. Users wishing to reset hints SHALL do so by manually editing `settings.json` (removing or setting to `false` the relevant `hints.shown.*` entries). No "Reset hints" button, menu, or hotkey SHALL be added by this change.

#### Scenario: No reset control exists in any window

- **WHEN** the editor is running with one or more `hints.shown.*` keys set to `true`
- **THEN** no editor window or menu MUST contain a control whose action is to clear, delete, or reset entries under the `hints.shown.` namespace
- **AND** the only supported way to reset hints MUST be manual edit of `settings.json`

### Requirement: Reference demo usage in an editor window

At least one existing editor window SHALL invoke the `Hints` API as a reference pattern for future hints. The demo SHALL use a key in the `hint.example.*` namespace and SHALL pull its body text from the `hint.example.body` localization key. The demo SHALL behave correctly under the singleton contract: it appears the first time the relevant view is shown after a fresh install (when `shouldShow` is `true`), the user's dismissal calls `markShown`, and after dismissal the popup MUST NOT reappear.

#### Scenario: Demo hint appears once and is then suppressed

- **WHEN** a fresh install opens the editor window that hosts the demo hint for the first time
- **THEN** the demo popup MUST be shown (driven by `shouldShow("hint.example.<id>") == true`)
- **AND** the popup body text MUST come from the `hint.example.body` localization key
- **WHEN** the user dismisses the popup
- **THEN** `markShown("hint.example.<id>")` MUST be invoked
- **AND** the popup MUST NOT be shown again on subsequent opens of that window in this session or after restart
