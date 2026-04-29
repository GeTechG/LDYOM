## Context

Today, pressing the `openEditor` hotkey with no project loaded jumps straight to the `project_manager` window (see `src/core/application.cpp` lines 56-69). New users land in a list of projects without any orientation about what LDYOM2 is or how the editor expects them to work. Two behavioral toggles already exist in `quick_settings` and shape the entity-creation experience significantly:

- `editor.entities.auto_attach_objective_specific`
- `editor.entities.auto_link_spawn_objective`

Both default to `true` and are only discoverable by opening Quick Settings and scrolling to the "Entity Creation Defaults" section. There is also no shared mechanism for one-shot popup hints: any window that wants to surface a tip-once-then-never-again must roll its own state.

`Settings` (`src/core/settings.h`) already provides typed `getSetting<T>` / `setSetting<T>` with dot-key paths backed by an `nlohmann::json` document, including auto-save on change. `Window` (`src/rendering/window.h`) and `WindowManager` already provide registration, sizing, pivoting, and a `setRenderCallback<T>` pattern that all existing editor windows (e.g. `ProjectManager`, `QuickSettings`) follow. The infrastructure to add a new first-run window and a settings-backed hint primitive is fully in place; this change is additive and constrained to the existing patterns.

Stakeholders: end users (especially first-time installers), and future contributors who want to add suppressible inline hints to other editor windows.

## Goals / Non-Goals

**Goals:**

- Show a `Welcome` window the first time a user presses `openEditor` with no project loaded and `onboarding.completed=false`, instead of `project_manager`.
- Offer two preset cards (**Simple**, **Advanced**) that write the two existing entity-creation toggles to known values and route the user into Quick Settings to confirm.
- Provide an "onboarding mode" for `QuickSettings` whose only difference from normal mode is the bottom button: it reads "Continue", marks onboarding complete, closes, and opens `project_manager`.
- Lay a small foundation for one-shot popup hints via a `Hints` singleton (`src/core/hints.{h,cpp}`) backed by `Settings` under the `hints.shown.<key>` namespace.
- Demonstrate `Hints` usage with one example popup inside an existing editor window so the pattern is copy-pastable.
- Keep the change additive: no breaking changes to settings schema, no Lua surface changes, no new build dependencies.

**Non-Goals:**

- No Lua bindings for `Hints`, `Welcome`, or onboarding state.
- No resumable onboarding (closing Welcome via Esc/X simply re-shows it next time the gate triggers).
- No "Skip onboarding" button or shortcut.
- No in-place tour overlays, tooltips arrows, or step-by-step coach marks anywhere in the editor.
- No UI to reset hints or re-trigger onboarding (advanced users can hand-edit the settings file).
- No migration of existing user settings; existing installs are treated as already-onboarded only if they already have a project loaded when the gate runs (otherwise they will see Welcome once, by design).

## Decisions

### 1. `Welcome` is a regular `Window`, not a `ModalPopupWindow`

A new class `Welcome : public Window` lives in `src/rendering/windows/welcome.{h,cpp}`, sized ~600x400 with a moderate `sizeMin` and centered pivot. It follows the same shape as `ProjectManager` (member fields for selection state, a static `renderContent` rendered through `setRenderCallback<Welcome>`).

**Rationale:** `Welcome` is a foreground experience but is not blocking inside a parent context the way `quick_settings` is when invoked from `main_menu`. Treating it as a regular registered window means it integrates with `WindowManager::openWindow("welcome")` exactly like `project_manager`, plays well with the existing close-all-windows hotkey behavior, and avoids the modal stack semantics that `ModalPopupWindow` enforces. The visual "modality" of onboarding is achieved by the gate logic (no other window is opened underneath it on first run), not by widget mechanics.

**Alternatives considered:** `ModalPopupWindow` would have given us `OpenPopup`/`BeginPopupModal` semantics, but the onboarding flow needs to *replace* `project_manager` rather than overlay something — there is nothing to overlay yet. Modal semantics also complicate the hand-off to `quick_settings` (closing one popup while opening another).

### 2. Onboarding gate sits in the `openEditor` hotkey callback

The branch in `application.cpp` lines 56-69 becomes:

