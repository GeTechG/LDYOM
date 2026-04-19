## Context

`ProjectPlayer` runs a project as a linear sequence of objectives. Several objective types (`core.timelimit`) and component watchdogs (required-actor-killed in `actor.cpp:363`, required-vehicle-destroyed in `vehicle.cpp:866`) call `ProjectPlayer::failCurrentProject()` when their fail condition hits. The scene-completion timer in `project_player.cpp:74` does the same. Today that call is indistinguishable from a hard stop — `failCurrentProject()` is one line: `stopCurrentProject();`.

GTA San Andreas itself tracks a single `int CTheScripts::OnAMissionFlag` (address `0xA476AC`, already bound in `plugin_sa/game_sa/CTheScripts.h:220`). That flag gates a lot of native game behavior (gang-war triggering, ped group logic, save-prompt suppression, radar/radio nuances). LDYOM2 has never toggled it — authored "missions" have always run as free-roam scripts from GTA's point of view.

The user wants three coordinated things:

1. Two new objectives (`mission_start`, `mission_end`) that bookend a mission and manage the flag + banner UI.
2. A runtime state on `ProjectPlayer` that carries the author's fail text and chosen fail action between mission-start and whenever a fail signal arrives.
3. A single-decision-point rewrite of `failCurrentProject()` that checks the runtime state and either plays the fail sequence or falls through to `stopCurrentProject()` as today.

Related precedent: `core.cutscene` already owns its own fade handling and writes to `ProjectPlayer::setFaded()`. "Objective configures runtime player state, ProjectPlayer respects it" is an established pattern.

## Goals / Non-Goals

**Goals:**

- Explicit mission-mode lifecycle owned by `core.mission_start` / `core.mission_end`.
- Every existing fail signal (`timelimit`, required-actor-death, required-vehicle-destruction, scene-completion timer) automatically routes through the fail sequence when mission mode is active, with zero edits to those call sites.
- Fail-action model that starts with one option (`EndProject`) but is extensible to future actions (restart scene, goto scene, respawn, …) via a variant of structs — each future action carrying its own payload.
- Guaranteed cleanup: `OnAMissionFlag` is always `0` when no project is running, regardless of how the project ended.
- Read-only Lua visibility of mission state for scripting.
- Authoring safety: nested `mission_start` is caught with a toast + log + stop; orphan `mission_end` is a harmless no-op.

**Non-Goals:**

- No fade in the fail sequence for v1 — the user explicitly deferred fade to a future fail-action option ("fade + more post-text conditions later"). The v1 fail sequence is: block controls → show text → wait → run action.
- No configurable fail-text hold duration — fixed to a single constant in the 500–1000ms range (chosen below).
- No extra fail actions beyond `EndProject` in v1. The variant is present, but populated with only one case. Future actions land via additive changes.
- No effect on free-roam projects. If neither `mission_start` nor `mission_end` is used, `OnAMissionFlag` stays `0` and `failCurrentProject()` behaves exactly as today.
- No changes to the cutscene/animation fade handshake at `project_player.cpp:120–132` or `:33–43`.
- No changes to existing fail call sites' signatures or semantics beyond the re-routed behavior inside `failCurrentProject()`.

## Decisions

### Fail-action shape: `std::variant<Action>` with JSON discriminator

The mission fail action is modeled as a `std::variant` of per-action POD structs, each with its own `TYPE` string constant. Today the variant has exactly one alternative:

```cpp
namespace mission_fail_actions {
    struct EndProject {
        static constexpr const char* TYPE = "end_project";
        NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(EndProject);  // no fields yet
    };
    // Future: struct RestartScene { std::string sceneId; ... };
}
using MissionFailAction = std::variant<mission_fail_actions::EndProject>;
```

Serialized JSON form uses a tagged envelope — `{ "type": "end_project", "data": { ... } }` — so future actions can add payload fields without schema surgery. Custom `to_json` / `from_json` free functions on `MissionFailAction` handle the tag dispatch.

