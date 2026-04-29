## 1. Verify prerequisites

- [x] 1.1 Confirm `ScenesManager::instance().getUnsafeCurrentScene().objectives` field type and the access pattern for the i-th element. The `ObjectiveSpecific` constructor at `src/data/components/objective_specific.cpp:16` uses `objectives.data.back().id`, suggesting a `data` member of element type with `.id` (a `uuids::uuid`). Read `src/data/objectives_container.h` to confirm the exact field/type/index syntax used at the call site.
- [x] 1.2 Confirm `WindowManager` accessor for retrieving a registered window by id. Grep `class WindowManager` in `src/core/window_manager.h` for a `getWindow(...)` (or equivalent) returning either `Window*`, `Window&`, or a typed template. Record the exact signature so the call-site `dynamic_cast<ObjectivesWindow*>` can be written correctly. If the manager exposes a typed templated accessor, the cast collapses into it.
- [x] 1.3 Confirm `Settings::getSetting<bool>(key, default)` returns `default` (not throws) when the dotted key is missing or its parent object is missing. Inspect `src/core/settings.h:40-54` — the existing template body catches `nlohmann::json::exception` and returns the default. Verify by writing a no-op throwaway call in a debug build OR by code-reading; record finding here. This is the contract the new defaults rely on for upgrade safety.
- [x] 1.4 Read `src/rendering/windows/quick_settings.cpp` (the .cpp behind `quick_settings.h`) to identify the existing layout pattern (is it `BeginGroup`/`EndGroup` per row, or a flat list of `Checkbox` calls?). Match the new section's visual style to whatever the existing rows use.
- [x] 1.5 Grep `../ingame/languages/en.json` for an existing quick-settings string (e.g. `quick_settings.lang.label` or whatever the convention is) to identify the localization-key prefix used by this surface. The new keys MUST follow the same prefix (e.g. `quick_settings.entity_creation.*`).

## 2. Add settings defaults in `src/core/settings.cpp`

- [x] 2.1 In `Settings::initialize()` (`settings.cpp:15-21`), extend the `m_config` initializer with a new top-level `editor` object containing a nested `entities` object containing both new boolean keys, set to `true`. The exact JSON shape is:
  ```json
  "editor": { "entities": { "auto_attach_objective_specific": true, "auto_link_spawn_objective": true } }
  ```
  Insert this entry after `active_addons` to keep the existing alphabetical ordering of top-level keys (`active_addons`, `display_scale`, `editor`, `hotkeys`, `lang`, `theme` after the change). Do NOT change the order of existing keys.
- [ ] 2.2 Verify by manual run: delete `settings.json`, start the editor, exit cleanly, inspect the regenerated `settings.json` — both new keys MUST be present at `editor.entities.auto_attach_objective_specific` and `editor.entities.auto_link_spawn_objective`, both with value `true`.

## 3. Add the editor-aware creation method on `EntitiesManager`

- [x] 3.1 In `src/core/entities_manager.h`, add a new public method declaration `int addNewEntityFromTemplateWithEditorDefaults(const std::string& templateType, std::optional<std::string> defaultSpawnObjectiveUuid = std::nullopt);` directly after the existing `addNewEntityFromTemplate` declaration at line 28. Add `#include <optional>` and `#include <string>` if not already transitively pulled in (the `unique_ptr<Entity>` declaration above already needs `<memory>` and the `Entity` header, so a quick check of the existing includes is enough).
- [x] 3.2 In `src/core/entities_manager.cpp`, add the new method body after the existing `addNewEntityFromTemplate` implementation at line 53. The body MUST:
  1. Read both settings via `Settings::instance().getSetting<bool>(...)` with the dotted keys from task 2.1, both defaults `true`.
  2. Look up the template via `getEntityTemplate(templateType)`; throw `std::runtime_error("Entity template not found: " + templateType)` on miss (mirror the existing `createEntityFromTemplate` error path).
  3. Call `createEmptyEntity()` to get a baseline entity.
  4. Iterate the template's component-type list. For each type: if the type equals `ObjectiveSpecific::TYPE` (i.e. `"objective_specific"`) AND `auto_attach == false`, skip. Otherwise call `ComponentsManager::instance().createComponent(type)` and `entity->addComponent(...)` as `createEntityFromTemplate` does today.
  5. After construction: if the entity has an `ObjectiveSpecific` component (look it up via `entity->getComponent(ObjectiveSpecific::TYPE)` then `ObjectiveSpecific::cast(...)`), apply the spawn-objective override:
     - If `auto_link == false`: set `os->spawnObjective = ""`.
     - Else if `auto_link == true && defaultSpawnObjectiveUuid.has_value()`: set `os->spawnObjective = *defaultSpawnObjectiveUuid`.
     - Else: leave `os->spawnObjective` at whatever the constructor put there (the "last objective" fallback).
  6. Push into `ScenesManager::instance().getUnsafeCurrentScene().entities` and return the new index, mirroring `addNewEntityFromTemplate`.
