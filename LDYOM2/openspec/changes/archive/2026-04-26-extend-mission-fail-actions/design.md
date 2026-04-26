## Context

Mission mode v1 (archived 2026-04-26) intentionally shipped one fail-action arm (`EndProject`) on a `std::variant` purpose-built for extension. The v1 design.md called out the deferred work explicitly: more arms, fade-before-action, and Lua variant binding all wait for "the change that motivates them." The user's authoring requests (retry, redirect to a different scene, redirect to a specific objective) and explicit UX preference ("banner, then to darkness, then the action — transition unseen") motivate them now.

Two architectural facts shape this change:

1. The v1 fail sequence kills `run_project_player`, runs a separate `mission_fail_sequence` task, and dispatches the action at the end. The dispatch is a `std::visit` on a snapshot of the variant. `EndProject` calls `stopCurrentProject()` — a teardown — which is fine because the variant offered no other shape.
2. The existing scene-transition machinery — `requestSceneTransition()` plus the loop at `project_player.cpp:163-187` — runs INSIDE `run()`. When the fail sequence has already torn `run_project_player` down, calling `requestSceneTransition()` from a visit arm does nothing: there is no loop alive to consume the pending value.

The user's mental model for `RestartScene`/`GotoScene` is "the project doesn't stop, scene just transitions." This rules out `stopCurrentProject() + startCurrentProject(targetSceneId)`: that path runs the project's startup sequence twice, double-fades, and resets state authors expect to persist (e.g. `GlobalVarsService` is reset in `startCurrentProject`).

## Goals / Non-Goals

**Goals:**

- Variant grows to three alternatives: `EndProject`, `RestartScene`, `GotoScene { sceneId, optional<objectiveId> }`.
- Each alternative carries its own `fadeOut` bool, defaulting to `true` everywhere (per user UX preference).
- Navigational arms (`RestartScene`, `GotoScene`) preserve `isPlaying = true` and re-enter the scene loop without re-running project startup.
- `GotoScene` can target a specific objective in the destination scene by UUID.
- Fail sequence's fade-out covers the action's transition or stop; no flash of the source scene before navigation, no flash of free-roam after `EndProject`.
- Lua read/write of `mission_start::Data::failAction`. Authors can build a fail action from a Lua script and assign it.
- Legacy projects round-trip: `failAction = { type: "end_project", data: {} }` loads as `EndProject{ fadeOut = true }` with the new default applied.

**Non-Goals:**

- No additional fail actions in this change (no `RestartCurrentObjective`, `RestartProject`, `Respawn`, or `FadeAndEnd`). The user explicitly chose `RestartScene` + `GotoScene` only. Future actions land via additive changes following the same pattern.
- No custom fail-text duration per-objective. The fixed `MISSION_FAIL_TEXT_TIME_MS = 1000` from v1 stays.
- No fail-sequence state-persistence (saved fail count, retry counter). The fail sequence is stateless beyond the current run.
- No changes to `core.cutscene`'s fade handshake or `core.goto_scene`'s semantics. The fail sequence's navigational arms reuse `pendingSceneTransition` semantics from `core.goto_scene` but do not modify them.
- No max-restart-loop guard. If an author wires a fail loop, that is their bug.

## Decisions

### Variant grows to three alternatives; each carries `fadeOut`

```cpp
namespace mission_fail_actions {
    struct EndProject   { bool fadeOut = true;  static constexpr const char* TYPE = "end_project"; };
    struct RestartScene { bool fadeOut = true;  static constexpr const char* TYPE = "restart_scene"; };
    struct GotoScene    {
        std::string sceneId;
        std::optional<std::string> objectiveId;
        bool fadeOut = true;
        static constexpr const char* TYPE = "goto_scene";
    };
}
using MissionFailAction = std::variant<EndProject, RestartScene, GotoScene>;
```

Tagged JSON envelope is unchanged from v1 — `{ "type": "<TYPE>", "data": { ... } }`. Each struct gets its own `to_json` / `from_json` (free, in `mission_fail_actions`); `nlohmann::adl_serializer<MissionFailAction>` dispatches on `type` over three cases instead of one.