Rationale: the user explicitly asked for "перечисление структур" (enum of structs), each with its own data. `std::variant` is exactly that in C++. The tagged-envelope JSON form is the standard pattern for discriminated unions in nlohmann::json — it round-trips cleanly, tolerates unknown tags during load with a clear error, and keeps each action's payload isolated from the others.

Alternatives rejected:

- **Single struct with `string type` + `nlohmann::json data`** — simpler to serialize but loses type safety on the C++ side. Every consumer would redo the tag-match dance; variant centralizes it into one `std::visit`.
- **Inheritance hierarchy with `std::unique_ptr<FailActionBase>`** — heavier (heap alloc per fail action, virtual dispatch, polymorphic clone), no concrete benefit over variant for a closed set of known action types.
- **Just `int type` enum with parallel optional data fields** — exactly the anti-pattern the user wanted to avoid by saying "перечисление структур."

### Where mission-mode state lives: inside `ProjectPlayerState`

`MissionModeState` is added as a field on `ProjectPlayerState` alongside `isFaded`, `currentObjectiveIndex`, `pendingSceneTransition`:

```cpp
struct MissionModeState {
    bool active = false;
    std::string failText;        // user-entered UTF-8 string
    std::string failTextGxt;     // cached utf8_to_cp1251 + gxt_encode result
    MissionFailAction failAction = mission_fail_actions::EndProject{};
};
```

Rationale: this state is runtime-only (not serialized with project JSON), scoped to a single project run, and read by `ProjectPlayer::failCurrentProject()` and the safety net in `transitionPlayingState(false)`. `ProjectPlayerState` is already the home for exactly this kind of ephemeral state.

Alternative rejected: a parallel singleton (`MissionModeService`). It would require duplicated reset logic in both project-start and project-stop paths, and the state has no readers outside `ProjectPlayer` anyway. Service-ification would be abstraction without a second caller.

### Caching the fail text as gxt

The user-entered UTF-8 `failText` is encoded once into `failTextGxt` during `mission_start::execute()` (mirroring how `cutscene.h:233-235` and `clock.h:64-66` cache their `gameText`). At fail time we feed `failTextGxt.data()` directly to `CMessages::AddBigMessage` without re-encoding.

