---
change: add-entity-creation-defaults-toggles
updated: 2026-04-29T01:30:00Z
status: paused
---

## In-Flight
none — all code/spec/localization tasks complete; only interactive runtime verifications remain.

## Decisions

- [task 1.1] `Scene.objectives` is `ObjectivesContainer { std::vector<Objective> data; }`. Index access: `scene.objectives.data[i].id`. Convert: `uuids::to_string(...)`.
- [task 1.2] `WindowManager` exposes `template <typename T> std::optional<T*> getWindowAs(std::string_view id)` — used at the call site instead of manual `dynamic_cast`.
- [task 1.4] `quick_settings.cpp` uses flat-list layout. New entity-creation section is introduced via `ImGui::SeparatorText`.
- [task 3.x] `ObjectiveSpecific` lives in `namespace components`. Component model uses `shared_ptr` (not raw); `cast` returns `std::shared_ptr<ObjectiveSpecific>`.
- [task 7.8] `EntitiesManager` has no Lua bindings (verified — `src/lua/` has zero references and `src/data/components/objective_specific.h:49` registers components, but no entities_manager binding exists). Lua isolation is trivially satisfied: the unchanged `addNewEntityFromTemplate` is unexposed to Lua, and the new editor method is also not exposed. Future Lua bindings (if added) will need to choose explicitly.

## Gotchas

- Localization file actual path is `C:\Users\User\Documents\LDYOM\ingame\LDYOM\languages\en.json` (double `LDYOM` nesting), NOT `../ingame/languages/en.json` as stated in CLAUDE.md.
- LSP diagnostics about missing `entity.h`, `logger.h`, basic STL types are intellisense-side noise — clang LSP doesn't have proper include paths. Real Debug build is the source of truth.
- `Settings::initialize()` uses brace-init nlohmann::json: `{"editor", {{"entities", {{"key", true}, ...}}}}` — verified working with the in-place build.
- Top-level keys in `m_config` are NOT alphabetical despite the spec hint (existing order is `lang, hotkeys, display_scale, theme, active_addons` — not alphabetical). New `editor` was inserted after `active_addons` per spec; final on-disk order matches insertion order.
- The orchestrator can perform: code edits, openspec validation, Debug build (PASSED — `ldyom.asi` compiled). The orchestrator CANNOT perform: launching the game, clicking UI, observing in-game spawn behavior. Tasks 2.2 and 7.2-7.7, 7.9, 7.10 require the user to launch the game and validate.
- `Settings::setSetting<T>` (settings.h:56-63) auto-calls `save()` whenever the value differs from the prior. Persistence is automatic — task 7.10 round-trip will pass naturally.

## Open Questions

## Next Steps (interactive verification — user must perform manually)

1. **2.2 / 7.2** First-run: delete `settings.json`, launch the editor, exit cleanly. Confirm `editor.entities.auto_attach_objective_specific=true` and `editor.entities.auto_link_spawn_objective=true` appear in the regenerated `settings.json`. Confirm both checkboxes are ON in the quick-settings panel.
2. **7.3** Newcomer-default flow: project with ≥2 objectives, select objective #2, click "+" in entities — new actor's `ObjectiveSpecific.spawnObjective` MUST be objective #2's UUID.
3. **7.4** No-selection fallback: deselect objective, click "+" — `spawnObjective` MUST be the last objective (constructor default).
4. **7.5** Auto-link OFF: turn that toggle off, click "+" — entity has `ObjectiveSpecific` but `spawnObjective` is empty.
5. **7.6** Auto-attach OFF: turn off, the auto-link checkbox greys out with disabled-hint visible. Click "+" — entity has NO `ObjectiveSpecific`. Playback: entity spawns at scene start, no objective-driven despawn.
6. **7.7** Re-enable: turn auto-attach back ON — auto-link checkbox restores its previous value.
7. **7.9** Existing-project verification: open a v1 project, confirm existing entities load with original components. Save+reload — byte-identical project JSON.
8. **7.10** `settings.json` round-trip: toggle off→close→reopen→on→close→reopen. Verify both keys present with correct values at each step.
