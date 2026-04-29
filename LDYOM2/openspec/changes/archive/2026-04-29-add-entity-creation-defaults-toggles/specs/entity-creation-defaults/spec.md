## ADDED Requirements

### Requirement: Persistent settings govern entity-creation defaults

The `Settings` system SHALL persist two boolean keys, `editor.entities.auto_attach_objective_specific` and `editor.entities.auto_link_spawn_objective`, that govern the editor's automatic-attach and automatic-link behavior at entity creation. Both keys SHALL default to `true`. A `settings.json` lacking either key SHALL behave as if that key were present with value `true`, and the next save cycle SHALL persist the explicit `true` so subsequent reads do not re-derive the default.

#### Scenario: Defaults are present on first run

- **WHEN** the editor starts and `settings.json` does not contain `editor.entities.auto_attach_objective_specific`
- **THEN** `Settings::getSetting<bool>("editor.entities.auto_attach_objective_specific", true)` MUST return `true`
- **AND** the next call to `Settings::save()` MUST write `editor.entities.auto_attach_objective_specific = true` to `settings.json`

- **WHEN** the editor starts and `settings.json` does not contain `editor.entities.auto_link_spawn_objective`
- **THEN** `Settings::getSetting<bool>("editor.entities.auto_link_spawn_objective", true)` MUST return `true`
- **AND** the next call to `Settings::save()` MUST write `editor.entities.auto_link_spawn_objective = true` to `settings.json`

#### Scenario: User-changed values round-trip through settings

- **WHEN** the user toggles `editor.entities.auto_attach_objective_specific` to `false` via the quick-settings UI
- **THEN** `settings.json` MUST contain `editor.entities.auto_attach_objective_specific = false` after the next save
- **AND** restarting the editor MUST restore `false` for that key
- **AND** the same property MUST hold for `editor.entities.auto_link_spawn_objective`

### Requirement: Editor-aware entity creation respects the auto-attach toggle

`EntitiesManager` SHALL expose a method `addNewEntityFromTemplateWithEditorDefaults(templateType, defaultSpawnObjectiveUuid)` that is the only entity-creation path the editor UI uses. This method SHALL read `editor.entities.auto_attach_objective_specific` and, when the value is `false`, SHALL exclude `"objective_specific"` from the entity's component set even though the registered template lists it. When the value is `true`, the method SHALL produce an entity with the same component set as the registered template (preserving today's behavior).

The pure path `addNewEntityFromTemplate(templateType)` SHALL remain unchanged in semantics — it always applies the full registered template — so non-UI callers (Lua bindings, future automation) are not affected by the editor-level toggle.

#### Scenario: Auto-attach ON yields a fully-templated entity

- **WHEN** `editor.entities.auto_attach_objective_specific` is `true`
- **AND** the editor calls `addNewEntityFromTemplateWithEditorDefaults("actor", ...)` 
- **THEN** the created entity MUST have an `ObjectiveSpecific` component (alongside the `Actor` component)
- **AND** the entity MUST be inserted into the current scene
- **AND** the returned index MUST point to the new entity

#### Scenario: Auto-attach OFF yields an entity without ObjectiveSpecific

- **WHEN** `editor.entities.auto_attach_objective_specific` is `false`
- **AND** the editor calls `addNewEntityFromTemplateWithEditorDefaults("actor", ...)`
- **THEN** the created entity MUST NOT have an `ObjectiveSpecific` component
- **AND** the entity MUST have the rest of the template's components (e.g. `Actor`)
- **AND** the entity MUST be inserted into the current scene

#### Scenario: Pure path is not affected by the toggle

- **WHEN** `editor.entities.auto_attach_objective_specific` is `false`
- **AND** Lua (or any other non-editor caller) invokes `EntitiesManager::addNewEntityFromTemplate("actor")`
- **THEN** the created entity MUST have an `ObjectiveSpecific` component
- **AND** the toggle value MUST NOT be read by `addNewEntityFromTemplate`

### Requirement: Editor-aware entity creation respects the auto-link toggle

