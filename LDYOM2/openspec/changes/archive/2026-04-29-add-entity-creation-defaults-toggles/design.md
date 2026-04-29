## Context

LDYOM2's entity creation pipeline is a three-layer stack:

1. **Templates** — `EntitiesManager::registerCoreEntityTemplates()` (`entities_manager.cpp:120-130`) registers nine `EntityTemplate` records, each a `std::vector<std::string>` of component type names. All nine include `"objective_specific"` as the second element. The templates are static for the session.
2. **Construction** — `createEntityFromTemplate(templateType)` looks up the template, calls `createEmptyEntity()` (which seeds position from the player and `areaId` from the GTA visible-area command), then iterates the type list and calls `ComponentsManager::createComponent(type)` for each. Components are added in registration order; `ObjectiveSpecific` is therefore always the second component on every entity template.
3. **Insertion** — `addNewEntityFromTemplate(templateType)` calls (2), pushes the result into `ScenesManager::instance().getUnsafeCurrentScene().entities`, and returns the new index.

The `ObjectiveSpecific` constructor (`objective_specific.cpp:13-16`) auto-fills `spawnObjective` with `to_string(...objectives.data.back().id)` if the scene has any objectives. This happens at component construction time, *inside* `createComponent("objective_specific")`, so it fires regardless of how the component is added (entity template, manual "Add Component" UI, JSON deserialization that omits the key, Lua).

The editor's "+" button in `EntitiesWindow::renderContent` (`entities.cpp:79-130`) is the single editor entry point that converts the user's intent into an `addNewEntityFromTemplate(...)` call. Nine `case` branches map the current `m_windowType` to the corresponding template name. `ObjectivesWindow::getSelectedObjectiveIndex()` (`objectives.h:21`) is the source of truth for "which objective the user is currently editing"; it lives on the window instance, not on a manager.

The `quick_settings` window (`src/rendering/windows/quick_settings.{h,cpp}`) is a `ModalPopupWindow` that already hosts the small "user preferences" surface (language, theme, display scale, addons). It is the natural home for two more boolean preferences. The `Settings` class (`src/core/settings.{h,cpp}`) is a thin wrapper over a single `nlohmann::json` document persisted to `settings.json`, with templated `getSetting<T>(key, default)` and `setSetting<T>(key, value)` and dot-to-JSON-pointer conversion. Adding two new boolean keys is a one-line change in the defaults initializer plus two reads at the call sites.

## Goals / Non-Goals

**Goals:**

- Two independent toggles, both default `true`, controlling auto-attachment of `ObjectiveSpecific` and auto-linking of `spawnObjective` on entity creation through the editor UI.
- Zero behavior change on upgrade: a `settings.json` without the new keys behaves identically to today.
- Toggle changes take effect immediately on the next entity creation; no restart required.
- Power users can disable both toggles once and the editor stops adding components and links they will only delete.
- Lua and other non-UI callers of `EntitiesManager::addNewEntityFromTemplate` are not affected by these toggles. The toggles are an editor-level preference, not a project- or runtime-level policy.
- The second toggle visibly indicates dependence on the first (disabled in UI when the first is OFF), so the user is not confused by a no-op control.

**Non-Goals:**

- No migration of existing entities. Toggles do not retroactively add or remove components from already-placed entities.
- No effect on the `ObjectiveSpecific` constructor itself. The auto-fill of `spawnObjective` from `objectives.data.back().id` continues to fire whenever the component is constructed (manual add, JSON load with missing key, Lua). This change wraps the construction with editor policy, not the constructor.
- No exposure to Lua. `addNewEntityFromTemplateWithEditorDefaults` reads UI window state and is deliberately editor-only. Lua scripts that want different behavior already call `addNewEntityFromTemplate` (or build entities manually via `createEmptyEntity`); their surface is unchanged.
- No bundling into a `simple_mode` / `advanced_mode` enum or a "Newcomer or Power User?" first-run modal. Two granular booleans cover the needs articulated; a higher-level mode would force users to accept or reject the entire bundle and complicates future toggles that don't fit the same axis.
- No project-level override. A `settings.json` setting applies globally to whoever opens the editor on this machine. Cross-author projects do not need a "this project requires advanced mode" flag in v1.
- No new toggles beyond these two. Hiding the node editor, curating the component or objective lists, or collapsing "advanced" fields in `editorRender` are separate ideas explicitly deferred — the user has stated they are not committed to those follow-ups, so they should not be designed-around in this change.
- No telemetry. We do not record which toggle states users land on.

## Decisions

### Two booleans, not one mode

The proposal adds two independent boolean settings rather than one `editor.mode` enum. Rationale:

- The two behaviors (component-attach vs. spawn-link) are conceptually orthogonal. A user can plausibly want "yes, attach `ObjectiveSpecific` so I can pick lifecycles in the UI" while also wanting "no, do not pre-link to an arbitrary objective; I want to pick myself." A mode enum forces both choices in lockstep.
- A future toggle (e.g. "show advanced fields") may not fit cleanly into the same `simple` / `advanced` axis. Two booleans now leave room for a third boolean later without committing to a mode taxonomy. The user explicitly said the future toggles I proposed during exploration "don't seem particularly relevant" — meaning we should not architect for a hypothetical bundle that may never ship.
- A boolean is easier to surface in `quick_settings` (a checkbox row) than an enum (a combo with explanatory text per option). The `quick_settings` window today has only checkboxes, sliders, and dropdowns for trivially-bounded enums (language, theme); it does not have a precedent for "mode pickers."

### New method `addNewEntityFromTemplateWithEditorDefaults`, not modify existing

The existing `EntitiesManager::addNewEntityFromTemplate(templateType)` is a public API. It is called from `entities.cpp` (UI) today, but its signature and semantics are also implicit contracts for any Lua binding or future caller. Three options were considered:

- **A — Modify `addNewEntityFromTemplate` in place** to read settings and conditionally filter `"objective_specific"`. Simplest. Rejected: any non-UI caller (Lua scripts running in playback, future automation) would silently inherit a UI-level preference. A Lua mission that constructs entities at runtime should not have its component graph mutated by a checkbox the player toggled.
- **B — Add an `EntityCreationOptions` parameter with a default value** to the existing function. Existing call sites unchanged; new behavior opt-in via the struct. Rejected: the options struct would have to encode the "look up the selected objective" responsibility somehow, and the struct's default would need to mean "do nothing fancy" — which is a confusing semantics for "the field literally named `defaultSpawnObjectiveUuid`."
- **C — Add a new method `addNewEntityFromTemplateWithEditorDefaults(templateType, std::optional<std::string> defaultSpawnObjectiveUuid)`** that explicitly names its UI-coupling and reads `Settings` directly. The UI replaces its nine call sites with the new method. Lua callers are unaffected. This is what we ship.

The new method's name is verbose on purpose. "WithEditorDefaults" signals that this is the editor's policy-applying entry point, distinct from the pure construction path. The presence of two methods with overlapping responsibilities is a small cost paid once; it preserves a clean Lua surface and a clear policy boundary.

### Read settings inside the new method, pass selected objective from the caller

The new method takes the selected objective UUID as a parameter rather than reading `ObjectivesWindow::getSelectedObjectiveIndex()` itself. This keeps `EntitiesManager` from depending on the rendering layer (window classes, `WindowManager`). The UI is the only thing that knows which window the user has selected an objective in; making the manager lean over into the rendering layer would invert the layering the rest of the codebase observes.

The two `Settings` reads, by contrast, happen *inside* the method. Settings are a global singleton already read from many places in the codebase; they're a horizontal concern, not a layer-violating one.

### `auto_link_spawn_objective` semantics: prefer-selected-with-fallback, override-empty-on-OFF

The toggle has three observable states once interpreted:

| `auto_attach` | `auto_link` | Selection | Final `spawnObjective` |
| --- | --- | --- | --- |
| `true` | `true` | objective `X` selected | `X.id` |
| `true` | `true` | nothing selected | constructor default (last objective `id`, or `""` if scene has none) |
| `true` | `false` | (any) | `""` (free lifecycle) |
| `false` | (any) | (any) | no `ObjectiveSpecific` component → no field |

The "fallback to constructor default" branch matters. The `ObjectiveSpecific` constructor already fills `spawnObjective` with the last objective's id at the moment it is constructed; this happens *before* the editor-aware method gets a chance to override anything. We therefore:

- Leave the constructor fill alone (it produces the existing behavior for "selected nothing, scene has objectives, toggle ON").
- On `auto_link == false`, explicitly set `spawnObjective = ""` to opt out.
- On `auto_link == true && selection.has_value()`, explicitly set `spawnObjective = *selection` to override the constructor's "last" with the user's "selected".
- On `auto_link == true && !selection.has_value()`, leave the constructor fill alone.

This keeps the implementation a small post-construction mutation and avoids touching the `ObjectiveSpecific` constructor (which would ripple into manual component-add and JSON-load paths the toggles are *not* trying to control).

### Disabled-state UI for the second toggle

When `auto_attach_objective_specific == false`, the `auto_link_spawn_objective` toggle is rendered inside an `ImGui::BeginDisabled() / EndDisabled()` pair. Its persisted value is preserved (the user gets their preference back when they re-enable the first toggle), but it cannot be changed while it is moot. A short greyed-out hint string ("Requires auto-attach") is rendered next to it so users understand why the control is inert. This avoids the confusing scenario where a user disables auto-attach, then later sees the "auto-link" checkbox is still checked and wonders why entities don't get linked.