Rationale: `utf8_to_cp1251 + gxt_encode` is not free, and the fail path is already sensitive (we've just cancelled the main run task and are racing to show something). Caching at authoring/start time amortizes cost and keeps the fail sequence minimal.

### Smart `failCurrentProject()` — single decision point with re-entrancy guard

`failCurrentProject()` becomes the only place that distinguishes "fail" from "stop". Crucially, the fail coroutine flips `missionMode.active = false` _before_ it awaits, so any second fail signal arriving during the 1-second banner falls through to the plain `stopCurrentProject()` branch:

```cpp
void ProjectPlayer::failCurrentProject() {
    if (!m_state.missionMode.active) {
        stopCurrentProject();
        return;
    }

    TaskManager::instance().removeTask("run_project_player");

    TaskManager::instance().addTask("mission_fail_sequence", []() -> ktwait {
        auto& st = instance().m_state.missionMode;

        // Snapshot the data we need; from here on we drop mission-mode state
        // so re-entrant failCurrentProject() calls during the banner fall
        // through to stopCurrentProject() instead of spawning a second task.
        std::string failTextGxt = st.failTextGxt;
        MissionFailAction action = st.failAction;
        st.active = false;

        plugin::Command<Commands::SET_PLAYER_CONTROL>(0, false);
        CMessages::AddBigMessage(failTextGxt.data(),
                                 MISSION_FAIL_TEXT_TIME_MS, STYLE_BOTTOM_RIGHT);
        co_await MISSION_FAIL_TEXT_TIME_MS;
        plugin::Command<Commands::SET_PLAYER_CONTROL>(0, true);

        std::visit([](auto&& a) {
            using T = std::decay_t<decltype(a)>;
            if constexpr (std::is_same_v<T, mission_fail_actions::EndProject>) {
                instance().stopCurrentProject();
            }
        }, action);
    });
}
```

Rationale: the user asked for "умное" — smart — checking in one place. Every existing call site (`timelimit.h:95`, `timelimit.h:121`, `actor.cpp:363`, `vehicle.cpp:866`, `project_player.cpp:74`) is left untouched. Adding a future fail action is one more variant arm in the `std::visit`.

The re-entrancy guard is not cosmetic — multiple fail signals firing in the same tick window is plausible in practice (required actor killed by an explosion that also destroys a required vehicle; timelimit expires during an actor-death watchdog check). Without the guard, `TaskManager::addTask("mission_fail_sequence", ...)` would be called twice with the same name, producing either a doubled banner, a dropped fail sequence, or an error depending on TaskManager's duplicate-name policy. Snapshotting into locals also protects the data against the safety net that runs later inside `stopCurrentProject()` → `transitionPlayingState(false)`, which resets `missionMode = {}` — `AddBigMessage`'s char* buffer is still being rendered when that reset happens.

Alternative rejected: making callers check `isMissionActive()` themselves. Five duplicated branches today, N branches as more fail sites arrive. Centralization is plainly better.

Alternative rejected: a separate `missionMode.failing` boolean to gate re-entry while keeping `active` true. More fields, same effect. The simpler `active = false` read during re-entry is exactly the semantics we want — once the fail sequence has started, the mission is effectively over from every other observer's point of view.

### Fail-sequence runs as its own task, not inside the cancelled run loop

`stopCurrentProject()` already uses the pattern "yank `run_project_player`, then spawn `stopping_project_player`" (see `project_player.cpp:248-261`). The fail sequence follows the same pattern: it is scheduled as a new task (`mission_fail_sequence`) _after_ `removeTask("run_project_player")`, so it runs to completion even though the main run coroutine is gone.

Rationale: the in-flight objective coroutine is already effectively cancelled (no graceful unwinding exists in the current design — `TaskManager::removeTask` just drops the task). Trying to play the fail sequence inside the current objective would require either adding a cancel/exception pathway through every `co_await` site or trusting each objective to poll a flag. Spawning a separate task sidesteps both. The `stopping_project_player` precedent proves this works.

The final action (e.g. `EndProject` → `stopCurrentProject()`) then runs after the text has held. `stopCurrentProject()` will itself try to `removeTask("run_project_player")`, which is a no-op since we already removed it — harmless.

### Safety net in `transitionPlayingState(false)`

Adds three lines to the project-stop branch (around `project_player.cpp:325`):

```cpp
TaskManager::instance().removeTask("mission_fail_sequence");
CTheScripts::OnAMissionFlag = 0;
instance().m_state.missionMode = {};
```

This covers every termination path: user hits `LDSTOP`, UI stop button, `mission_end` executed normally, scene-completion timer fires, fail sequence completes and calls `stopCurrentProject()`, or the project crashes out. Whatever the path, `OnAMissionFlag` is `0` when the free-roam state returns, and no `mission_fail_sequence` task is left orphaned showing a banner over free-roam.

Rationale: the user explicitly called this out ("при завершении проекта нужно учесть выключение если пользователь не выключил сам"). Putting it in `transitionPlayingState(false)` — which is the single choke point for "project stopped" cleanup — means there is no missable path. Doubly safe: even if `mission_end` ran successfully a moment earlier and already cleared the flag, the safety-net write is idempotent.

The `removeTask("mission_fail_sequence")` line closes an edge where the user hits `LDSTOP` mid-banner. Without it, the fail coroutine would survive its parent run task and eventually hit `stopCurrentProject()` → `removeTask` → `!isPlaying` early-return (no crash, but wastes a tick and produces a confusing log line). Cheaper to remove it up front, and it sits next to the existing `removeTask("stop_cheat")` / `removeTask("project_tasks")` / `removeTask("scene_completion_timer")` calls in the same branch.

### Nested `mission_start` — stop with toast + log

When `mission_start::execute()` runs and `missionMode.active` is already `true`:

```cpp
if (ProjectPlayer::instance().isMissionActive()) {
    auto& sc = ScenesManager::instance().getUnsafeCurrentScene();
    int idx = ProjectPlayer::instance().getCurrentObjectiveIndex();
    Notifications::error(
        _("objectives.core.mission_start.nested_error.title"),
        fmt::format(_("objectives.core.mission_start.nested_error.body"),
                    sc.info.name, idx));
    LDYOM_ERROR("mission_start called while mission already active "
                "(scene='{}', objective_index={})", sc.info.name, idx);
    ProjectPlayer::instance().stopCurrentProject();
    co_return;
}
```

Rationale: the user chose "проект завершиться а пользователю будет предупреждение что нельзя запускать внутри миссии ещё одну и указание в какой сцене, какая цель проблемная." `Notifications::error` is the existing toast path (`src/core/notifications.h`), `LDYOM_ERROR` is the standard logger, and `stopCurrentProject()` (not `failCurrentProject()`) is correct here — this is an authoring bug, not a mission fail. Routing through the fail sequence would obscure the problem.

### Orphan `mission_end` — silent no-op

When `mission_end::execute()` runs and `missionMode.active` is `false`, the objective does nothing — no flag write, no banner, no fade. Control returns, next objective runs normally.

Rationale: the user explicitly chose "Если будет вызвано завершить миссию до запуска миссии то ничего не произойдет и цели дальше пойдут выполняться." This is the least surprising behavior — `mission_end` is naturally idempotent, and this keeps it that way.

### Banner timings — two constants

```cpp
constexpr int MISSION_FAIL_TEXT_TIME_MS = 1000;  // fail text hold
constexpr int MISSION_FADE_TIME_MS      = 500;   // uniform fade duration
```

Rationale: the user accepted "статичное типо 500-1000мс." `500` for fade matches every other fade call in the codebase (`cutscene.h:86`, `project_player.cpp:122`, `:167`, `:191`). `1000` for the fail banner matches the scene-name banner at `project_player.cpp:54`. Putting both as named constants in `project_player.h` makes them easy to retune without chasing magic numbers.

### Lua visibility — read-only `isMission`, no variant binding in v1

`projectPlayer.isMission` maps to `ProjectPlayer::isMissionActive()`. Read-only. No Lua-facing API to flip the flag or alter the fail action — those are the authored objectives' job, and exposing them to Lua would create two sources of truth.

The `failAction` variant is deliberately **not** exposed to Lua in v1. sol2 does support `std::variant`, but the binding is not the free one-liner that cutscene/clock Data structs get via `SOL_LUA_FOR_EACH` — each alternative needs to be registered as a usertype, and round-tripping a Lua-constructed variant back into the C++ Data field requires a hand-written `from_lua` path. With only one alternative today (`EndProject`, no payload), the work does not justify itself. When a second alternative lands (e.g. `RestartScene { string sceneId }`), the Lua binding is sized appropriately and arrives in the same change that motivates it.

In v1, the `SOL_LUA_FOR_EACH` call on `mission_start::Data` elides `failAction`. Authors configure the fail action through the ImGui combo in the editor; Lua cannot read or write it. This is called out in the tasks.md task for the Lua declaration file so the gap is documented in-place.

Rationale: the user said "в lua можно забиндить состояние миссии." Scripts need to observe whether the player is in a mission (to gate UI, adjust spawn rates, etc.). They do not need to construct mission data from Lua.

## Risks / Trade-offs

- **[Risk]** `failCurrentProject()` is called from inside coroutines spawned by `TaskManager::addTask` (e.g. `actor.cpp:363`'s actor-death watch, `project_player.cpp:74`'s completion timer). After `removeTask("run_project_player")`, the caller coroutine itself may still be in flight; its post-call code runs. → Review shows every caller treats `failCurrentProject()` as fire-and-forget — nothing reads `isPlaying` after the call expecting a specific value. `completion_timer` breaks out of its loop via the `getTimerTime() > 0` condition. Safe.

- **[Risk]** `CMessages::AddBigMessage` requires a mutable `char*` and cp1251-encoded gxt text. If `failTextGxt` is mutated concurrently the data pointer becomes unstable. → Mitigation: `failTextGxt` is written exactly once (in `mission_start::execute()`) and read exactly once (in the fail-sequence task). No aliasing. Same pattern already works for `clock::Data::gameText` and `cutscene::Data::gameText`.

- **[Risk]** The mission-fail sequence runs _after_ the objective loop is cancelled, so any fade state the loop was mid-way through (e.g. `isFaded == true` because the last objective faded out) persists. Banner on black screen is hard to read. → Mitigation: the v1 fail sequence does no fade of its own, so if the screen is already black the user sees nothing. Accepted trade-off — the user deferred fade behavior to a future fail-action option. Documented in Non-Goals. When the `FadeThenEnd` action is added, it will explicitly fade-in first.

- **[Risk]** `OnAMissionFlag` is a global game variable; other plugins or the base game could also write it. → Not our concern. We set it exactly when `mission_start` runs, clear it in `mission_end` and in the safety net. If another plugin stomps it mid-project, that is that plugin's bug.

- **[Risk]** Using `std::variant` forces `<variant>` into the `project_player.h` header, dragging includes into every `project_player.h` consumer. → Acceptable cost; `<variant>` is a lightweight standard header and `ProjectPlayer` is not an amalgamation bottleneck. Alternative (forward-declare the variant in the header and define in .cpp) is possible but makes inlining the trivial `isMissionActive()` accessor awkward.

- **[Trade-off]** Fail-text hold duration is fixed, not per-objective. Accepted: the user OK'd a static value; variability can be added later as an objective Data field without changing the capability shape.

- **[Trade-off]** Two objectives + one runtime state + one smart accessor versus a single "mission wrapper" that bookends a range of objectives. Accepted the two-objective design because it mirrors every other LDYOM2 bracketed pattern (e.g. timer start/stop), composes with scene transitions naturally (mission can span scenes if the user wants), and keeps the ImGui editor's flat objective list unchanged.

## Migration Plan

Zero migration. New objectives are additive — legacy projects contain neither and therefore never toggle `OnAMissionFlag` or trigger the new fail path. `failCurrentProject()`'s new branch is gated on `missionMode.active`, which defaults to `false` and can only be set by `mission_start`. Rollback is a clean revert: any project JSON saved with `core.mission_start` or `core.mission_end` entries becomes unparseable after revert (unknown objective type), which is the same failure mode as reverting any objective-adding change in this codebase — the ObjectivesManager errors with "unknown type" on load and skips the objective.

## Open Questions

- Banner style (resolved): `mission_start`'s title banner uses `STYLE_BOTTOM_RIGHT` to match the existing scene-name banner, while `mission_end` ("MISSION PASSED!") and the mission-fail banner ("MISSION FAILED!") both use `STYLE_MIDDLE` for a classic mission-complete feel. See `src/data/objectives/mission_start.h:157`, `src/data/objectives/mission_end.h:117,120,124`, and `src/core/project_player.cpp:285,287`.
- Localization key convention for the nested-error toast: title key `objectives.core.mission_start.nested_error.title` and body key `...nested_error.body` — verify during apply that other objectives use the same dotted-key convention for multi-line error messages.
- Lua declaration placement: `ProjectPlayer.isMission` likely belongs in the same file as `ProjectPlayer.isPlaying` — verify during apply by grepping the declarations directory for the existing `isPlaying` field.