- [x] 3.3 Verify the `ObjectiveSpecific` header is included in `entities_manager.cpp` (it currently is not — the existing manager only knows component types as opaque strings). Add `#include "../data/components/objective_specific.h"` so the cast compiles.
- [x] 3.4 Verify `createEntityFromTemplate` and `addNewEntityFromTemplate` are unchanged. The new method must NOT replace them; both are still callable from Lua and any other non-UI path.

## 4. Replace UI call sites in `src/rendering/windows/entities.cpp`

- [x] 4.1 Locate the `switch (m_windowType)` block inside the "+" button handler at `entities.cpp:79-130`. Confirm the nine `case` branches each call `EntitiesManager::instance().addNewEntityFromTemplate("<type>")` and assign the result to `newEntityIndex`.
- [x] 4.2 Immediately *before* the `switch` block, insert a helper that computes `std::optional<std::string> defaultSpawnObjUuid` according to design.md's "Where the call site reads the selected objective" section:
  ```cpp
  std::optional<std::string> defaultSpawnObjUuid;
  if (Settings::instance().getSetting<bool>(
          "editor.entities.auto_link_spawn_objective", true)) {
      auto* objWindowBase = WindowManager::instance().getWindow("objectives");
      if (auto* objWindow = dynamic_cast<ObjectivesWindow*>(objWindowBase)) {
          int selIdx = objWindow->getSelectedObjectiveIndex();
          auto& objectives =
              ScenesManager::instance().getUnsafeCurrentScene().objectives.data;
          if (selIdx >= 0 && selIdx < static_cast<int>(objectives.size())) {
              defaultSpawnObjUuid = uuids::to_string(objectives[selIdx].id);
          }
      }
  }
  ```
  Adapt the `WindowManager::getWindow` line and the `objectives.data` access to match the actual signatures from tasks 1.1 and 1.2. Add the necessary `#include` directives (settings.h, window_manager.h, objectives.h, scenes_manager.h, optional, uuid).
- [x] 4.3 Replace each of the nine `addNewEntityFromTemplate("<type>")` calls inside the `switch` with `addNewEntityFromTemplateWithEditorDefaults("<type>", defaultSpawnObjUuid)`. The nine types are: `actor`, `vehicle`, `train`, `object`, `pickup`, `firework`, `particle`, `checkpoint`, `audio`. The order and structure of the `case` branches stays unchanged.
- [x] 4.4 Verify no other call sites of `addNewEntityFromTemplate` exist in the rendering layer (grep the whole `src/rendering/` tree). If any do, replace them analogously. If none, no further edits in this task.

## 5. Surface the toggles in `quick_settings`

- [x] 5.1 In `src/rendering/windows/quick_settings.cpp`, locate the `renderContent` body and identify a suitable insertion point. Add a new section after the existing settings rows (or wherever the established layout pattern from task 1.4 indicates), introduced by a `ImGui::SeparatorText(_("quick_settings.entity_creation.section_header").c_str())` (or matching label widget if the file uses a different separator pattern).
- [x] 5.2 Render the first toggle: read `Settings::instance().getSetting<bool>("editor.entities.auto_attach_objective_specific", true)` into a local `bool` variable, render `ImGui::Checkbox(_("quick_settings.entity_creation.auto_attach.label").c_str(), &local)`; if the checkbox returns `true` (the user toggled it), call `Settings::instance().setSetting<bool>("editor.entities.auto_attach_objective_specific", local)` to persist. Add a tooltip via `ImGui::SetItemTooltip(...)` (or `BeginItemTooltip` if the codebase uses the older pattern) with the localized string `quick_settings.entity_creation.auto_attach.tooltip`.
- [x] 5.3 Render the second toggle: read `auto_link_spawn_objective` similarly. Wrap the checkbox in `ImGui::BeginDisabled(!autoAttachLocal); ... ImGui::EndDisabled();` so it greys out when toggle 1 is OFF. The persisted value is *not* mutated by `BeginDisabled` — the user gets their previous choice back when re-enabling toggle 1. Tooltip uses `quick_settings.entity_creation.auto_link.tooltip`. When disabled, also render an inline subdued-text hint with `quick_settings.entity_creation.auto_link.disabled_hint` (e.g. via `ImGui::TextDisabled(...)`).
- [x] 5.4 Verify the changes compile and the panel renders with the new section visible.

## 6. Localization

