## Why

Authors using scene-switch objectives to build hub-style navigation (menus, room-to-room transitions, quick retries) are forced through the default 500ms fade-out + 600ms wait + fade-in every time they branch scenes. For many gameplay patterns this cinematic transition feels wrong and slow; authors want a per-call choice to swap scenes instantly while keeping the default fade for story beats. The transition is a property of the calling site (like the cutscene objective's own fade control), not of the destination scene, so the flag belongs on the scene-switch objective itself.

## What Changes

- Add an `instantTransition` boolean (default `false`) to the `goto_scene` and `goto_scene_if_var` objective Data structs, surfaced in the editor UI with a tooltip warning about visible artifacts (entity pop-in, camera jumpcut, weather/time snap, scene name banner).
- Extend `ProjectPlayer::requestSceneTransition` to accept and carry the instant flag so the player runtime skips the fade-out/wait block and leaves `isFaded` clear, preventing the next scene's first objective from fading in.
- Update the Lua binding for `requestSceneTransition` and the exposed objective Data fields to accept the new flag, and refresh the core Lua declaration files accordingly.
- Add localization keys for the checkbox label and tooltip.
- Existing projects and scripts deserialize with `instantTransition = false`, preserving current behavior; no breaking changes.

## Capabilities

### New Capabilities
- `scene-transition`: Covers the project player's scene-to-scene switching behavior, including the default fade-out/wait/fade-in cinematic and the opt-in instant swap mode requested by individual scene-switch objectives or Lua callers.

### Modified Capabilities

_None — `openspec/specs/` contains no existing specs to modify._

## Impact

- Code: `src/data/objectives/goto_scene.h`, `src/data/objectives/goto_scene_if_var.h`, `src/core/project_player.h`, `src/core/project_player.cpp`, `src/lua/project_player_binding.cpp`.
- Localization: `../ingame/languages/en.json` (new objective UI keys).
- Lua declarations: `../ingame/definitions/core` (Data struct fields + `requestSceneTransition` signature).
- APIs: `ProjectPlayer::requestSceneTransition` gains an optional `bool instant` parameter; the Lua-facing function signature mirrors it. Existing callers remain source-compatible via the default argument.
- Dependencies/systems: none added; relies on the JSON default-value macro already used for objective Data serialization.
