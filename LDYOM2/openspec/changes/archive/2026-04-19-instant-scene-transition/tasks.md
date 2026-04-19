## 1. Verify prerequisites

- [x] 1.1 Confirm `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT` is available in the vendored nlohmann version (grep usage in repo; design.md notes `goto_scene_if_var.h:40` already uses it — if present, mark verified).
- [x] 1.2 Inspect existing localization keys under `objectives.core.goto_scene` and `objectives.core.goto_scene_if_var` in `../ingame/languages/en.json` to decide whether new checkbox keys should be shared or duplicated per objective.
- [x] 1.3 Inspect `../ingame/definitions/core` declarations for existing optional-parameter conventions (`@default` annotations vs `boolean?`) to match when updating the `requestSceneTransition` signature.

## 2. Data model

- [x] 2.1 Add `bool instantTransition = false` to `goto_scene::Data` in `src/data/objectives/goto_scene.h`.
- [x] 2.2 Upgrade `goto_scene::Data`'s NLOHMANN macro to `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT` and include `instantTransition` in the field list.
- [x] 2.3 Update `SOL_LUA_FOR_EACH` for `goto_scene::Data` to expose `instantTransition` to Lua.
- [x] 2.4 Add `bool instantTransition = false` to `goto_scene_if_var::Data` in `src/data/objectives/goto_scene_if_var.h`, extend its existing `WITH_DEFAULT` macro, and expose it via `SOL_LUA_FOR_EACH`.

## 3. ProjectPlayer state and API

- [x] 3.1 Introduce `struct PendingTransition { std::string sceneId; bool instant; };` in `src/core/project_player.h` and replace `std::optional<std::string> pendingSceneTransition` with `std::optional<PendingTransition>` in `ProjectPlayerState`.
- [x] 3.2 Extend `ProjectPlayer::requestSceneTransition` signature to `requestSceneTransition(std::string_view sceneId, bool instant = false)` in `project_player.h` and `project_player.cpp`, storing the flag in the pending struct.
- [x] 3.3 Update every internal read of `pendingSceneTransition` in `project_player.cpp` (the `break` site around line 138 and the consume block starting around line 160) to access `pending->sceneId` and reset the full optional.
- [x] 3.4 In the consume block at `project_player.cpp:160-177`, gate the `DO_FADE` + wait on `!pending->instant` so instant transitions skip the fade-out; keep `isFaded = false` on the instant path so the next scene's first-objective prelude (guard at line 128) does not fade back in.

## 4. Call sites

- [x] 4.1 Update `goto_scene::execute` to forward `data.instantTransition` into `requestSceneTransition`.
- [x] 4.2 Update `goto_scene_if_var::execute` to forward `data.instantTransition` into `requestSceneTransition` on the branch that triggers the transition.

## 5. Editor UI

- [x] 5.1 Add an `ImGui::Checkbox` for "Instant transition" in `goto_scene::renderEditor`, bound to `data.instantTransition`, with a tooltip listing the visible artifacts (entity pop-in, camera jumpcut, weather/time snap, scene name banner).
- [x] 5.2 Add the same checkbox + tooltip in `goto_scene_if_var::renderEditor`, placing it in a layout consistent with the panel's existing rows.

## 6. Lua binding

- [x] 6.1 Extend the `requestSceneTransition` binding in `src/lua/project_player_binding.cpp` (around line 102) to accept an optional second `sol::optional<bool> instant` argument, defaulting to `false` when missing/nil.
- [x] 6.2 Update the core Lua declaration files in `../ingame/definitions/core` for the `goto_scene` / `goto_scene_if_var` `Data` structs (new `instantTransition` field) and the `requestSceneTransition` signature (optional `instant` boolean parameter), matching the annotation convention discovered in task 1.3.

## 7. Localization

- [x] 7.1 Add `objectives.core.goto_scene.instant_transition` and `objectives.core.goto_scene.instant_transition_tooltip` keys to `../ingame/languages/en.json`.
- [x] 7.2 Add matching keys under `objectives.core.goto_scene_if_var` (or reuse the `goto_scene` keys if the existing keyspace convention from task 1.2 prefers sharing), ensuring both editor panels render localized strings rather than literals.

## 8. Verification

- [x] 8.1 Build the project in Debug configuration (per CLAUDE.md: always build Debug).
- [x] 8.2 Manually verify: load a legacy project JSON without any `instantTransition` field — loading succeeds and the field defaults to `false` on every `goto_scene` / `goto_scene_if_var` objective.
- [x] 8.3 Manually verify: enable "Instant transition" on a `goto_scene` objective, run the project — the scene swaps without fade-out or fade-in.
- [x] 8.4 Manually verify: default behavior is unchanged — with `instantTransition = false`, the 500ms fade-out + 600ms wait + fade-in still plays.
- [x] 8.5 Manually verify: `projectPlayer:requestSceneTransition("sceneId", true)` from Lua skips the fade, and `projectPlayer:requestSceneTransition("sceneId")` still triggers the default fade.
- [x] 8.6 Manually verify: instant transition fired while the screen is already faded to black (e.g., mid-cutscene) does not add an extra fade and does not force a fade-in on the next objective.