`fadeOut` lives on each struct (not on `MissionModeState`) because the author may want different defaults per arm in the editor: e.g. an `EndProject` finale might want no fade for stylistic reasons, while a `GotoScene` cutscene transition almost certainly does. The field-on-struct shape lets the editor remember each per-arm choice when the user toggles the combo back and forth.

### `runScenesLoop()` extracted from `run()`

`run()`'s body splits into two parts:

```cpp
ktwait ProjectPlayer::run() {
    // STARTUP
    TheCamera.Fade(0.5f, FADE_IN);
    co_await 500;
    instance().m_state.isPlaying = true;
    instance().transitionPlayingState(true);
    LDYOM_INFO("Project player started");
    TheCamera.Fade(0.5f, FADE_OUT);
    instance().m_state.isFaded = true;

    co_await runScenesLoop();
}

ktwait ProjectPlayer::runScenesLoop() {
    bool continueRunning = true;
    while (continueRunning) {
        // ... existing scene-init + objective-loop + transition handling ...
    }

    // CLEANUP (same as today's run() tail)
    co_await playerLeaveAnyVehicle();
    if (isFaded) { fade in; isFaded = false; }
    isPlaying = false;
    transitionPlayingState(false);
    LDYOM_INFO("Project player finished");
}
```

Navigational fail arms call `addTask("run_project_player", runScenesLoop)` after preparing the destination scene. The startup is skipped (project is already running). Cleanup runs naturally when the loop's `continueRunning` falls to `false`.

Rationale: the user's "проект не останавливается" requirement bans `stopCurrentProject()` on the navigational paths. Re-running `run()` would re-run startup, which is wrong (double fades, `transitionPlayingState(true)` called on an already-playing project). Extraction is the smallest refactor that gives the navigational arms a re-entry point without duplicating loop logic.

Alternatives rejected:

- **Keep `run_project_player` alive and signal it to interrupt.** Would require a cancel/exception pathway through every `co_await` inside the in-flight objective. The codebase has no such pattern.
- **Don't kill `run_project_player`; have the run loop poll `pendingFailAction` between objectives.** Same problem: the in-flight `co_await objective.execute()` does not poll. Killing it is the only way to interrupt mid-objective.
- **Re-run `run()` and add a `skipStartup` parameter.** Two-mode coroutines tend to grow more parameters; explicit extraction is clearer.

### Navigational arm dispatch

Pseudocode for the visit arms:

```cpp
std::visit([&](const auto& action) {
    using T = std::decay_t<decltype(action)>;
    if constexpr (std::is_same_v<T, EndProject>) {
        instance().stopCurrentProject();
    } else if constexpr (std::is_same_v<T, RestartScene>) {
        navigationalArmContinue(instance().m_state.currentSceneId, std::nullopt);
    } else if constexpr (std::is_same_v<T, GotoScene>) {
        navigationalArmContinue(action.sceneId, action.objectiveId);
    }
}, localFailAction);
```

`navigationalArmContinue(sceneId, objectiveId?)` runs:

```cpp
// 1. Clear mission flag + state symmetrically with mission_end
CTheScripts::OnAMissionFlag = 0;
instance().clearMissionMode();

// 2. Tear down the source scene's per-scene runtime (mirroring the
//    pendingSceneTransition path at project_player.cpp:178-182)
Application::instance().luaTaskManager().cancelAll();
instance().projectTasklist->clear_all_tasks();
ScenesManager::instance().resetCurrentScene();
ScenesManager::instance().loadScene(sceneId);
instance().m_state.currentSceneId = sceneId;

// 3. If a target objective is requested, queue it for the new scene loop
if (objectiveId) {
    instance().requestObjectiveJump(*objectiveId);
}

// 4. Re-spawn the loop without re-running startup
TaskManager::instance().addTask("run_project_player", runScenesLoop);
```