```
if (currentProjectIndex == -1) {
    if (!Settings::instance().getSetting<bool>("onboarding.completed", false))
        WindowManager::instance().openWindow("welcome");
    else
        WindowManager::instance().openWindow("project_manager");
} else {
    WindowManager::instance().openWindow("main_menu");
}
```

**Rationale:** The hotkey callback is the single chokepoint where editor entry happens with `currentProjectIndex == -1`. Putting the gate there keeps the rest of the codebase oblivious to onboarding state and means any future entry path that goes through `openWindow("project_manager")` directly (e.g. internal navigation) is *not* gated, which matches the user-visible spec ("first mod entry" means hotkey, not internal re-routing).

**Alternatives considered:** Subclassing `ProjectManager::open()` to redirect — rejected because it conflates onboarding with project management and would also fire on internal openings of `project_manager`. A dedicated "router" helper — rejected as overkill for a four-line check.

### 3. `QuickSettings` gains an `m_onboardingMode` flag rather than spawning a separate window

`QuickSettings` keeps its current class identity. A new `bool m_onboardingMode = false` member is set by the Welcome flow before opening (via a public setter, e.g. `setOnboardingMode(true)`), and reset to `false` on close. The bottom button branches on this flag: when true it reads `_("quick_settings.continue")` and on click sets `onboarding.completed=true`, closes, opens `project_manager`. When false it reads the existing `_("quick_settings.close")` and just closes.

**Rationale:** The form *itself* — language, theme, scale, hotkeys, entity defaults — is exactly what we want to show during onboarding. Duplicating the window into a `QuickSettingsOnboarding` would mean keeping two parallel files in sync forever. A single boolean toggling one button label and one click handler is the minimum-invasive way to express "same form, slightly different exit semantics".

**Alternatives considered:** A wholly separate `OnboardingQuickSettings` window — rejected for the duplication cost. Driving the difference via a global "onboarding active" flag — rejected because it leaks transient UI state into a global.

### 4. `Hints` is a singleton thin-wrapping `Settings`, not its own file format

`Hints` lives in `src/core/hints.{h,cpp}` and exposes `instance()`, `shouldShow(key) const`, `markShown(key)`. Internally it just calls `Settings::instance().getSetting<bool>("hints.shown." + key, false)` (negated for `shouldShow`) and `setSetting<bool>(..., true)` for `markShown`.

**Rationale:** `Settings` already gives us atomic, autosaved, dot-keyed boolean storage. There is no reason to invent a parallel hint store. Wrapping it in a singleton with a tiny semantic API (`shouldShow`/`markShown`) keeps call sites readable (`if (Hints::instance().shouldShow("entity_panel_intro")) { ... Hints::instance().markShown(...); }`) and decouples callers from the underlying key prefix, leaving room to swap storage later without touching call sites.

**Alternatives considered:** A standalone JSON file under the LDYOM data folder — rejected because it's a second persistence channel for the same concept. Inlining `Settings` calls at every hint site — rejected because every call site would need to know the `hints.shown.` prefix and the polarity of the bool.

### 5. No resumability — Welcome simply re-opens until a preset is chosen

If the user presses Esc or clicks the X on Welcome, nothing is persisted; `onboarding.completed` stays `false`. The next `openEditor` press with no project will re-open Welcome.

**Rationale:** A resumable model would require either a per-step `onboarding.step` field or partial-state recovery (which preset was tentatively chosen?). Both are more complex than the value they provide. The flow is short (one preset click → one Continue click) and a user who closes it almost certainly wants to start over, not resume.

**Alternatives considered:** A `onboarding.step` enum (`welcome` | `quick_settings` | `done`) — rejected as premature complexity. A "Don't show again" checkbox on Welcome — rejected because it overlaps with the explicit Continue-completes-onboarding semantics and creates a dead-end state where the user is neither onboarded nor signaled they should be.

### 6. Storage layout: `onboarding.completed` and `hints.shown.<key>`

Keys are dot-noted matching the existing `Settings` convention (e.g. `editor.entities.auto_attach_objective_specific`). The two namespaces are:

- `onboarding.completed` — single bool, default `false`.
- `hints.shown.<key>` — bool per hint key, default `false`. `<key>` is a stable string chosen by the call site (e.g. `entity_panel_intro`).