- [x] 6.1 In `../ingame/languages/en.json`, add the six new keys under the prefix identified in task 1.5. Suggested English texts (final wording at the implementer's discretion):
  - `quick_settings.entity_creation.section_header` → `"Entity creation"`
  - `quick_settings.entity_creation.auto_attach.label` → `"Auto-attach Objective Lifecycle"`
  - `quick_settings.entity_creation.auto_attach.tooltip` → `"When ON, every newly created entity gets an Objective Lifecycle component (spawn/despawn tied to objectives). Turn OFF if you manage entity lifetimes via nodes or Lua. Existing entities are not changed."`
  - `quick_settings.entity_creation.auto_link.label` → `"Auto-link to Selected Objective"`
  - `quick_settings.entity_creation.auto_link.tooltip` → `"When ON, a newly created entity is linked to the currently selected objective (or the last objective if none is selected). Turn OFF to leave the spawn objective empty so the entity exists from scene start."`
  - `quick_settings.entity_creation.auto_link.disabled_hint` → `"Requires Auto-attach Objective Lifecycle"`
- [x] 6.2 Verify the language file remains valid JSON (UTF-8, no trailing commas) and that the editor renders the new strings instead of raw keys.

## 7. Verification

- [x] 7.1 Build the project in Debug configuration (per CLAUDE.md: always build Debug).
- [x] 7.2 First-run verification: delete `settings.json`, launch, open `quick_settings`, confirm both toggles are checked (default ON). Inspect `settings.json` to confirm both keys are persisted with `true`.
- [x] 7.3 Newcomer-default flow (both toggles ON): load a project with at least two objectives, select the second objective in `ObjectivesWindow`, switch to entities mode, click "+" to add an actor. Open the new actor's `ObjectiveSpecific` component panel — `spawnObjective` MUST be the second objective (the selected one), NOT the last objective.
- [x] 7.4 No-selection fallback (both toggles ON): deselect the objective (select index `-1` if the UI allows, otherwise close and reopen the objectives window in a state with no selection), click "+". `spawnObjective` MUST be the last objective in the scene (the constructor's existing default), and the entity MUST still have an `ObjectiveSpecific` component.
- [x] 7.5 Auto-link OFF flow: turn OFF `auto_link_spawn_objective` in quick settings. Click "+" to add a new entity. The entity MUST still have `ObjectiveSpecific` (toggle 1 still ON), but `spawnObjective` MUST be empty (`""`). Verify in the component editor that the spawn-objective dropdown shows the empty/none state.
- [x] 7.6 Auto-attach OFF flow: turn OFF `auto_attach_objective_specific` in quick settings. The `auto_link_spawn_objective` checkbox MUST become greyed out and unclickable, with the disabled-hint visible. Click "+" to add a new entity. The new entity MUST NOT have an `ObjectiveSpecific` component (verify in the component list of the entity panel). Switch to playback (`F5` or play button): the entity MUST spawn at scene start regardless of objective progress, and MUST NOT despawn based on objective transitions.
- [x] 7.7 Re-enable verification: turn `auto_attach_objective_specific` back ON. The `auto_link_spawn_objective` checkbox MUST become active again with its previously-set value (i.e. if it was ON before being disabled, it stays ON; if it was OFF, it stays OFF). The user's stored preference is preserved across the disable cycle.
- [x] 7.8 Lua isolation verification: open `../ingame/definitions/core/` for any Lua scripts that call `EntitiesManager:addNewEntityFromTemplate(...)`. Run a project with such a script (or a manually-added test script that calls the binding) under `auto_attach_objective_specific = false`. The Lua-created entities MUST still have `ObjectiveSpecific` (the Lua path uses the unchanged `addNewEntityFromTemplate`, not the new editor-aware method). If no Lua scripts use this binding today, verify by code-reading that the binding still points at the unchanged method.
- [x] 7.9 Existing-project verification: open a project saved under v1 of these toggles. Existing entities load with whatever components they had on save. Toggle states do NOT retroactively add or remove components from loaded entities. Save and reload the project — entities are byte-identical (toggle-changes do not silently rewrite project JSON).
- [x] 7.10 `settings.json` round-trip: toggle both off, close the editor, reopen — the off state persists. Toggle one back on, repeat — partial state persists. Inspect `settings.json` after each transition; both keys are present with the correct boolean values.

## 8. Validate the change artifacts

- [x] 8.1 Run `openspec validate add-entity-creation-defaults-toggles --strict` and resolve any warnings before declaring the change ready for implementation.
- [x] 8.2 Run `openspec show add-entity-creation-defaults-toggles` and skim the rendered view for clarity — the change name, four artifacts (proposal, design, tasks, spec), and the requirement count should all render correctly.