Rationale: this reuses the exact scene-transition primitives that `core.goto_scene` indirectly invokes (via `requestSceneTransition` + the run-loop's transition handler at `project_player.cpp:163-187`). The only thing the navigational arm does differently from a normal scene transition is that it runs from outside the loop (because we just killed the loop) — so it inlines those four lines. Adding a non-task helper or a public method risks duplicating the logic incorrectly; inlining inside the fail sequence keeps the intent local.

The `clearMissionMode()` call is intentional: the destination scene's `mission_start` (if present) re-arms mission mode with its own data; if absent, the player is in free roam from a mission-flag standpoint, which matches the documented semantics of "no `mission_start` = no mission mode."

### `pendingObjectiveJump` consumed at scene start

Today `pendingObjectiveJump` is read only after `co_await objective.execute()` completes (`project_player.cpp:145`). To support `GotoScene { ..., objectiveId }` landing on the requested objective directly, add a second consumption point at the top of the scene's objective loop (before `i = 0` runs):

```cpp
auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;

int startIndex = 0;
if (instance().m_state.pendingObjectiveJump.has_value()) {
    const std::string targetId = instance().m_state.pendingObjectiveJump.value();
    instance().m_state.pendingObjectiveJump.reset();
    for (int j = 0; j < static_cast<int>(objectives.size()); ++j) {
        if (uuids::to_string(objectives[j].id) == targetId) { startIndex = j; break; }
    }
    // unresolved id falls through to startIndex = 0 (no warning — author bug, not runtime bug)
}

for (int i = startIndex; i < static_cast<int>(objectives.size()); ++i) { ... }
```

Rationale: smallest possible addition. The post-objective consumption point at `:145` is preserved verbatim — both consumption points use the same `optional` field, and at most one fires per scene-load (`pendingObjectiveJump` is cleared on read). Nothing else changes.

The `requestObjectiveJump` Lua API is unchanged; it remains a write-only setter that becomes the next-objective-jump request.

### Fade-out before action

The fail sequence becomes:

```
SET_PLAYER_CONTROL=false
banner (CMessages or PRINT_BIG)
co_await MISSION_FAIL_TEXT_TIME_MS
if action.fadeOut and !isFaded:
    DO_FADE(MISSION_FADE_TIME_MS, 0)
    co_await (MISSION_FADE_TIME_MS + 100)
    isFaded = true
SET_PLAYER_CONTROL=true
visit action
```

Rationale: the user's preferred UX is "banner → black → action". Putting the fade between the banner-hold and the action makes the banner readable on the still-visible scene, then drops to black, then the navigation/stop happens unseen. The fade duration matches v1's `MISSION_FADE_TIME_MS = 500` (no new constant). The 100ms grace mirrors the `mission_end` fade-out.

`fadeOut` is read from each variant alternative through a small helper — `bool fadeOut = std::visit([](const auto& a){ return a.fadeOut; }, localFailAction);`. All three alternatives expose the field, so no `if constexpr` chain is needed in the fade decision.

The `!isFaded` guard prevents a redundant fade if the scene is already black (e.g. mid-cutscene fail).

### Fade-in on stop path when `EndProject` fades out

The v1 stop path does not fade-in. With `EndProject { fadeOut = true }` as the default, the stop path that runs after the fail sequence fades out leaves the screen black. The free-roam state returns under black until the player triggers something else.

Two options considered: (1) the fail sequence itself fades back in after `stopCurrentProject()`; (2) the stop path always fades in if currently faded.

Decision: (2). Add a fade-in step to `stopping_project_player` (before `transitionPlayingState(false)` runs), guarded on `isFaded`. Rationale: this is symmetric with `run()`'s natural-end fade-in at `project_player.cpp:192-196` and works regardless of WHO faded out — fail sequence, last cutscene objective, or anything else. A single restoration point at the project-stop boundary is cleaner than per-call-site fades.

Concretely, modify the `stopping_project_player` task:

```cpp
TaskManager::addTask("stopping_project_player", []() -> ktwait {
    co_await playerLeaveAnyVehicle();
    if (instance().m_state.isFaded) {
        plugin::Command<plugin::Commands::DO_FADE>(MISSION_FADE_TIME_MS, 1);
        co_await MISSION_FADE_TIME_MS;
        instance().m_state.isFaded = false;
    }
    instance().m_state.isPlaying = false;
    instance().transitionPlayingState(false);
    LDYOM_INFO("Project player stopped");
});
```

This also fixes a latent v1 corner: a project that ends mid-cutscene-fadeout via a user-triggered stop (UI button or LDSTOP cheat) currently leaves the player faded.

### Lua exposure of `MissionFailAction`

Each alternative becomes a sol2 usertype with its own constructor and field accessors:

```cpp
auto endProj = lua_state.new_usertype<mission_fail_actions::EndProject>("MissionFailEndProject");
endProj["fadeOut"] = &mission_fail_actions::EndProject::fadeOut;

auto restartScene = lua_state.new_usertype<mission_fail_actions::RestartScene>("MissionFailRestartScene");
restartScene["fadeOut"] = &mission_fail_actions::RestartScene::fadeOut;

auto gotoScene = lua_state.new_usertype<mission_fail_actions::GotoScene>("MissionFailGotoScene");
gotoScene["sceneId"]     = &mission_fail_actions::GotoScene::sceneId;
gotoScene["objectiveId"] = &mission_fail_actions::GotoScene::objectiveId;  // sol2 supports optional<string>
gotoScene["fadeOut"]     = &mission_fail_actions::GotoScene::fadeOut;
```

`mission_start::Data::failAction` is bound NOT via the v1 macro (which would ignore the variant — sol2 does not auto-bind `std::variant` member fields) but as an explicit sol2 property:

```cpp
ut["failAction"] = sol::property(
    [](const Data& d) -> sol::object {
        // returns active alternative as its usertype
    },
    [](Data& d, sol::object value) {
        // accepts any of the three usertypes; throws on type mismatch
    });
```

The setter does an `is<T>()` check across the three alternatives and assigns into the variant. The getter does a `std::visit` and returns the active alternative wrapped via `sol::make_object`. Authors get:

```lua
data.failAction = MissionFailGotoScene.new()       -- builder
data.failAction.sceneId = some_scene_uuid           -- field access on variant active alternative
data.failAction.objectiveId = some_obj_uuid
data.failAction.fadeOut = true

-- read-side:
local action = data.failAction
if getmetatable(action) == MissionFailRestartScene then ... end
```

Rationale: this is the binding shape that v1's design.md described as "appropriate when a second alternative lands." It is hand-written, but small (~30 lines for the property, plus three usertype declarations). Unlike a tagged-table shape, it preserves type-safety on both sides — the C++ variant remains the canonical store, Lua just hands it strongly-typed values.

The `SOL_LUA_FOR_EACH` call on `mission_start::Data` is updated to include `failAction` (via a different macro path that knows about the variant), or — simpler — `failAction` is bound separately after the macro and the macro's field list excludes it. The latter is consistent with how `gameTitleText` and `gameEndText` are excluded.

### Editor combo with conditional payload

```
[combo: Fail action ▼]
  - End project       (default)
  - Restart scene
  - Go to scene  → renders below:
                    [combo: Target scene ▼]   (lists scenes by name)
                    [combo: Target objective ▼] (lists objectives in selected scene; "<first>" sentinel)
[checkbox: Fade out]   (always rendered; bound to active alternative's fadeOut)
```

When the combo selection changes, the active variant alternative is replaced with a default-constructed instance of the new alternative. The previous alternative's `fadeOut` value is NOT preserved across the swap (each arm has its own field), keeping the editor predictable.

The `fadeOut` checkbox is rendered AFTER the conditional payload section, so its position is stable across selections.

Rationale: matches the user's "да" to "render conditional payload below combo." Showing the combo at the top with payload below is the natural ImGui pattern (mirrors `clock.h`'s conditional rows). Resetting `fadeOut` per-alternative is conservative — the alternative is the user's primary choice; secondary fields follow.