### Settings keys: `editor.entities.auto_attach_objective_specific` and `editor.entities.auto_link_spawn_objective`

The `editor.entities.*` prefix is forward-looking: future entity-creation toggles that we have not yet committed to (e.g. "auto-set position to camera target instead of player position") would land under the same prefix without polluting the top-level `editor.*` namespace. The verbose suffix names mirror the underlying mechanic (the component type name and the field name) rather than abstract UX concepts; this keeps the `settings.json` self-documenting and avoids needing a comment to explain what `editor.entities.simple` would mean.

### Where the call site reads the selected objective

In `entities.cpp`, immediately before the `switch (m_windowType)` block in the "+" button handler, a small helper computes:

```cpp
std::optional<std::string> defaultSpawnObjUuid;
if (Settings::instance().getSetting<bool>("editor.entities.auto_link_spawn_objective", true)) {
    auto* objWindowBase = WindowManager::instance().getWindow("objectives");
    if (auto* objWindow = dynamic_cast<ObjectivesWindow*>(objWindowBase)) {
        int selIdx = objWindow->getSelectedObjectiveIndex();
        auto& objectives = ScenesManager::instance().getUnsafeCurrentScene().objectives.data;
        if (selIdx >= 0 && selIdx < static_cast<int>(objectives.size())) {
            defaultSpawnObjUuid = uuids::to_string(objectives[selIdx].id);
        }
    }
}
```

The setting is checked before the lookup so we don't pay the `dynamic_cast` and scene query when the toggle is OFF. The optional stays `std::nullopt` if anything along the chain is missing (objectives window not registered, window cast fails, no selection, selection out of range), which the manager-side method treats as "fall back to constructor default."

The exact `WindowManager::getWindow` accessor signature must be verified against `src/core/window_manager.h` during implementation; the snippet above assumes a `Window*` (or `Window&`) accessor that the call site can `dynamic_cast`. If the manager exposes a typed templated accessor, the cast collapses into the accessor.

### `ObjectivesContainer` field access

`Scene::objectives` is an `ObjectivesContainer` (per `objectives_container.h:4`). Its actual element-list field name needs to be verified at implementation time — the `ObjectiveSpecific` constructor uses `objectives.data.back().id`, which suggests a `data` field. Confirming the type and access pattern is a task in tasks.md before the call-site code is written.

## Risks / Trade-offs

- **Inconsistency between editor-add and manual-add paths.** When a user adds an `ObjectiveSpecific` via the "Add Component" UI on an entity created with `auto_attach == false`, the constructor's auto-fill *will* fire — the toggles only intercept the bulk-template path. This is acceptable: the toggles are about "creation defaults", not "constructor behavior". A user who explicitly adds the component is opting into its full default behavior at that moment.
- **Settings ordering dependency.** `Settings::initialize()` is called once at startup; the new defaults must be present in the initial `m_config` before `merge_patch` runs against the on-disk config. The existing initializer already has this shape (lang, hotkeys, display_scale, theme, active_addons); we extend the same nested-object pattern.
- **`dynamic_cast<ObjectivesWindow*>` on `WindowManager::getWindow` result.** This requires `WindowManager` to return a polymorphic `Window*` pointer. If the manager's accessor returns a `unique_ptr` by reference or a custom handle type, the call site code adapts to that. Implementation tasks include verifying this.
- **Loading legacy projects with `auto_attach == false`.** A user who disables auto-attach and then loads a project that was authored under `auto_attach == true` will see all the existing `ObjectiveSpecific` components on the loaded entities. This is correct behavior — the toggle controls creation, not loading — but worth calling out in the toggle's tooltip so users don't expect a global cleanup.
- **`std::optional<std::string>` ABI in the public header.** `addNewEntityFromTemplateWithEditorDefaults` is a public method on a singleton; the `<optional>` and `<string>` includes need to be in the `entities_manager.h` header transitively (verified by build). Nothing tricky, but worth confirming as a task.

## Migration Plan

None. A `settings.json` from a previous install lacks both new keys; `getSetting<bool>(key, true)` returns the default, and `setSetting` writes the explicit value back the first time the user toggles the checkbox or otherwise triggers a save. Existing projects load with their existing components untouched. Existing Lua scripts continue to work.

## Open Questions

None blocking. The two implementation-time confirmations are:

- The exact `Scene::objectives` container element-access syntax (likely `objectives.data[i]`, but to be confirmed against `objectives_container.h`).
- Whether `WindowManager` exposes a typed window accessor or a base-`Window*` accessor; the cast strategy adapts to whichever the codebase provides.

Both are task-level lookups, not design unknowns.
