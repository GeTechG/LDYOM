## Why

LDYOM2 today gives every newly-created entity (actor, vehicle, object, pickup, …) an `ObjectiveSpecific` component by virtue of `EntitiesManager::registerCoreEntityTemplates()` listing `"objective_specific"` in all nine entity templates (`src/core/entities_manager.cpp:120-130`). The component's constructor then auto-fills `spawnObjective` with the UUID of the last objective in the scene (`src/data/components/objective_specific.cpp:16`). The combined effect is DYOM-style: every entity has an objective range, the user does not need to think about lifecycle, and missions "just work" with one objective list.

This default is the right one for new authors — it's the mental model DYOM established and is what makes a mission editor feel intuitive on day one. But it actively gets in the way of authors who use the node editor, write Lua scripts, or otherwise manage entity lifecycle themselves: every new entity ships with a component they will immediately delete, and a `spawnObjective` value that points at an objective the entity has nothing to do with. That deletion is a nine-click penalty for power users on every entity placement.

The user wants two independent toggles, both default ON (preserving today's newcomer-friendly behavior), that let advanced authors disable each half of this auto-setup once and forget. The two toggles are intentionally separate: a user can keep `ObjectiveSpecific` on every entity (because they like the lifecycle gating UI) but skip the auto-link step (because they want to choose which objective owns the entity). Bundling them into a single `simple/advanced` mode would force the choice; two toggles let each author find their own balance.

## What Changes

- Add two persistent boolean settings to `Settings`, both default `true` so existing users see no behavior change on upgrade:
  - `editor.entities.auto_attach_objective_specific` — when `true`, `ObjectiveSpecific` is included in every entity template at creation time. When `false`, it is filtered out.
  - `editor.entities.auto_link_spawn_objective` — when `true` (and the entity has `ObjectiveSpecific`), `spawnObjective` is filled with the currently selected objective UUID, falling back to the constructor's existing "last objective" default when no objective is selected. When `false`, `spawnObjective` is overridden to the empty string so the entity spawns at scene start regardless of objective state.
- Add a new editor-aware creation path on `EntitiesManager`: a method `addNewEntityFromTemplateWithEditorDefaults(templateType, std::optional<std::string> defaultSpawnObjectiveUuid)` that reads both settings and applies the resulting policy. The existing `addNewEntityFromTemplate(templateType)` keeps its current pure behavior (no settings reads, full template applied) so Lua scripts and any other non-UI callers are not silently affected by a UI-level preference.
- Replace the nine `addNewEntityFromTemplate(...)` call sites in `EntitiesWindow::renderContent` (`src/rendering/windows/entities.cpp:94-129`) with calls to the new editor-aware method. Each call site computes the optional default UUID by reading `ObjectivesWindow::getSelectedObjectiveIndex()` via `WindowManager` and resolving the index to an objective UUID through the current scene.
- Surface both toggles in the existing `quick_settings` window (`src/rendering/windows/quick_settings.cpp`) under a new "Entity creation" section with explanatory tooltips. The second toggle is rendered with `BeginDisabled` when the first is `false`, since linking is moot when no `ObjectiveSpecific` component exists to receive the value.
- Localize the new section header, both labels, both tooltips, and the disabled-state hint in `../ingame/languages/en.json`.

## Capabilities

### New Capabilities

- `entity-creation-defaults`: Covers the two `Settings` toggles that govern automatic `ObjectiveSpecific` attachment and `spawnObjective` linking when an entity is created through the editor UI, and the editor-aware creation path on `EntitiesManager` that consumes them.

### Modified Capabilities

_None._ No existing capability today specifies entity-creation behavior; the templates table in `EntitiesManager` and the `ObjectiveSpecific` constructor's auto-fill are unspecced. The new capability captures both for the first time.

## Impact

- Code:
  - `src/core/settings.cpp` — two new entries in `Settings::initialize()`'s `m_config` defaults map under `editor.entities.*`.
  - `src/core/entities_manager.h` / `.cpp` — new public method `addNewEntityFromTemplateWithEditorDefaults` and the supporting plumbing to filter `"objective_specific"` from the template and to override `spawnObjective` post-construction. The existing `createEntityFromTemplate` and `addNewEntityFromTemplate` are unchanged.
  - `src/rendering/windows/entities.cpp` — the nine `case` branches that call `addNewEntityFromTemplate(...)` are replaced with calls to the new method, each preceded by a small block that resolves the selected objective UUID.
  - `src/rendering/windows/quick_settings.cpp` — a new section with two `ImGui::Checkbox` rows wrapping `Settings::getSetting<bool>` / `setSetting<bool>` calls, plus tooltips and a `BeginDisabled` guard on the second row.
- Localization: `../ingame/languages/en.json` gains six new keys (section header, two labels, two tooltips, one disabled-hint). Keys follow the existing dotted convention used by other quick-settings strings.
- Lua: no API change. The Lua-callable `EntitiesManager::addNewEntityFromTemplate` is unchanged. The new editor-aware method is intentionally not exposed to Lua because it reads UI window state.
- Project file format: no schema changes. Existing projects load with the same components they were saved with; toggles only affect *future* creation. Switching toggles never mutates already-placed entities.
- `settings.json`: a `settings.json` without the new keys is upgraded transparently — `getSetting<bool>(key, true)` returns the default and `setSetting` writes the explicit value back the first time the user opens or interacts with the quick-settings panel.
- Backwards compatibility: a fresh install gets both toggles `true`, which is byte-for-byte the current behavior. No migration. No silent behavior changes on upgrade.