**Rationale:** Mirrors the existing namespace style. Dotted paths translate cleanly to `nlohmann::json::json_pointer` via the existing `Settings::dotToPointer`. Putting all hint flags under one parent (`hints.shown`) keeps the settings file readable and makes future bulk operations (e.g. an internal reset helper, if ever needed) trivially expressible as "delete the `hints.shown` subtree".

**Alternatives considered:** A single `onboarding.hints_shown` array of strings — rejected because it requires custom array handling instead of `getSetting<bool>`. A flat `hint_<key>_shown` naming — rejected as harder to inspect/group.

### 7. `Welcome` class structure mirrors `ProjectManager`

```cpp
class Welcome : public Window {
    int m_selectedPreset = -1;        // -1, 0=Simple, 1=Advanced
    static void renderContent(Welcome* window);
    static void renderPresetCard(Welcome* window, int presetIndex, ...);
public:
    Welcome();
    ~Welcome() override = default;
    void open() override;             // resets m_selectedPreset
};
```

`renderContent` lays out: title (`welcome.title`), intro paragraph (`welcome.intro`), preset choice explanation (`welcome.preset_choice_explain`), then two side-by-side "cards" (selectable `Child` regions or styled buttons) for Simple / Advanced. Clicking a card writes the two `editor.entities.*` settings, closes Welcome, calls `setOnboardingMode(true)` on the `QuickSettings` window instance, and opens it.

**Rationale:** `ProjectManager`'s structure (member state + static render + `setRenderCallback<T>`) is the established codebase convention. Following it makes the new file maximally familiar to maintainers. Resetting transient selection state in `open()` matches `ProjectManager`'s pattern.

## Risks / Trade-offs

- **[Risk]** Existing users who happen to press `openEditor` with no project loaded will see Welcome once. → **Mitigation**: This is acceptable and explicitly noted as in-spec; the flow is short and harmless. Power users can pre-set `onboarding.completed=true` in the settings file.
- **[Risk]** `m_onboardingMode` could be left set on `QuickSettings` if the window is closed via path other than the Continue/Close buttons (Esc, X). → **Mitigation**: Reset `m_onboardingMode` in the existing `close()` override path (or add an override) so the next ordinary `openWindow("quick_settings")` always starts in normal mode.
- **[Risk]** Presets currently set only two settings; if more behavioral toggles are added later, "Simple" and "Advanced" will drift unless explicitly extended. → **Mitigation**: Centralize preset application in a single helper (e.g. `Welcome::applyPreset(Preset)`) so the call site list lives in one place. Long-term, if presets grow, consider a `presets.<name>` settings schema; not needed yet.
- **[Risk]** Localization placeholders ship to users speaking en/ru if final copy is delayed. → **Mitigation**: Keys are scoped under `welcome.*` and `hint.example.*` so a follow-up copy pass is one-file-per-locale and obviously incomplete to translators.
- **[Trade-off]** No skip / no resume keeps the model simple at the cost of forcing every first-run user through the same two clicks. Acceptable given the brevity of the flow.
- **[Trade-off]** Hints are bool-only (no "snooze for N days", no per-version invalidation). Acceptable for v1; future per-key versioning can be added by extending the key (e.g. `hints.shown.entity_panel_intro.v2`) without breaking the API.

## Migration Plan

This change is purely additive at the data layer:

1. New settings keys (`onboarding.completed`, `hints.shown.*`) default to `false` via `Settings::getSetting<bool>(..., false)`. No existing settings are read/written in incompatible ways.
2. Existing `editor.entities.auto_attach_objective_specific` and `editor.entities.auto_link_spawn_objective` continue to be defaulted to `true` everywhere they are read; presets only overwrite them when explicitly chosen.
3. No file-format change, no schema versioning bump, no migration script.
4. **Rollback strategy:** revert the commit. Stale `onboarding.completed` / `hints.shown.*` keys left in user settings are harmless on older builds (they are simply unread). No data loss.

## Open Questions

- None blocking. Final copy for `welcome.*` and `hint.example.body` will be authored in a follow-up pass; placeholders are sufficient for the implementation slice.