`addNewEntityFromTemplateWithEditorDefaults` SHALL read `editor.entities.auto_link_spawn_objective` and, *when the entity has an `ObjectiveSpecific` component after construction*, SHALL apply the following policy to its `spawnObjective` field:

- If `editor.entities.auto_link_spawn_objective` is `false`: `spawnObjective` SHALL be set to the empty string `""`, overriding whatever the `ObjectiveSpecific` constructor set.
- If `editor.entities.auto_link_spawn_objective` is `true` AND the caller supplied a non-empty `defaultSpawnObjectiveUuid`: `spawnObjective` SHALL be set to that UUID, overriding the constructor default.
- If `editor.entities.auto_link_spawn_objective` is `true` AND the caller supplied no UUID (`std::nullopt`): `spawnObjective` SHALL retain whatever the `ObjectiveSpecific` constructor set (i.e. the last objective's UUID, or `""` if the scene has no objectives).

When the entity has no `ObjectiveSpecific` component (because auto-attach was OFF), the auto-link toggle SHALL be a no-op for that creation.

#### Scenario: Auto-link ON with selected objective links to selection

- **WHEN** both toggles are `true`
- **AND** the user has objective `B` selected in `ObjectivesWindow`
- **AND** the editor invokes the create-entity path passing `defaultSpawnObjectiveUuid = uuid_of(B)`
- **THEN** the new entity's `ObjectiveSpecific::spawnObjective` MUST equal `uuid_of(B)` as a string
- **AND** the value MUST NOT be the last-objective default

#### Scenario: Auto-link ON with no selection falls back to constructor default

- **WHEN** both toggles are `true`
- **AND** the editor invokes the create-entity path passing `defaultSpawnObjectiveUuid = std::nullopt`
- **AND** the scene has at least one objective
- **THEN** the new entity's `ObjectiveSpecific::spawnObjective` MUST equal the UUID of the last objective in the scene (the constructor's default)

#### Scenario: Auto-link OFF clears the spawn objective

- **WHEN** `editor.entities.auto_attach_objective_specific` is `true` AND `editor.entities.auto_link_spawn_objective` is `false`
- **AND** the editor invokes the create-entity path with any `defaultSpawnObjectiveUuid` value
- **THEN** the new entity's `ObjectiveSpecific::spawnObjective` MUST be the empty string `""`

#### Scenario: Auto-link is moot when auto-attach is OFF

- **WHEN** `editor.entities.auto_attach_objective_specific` is `false` AND `editor.entities.auto_link_spawn_objective` is `true`
- **AND** the editor invokes the create-entity path with `defaultSpawnObjectiveUuid = uuid_of(B)`
- **THEN** the new entity MUST NOT have an `ObjectiveSpecific` component
- **AND** the supplied UUID MUST be ignored (no error, no log)

### Requirement: Editor "+" button supplies the selected objective UUID

The "+" button in `EntitiesWindow::renderContent` SHALL, before invoking `addNewEntityFromTemplateWithEditorDefaults`, compute the optional `defaultSpawnObjectiveUuid` as follows: when `editor.entities.auto_link_spawn_objective` is `true`, look up the `ObjectivesWindow` from `WindowManager`; if that window exists and `getSelectedObjectiveIndex()` returns an index in range of the current scene's objective list, set the optional to the UUID of the objective at that index. Otherwise (toggle OFF, window missing, no selection, out-of-range), pass `std::nullopt`. The button SHALL then call `addNewEntityFromTemplateWithEditorDefaults(<template type>, defaultSpawnObjectiveUuid)` for the matching `m_windowType`.

#### Scenario: Selected objective is propagated to the new entity

- **WHEN** the user has objective `B` selected in `ObjectivesWindow`
- **AND** `editor.entities.auto_link_spawn_objective` is `true`
- **AND** the user clicks the "+" button in entities mode `Actor`
- **THEN** `EntitiesWindow` MUST call `addNewEntityFromTemplateWithEditorDefaults("actor", uuid_of(B))`

#### Scenario: No selection results in nullopt

