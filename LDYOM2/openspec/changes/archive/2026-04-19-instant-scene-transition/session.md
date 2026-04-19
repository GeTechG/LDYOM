---
change: instant-scene-transition
updated: 2026-04-19T12:00:00Z
status: completed
---

## In-Flight
none

## Decisions
- [task 2.2] `goto_scene::Data` NLOHMANN macro upgraded from plain to `_WITH_DEFAULT` — also tolerates missing `sceneId` in JSON (benign).
- [task 5.1] `goto_scene.h` checkbox uses bare `ImGui::Checkbox` (matches its simple layout).
- [task 5.2] `goto_scene_if_var.h` checkbox uses labeled-row pattern (Text + SameLine + Checkbox with `##instant` id).
- [task 6.2] Created NEW Lua declaration file `../ingame/LDYOM/addons/definitions/core/objectives.lua` (first objective-Data declaration file). `condition` field intentionally omitted from `ObjectiveGotoSceneIfVarData` until the `Condition` enum is bound to Lua.
- [task 8.x] User confirmed ("да все ок") — all manual in-game verification scenarios passed.

## Gotchas
- Real localization path: `../ingame/LDYOM/languages/en.json`.
- Real Lua definitions path: `../ingame/LDYOM/addons/definitions/core/`.

## Open Questions

## Next Steps
All 25 tasks complete. Ready to archive with `/opsx:archive instant-scene-transition`.