## Risks / Trade-offs

- **[Risk]** Lua failAction setter must reject unknown types loudly. If a user assigns a non-MissionFailAction usertype (e.g. a regular table), the setter must throw a clear sol2 error rather than silently dropping the assignment. → Mitigation: explicit `is<T>()` checks across the three alternatives; on no match, throw `sol::error` with a message naming the expected types. Tested in 9.7.

- **[Risk]** Migration: legacy projects with `EndProject` and a Custom fail text now fade out by default. Authors who hand-tested in v1 see a behavior change. → Mitigation: toggle is one click in the editor; documented in the proposal's Migration section. The user accepted this trade-off explicitly ("ставь true по умолчанию").

- **[Risk]** `runScenesLoop` extraction touches the project's hottest coroutine. A copy/paste error in the boundary between startup and the loop could double-fade or skip a step. → Mitigation: the extraction is purely structural — no logic changes to the loop body. Verify by diffing line ranges and running the v1 test plan (9.10 in the v1 archive: scene transitions still work) PLUS the new fail-action tests.

- **[Risk]** A `GotoScene { sceneId }` whose `sceneId` no longer exists (scene was deleted post-authoring) currently crashes `ScenesManager::loadScene`. → Mitigation: the editor combo only lists existing scenes; programmatic Lua assignment is the only path that could supply a stale id. A best-effort guard (check `getScenesInfo()` for the id, fall through to `EndProject` with `LDYOM_ERROR` if missing) is cheap and added in task 5.4. Same applies to `objectiveId`: missing objectives fall through to `startIndex = 0` (already documented in the consumption-point pseudocode).