- **WHEN** no objective is selected (`getSelectedObjectiveIndex()` returns `-1` or out-of-range)
- **AND** `editor.entities.auto_link_spawn_objective` is `true`
- **AND** the user clicks the "+" button in entities mode `Vehicle`
- **THEN** `EntitiesWindow` MUST call `addNewEntityFromTemplateWithEditorDefaults("vehicle", std::nullopt)`

#### Scenario: Auto-link OFF skips the lookup

- **WHEN** `editor.entities.auto_link_spawn_objective` is `false`
- **AND** the user clicks the "+" button in entities mode `Object`
- **THEN** `EntitiesWindow` MUST call `addNewEntityFromTemplateWithEditorDefaults("object", std::nullopt)`
- **AND** the lookup of `ObjectivesWindow` and the scene's objectives list MUST be skipped (no `dynamic_cast`, no `getUnsafeCurrentScene` call for this purpose)

### Requirement: Quick-settings UI surfaces both toggles with dependency feedback

The `quick_settings` window SHALL render a section titled by the localization key `quick_settings.entity_creation.section_header` containing two checkbox rows: one for `editor.entities.auto_attach_objective_specific` and one for `editor.entities.auto_link_spawn_objective`. Each row SHALL show a tooltip describing the toggle's effect when the user hovers it. The second row SHALL be wrapped in `ImGui::BeginDisabled / EndDisabled` such that it is non-interactive when the first toggle is `false`; while disabled, an inline subdued-text hint (the localization key `quick_settings.entity_creation.auto_link.disabled_hint`) SHALL be visible next to the row to explain the dependency. Toggling either checkbox SHALL persist the new value via `Settings::setSetting<bool>` immediately, and the change SHALL take effect on the next entity creation without requiring a restart.

#### Scenario: Both rows visible with default state

- **WHEN** the user opens `quick_settings` on a fresh install
- **THEN** the section header MUST be visible
- **AND** both checkboxes MUST be present and checked
- **AND** both rows MUST be interactive

#### Scenario: First toggle changes are persisted immediately

- **WHEN** the user clicks the first checkbox to toggle it from `true` to `false`
- **THEN** `Settings::setSetting<bool>("editor.entities.auto_attach_objective_specific", false)` MUST be invoked
- **AND** `settings.json` MUST contain `editor.entities.auto_attach_objective_specific = false` after the next save

#### Scenario: Second toggle becomes disabled when first is OFF

- **WHEN** the first toggle is `false`
- **THEN** the second checkbox MUST be rendered inside `ImGui::BeginDisabled(true)`
- **AND** the inline hint `quick_settings.entity_creation.auto_link.disabled_hint` MUST be visible
- **AND** clicks on the second checkbox MUST NOT mutate `editor.entities.auto_link_spawn_objective`

#### Scenario: Second toggle preserves its value across the disable cycle

- **WHEN** the first toggle is `true` AND the second toggle is `false`
- **AND** the user toggles the first to `false`, then back to `true`
- **THEN** the second checkbox MUST still display `false` after the first is re-enabled
- **AND** the persisted value of `editor.entities.auto_link_spawn_objective` MUST still be `false`

### Requirement: Existing entities are not retroactively modified

Toggling either setting SHALL affect only entities created *after* the toggle change. Entities already present in the current scene (loaded from project JSON or created earlier) SHALL retain their components and field values exactly. Saving and reloading a project across a toggle change SHALL produce byte-identical entity records (modulo unrelated changes the user makes).

#### Scenario: Toggling auto-attach OFF does not strip existing components

- **WHEN** a project loads with five entities each carrying `ObjectiveSpecific`
- **AND** the user toggles `editor.entities.auto_attach_objective_specific` to `false`
- **THEN** all five existing entities MUST still carry their `ObjectiveSpecific` components
- **AND** their `spawnObjective` fields MUST be unchanged

#### Scenario: Toggling auto-link OFF does not clear existing spawn objectives

- **WHEN** a project loads with three entities, each with `spawnObjective` set to a specific UUID
- **AND** the user toggles `editor.entities.auto_link_spawn_objective` to `false`
- **THEN** all three entities MUST still have their original `spawnObjective` UUIDs
- **AND** subsequent project saves MUST persist those original UUIDs unchanged