- **[Risk]** Re-spawning `run_project_player` while `stopping_project_player` is in flight (edge case: user triggers `LDSTOP` mid-fail-banner during navigation arm). → Mitigation: the safety-net `removeTask("mission_fail_sequence")` in `transitionPlayingState(false)` already prevents the visit arm from running after a user-stop. Add a sanity check at the top of `navigationalArmContinue`: if `!isPlaying`, do nothing and return.

- **[Trade-off]** Per-alternative `fadeOut` field vs. one global `MissionModeState::fadeOut`. Per-alternative is slightly more verbose but matches the editor UX (each arm remembers its own toggle) and is more extensible for future arms that might want different defaults. Accepted.

- **[Trade-off]** `pendingObjectiveJump` consumption point added at scene-loop start vs. teaching `startCurrentProject` (or a new method) about a starting objective. The double consumption point is uglier but tiny; introducing a `currentStartingObjectiveId` would touch more state and the start path. Accepted: scope-minimal beats elegance here.

- **[Trade-off]** Fade-in on the stop path applies UNIVERSALLY (any time `isFaded` at stop, fade in). This is intentional — it also fixes the latent v1 corner where a stop-during-cutscene-fadeout left the screen black. If anyone authored content that relied on "post-stop screen stays black," they break here. No known authoring relies on this; risk accepted.

## Migration Plan

JSON forward-compatibility:

- Legacy `failAction: { type: "end_project", data: {} }` → `EndProject { fadeOut = true }` (default applied because the field is absent in legacy JSON; nlohmann's `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT`-style `from_json` honors C++ defaults for missing keys).
- New tags `restart_scene` and `goto_scene` are unknown to v1 readers — projects using them WILL NOT load on v1. This is the same forward-compat profile as adding any new objective to the codebase. Acceptable: v1 is shipped, projects targeting v2 features are v2-only.

Behavior change:

- Existing `EndProject` + Custom fail text now fades out before the project stops. Authors who want v1 behavior toggle the new `Fade out` checkbox to off. No silent breakage of mechanics — just a UX shift.

Rollback:

- Reverting this change makes any project JSON containing `restart_scene` or `goto_scene` fail-action tags unparseable. `from_json` throws a descriptive error naming the unknown type. Same rollback shape as v1.

## Open Questions

- Localization key for the fade-out checkbox (`objectives.core.mission_start.fade_out`) — verify there isn't already a sibling key in `mission_end` we can share. If `mission_end` already has `fade_out`, both objectives use the same key.
- Whether the `MissionFailGotoScene.objectiveId` Lua field should accept `nil` to mean "first objective" (preferred — matches `std::optional<std::string>`'s natural Lua mapping).
- Does the editor need a "preview which objective" feature when an objective combo is shown for `GotoScene`? Probably not for v2; revisit if authors request it.
