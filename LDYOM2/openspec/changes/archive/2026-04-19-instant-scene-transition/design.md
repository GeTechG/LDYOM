## Context

LDYOM2's `ProjectPlayer` currently forces every scene-switch through a cinematic fade: before tearing down the outgoing scene it issues a 500ms `DO_FADE` to black, waits 600ms, sets `isFaded = true`, swaps the scene, and the new scene's first-objective prelude fades back in (guarded by the `if (isFaded)` check in `project_player.cpp:128`). That 1.1s-plus curtain is baked into `ProjectPlayer::requestSceneTransition` and its consumers (`goto_scene`, `goto_scene_if_var`, and the Lua binding `project_player.request_scene_transition`).

Authors building hub menus, room-to-room navigation, or quick retries want the scene change to feel instantaneous while still preserving the cinematic fade for story beats. The choice is contextual to the calling site — the same destination scene may be entered cinematically from one objective and instantly from another — so the signal needs to ride with the transition request, not with the destination scene.

Related precedent in the codebase: the `core.cutscene` objective already owns its own fade handling via the special-case at `project_player.cpp:120-124`, so "fade policy travels with the calling objective" is an established pattern.

## Goals / Non-Goals

**Goals:**

- Per-objective opt-in to skip the scene-transition fade-out/wait block for `goto_scene` and `goto_scene_if_var`.
- Keep current default behavior (cinematic fade) for existing projects and callers — zero-config backward compatibility.
- Extend the Lua-facing `request_scene_transition` API so scripted transitions can opt in symmetrically.
- Surface the new control in the editor with a tooltip warning about visible artifacts (entity pop-in, camera jumpcut, weather/time snap, scene name banner).
- Keep the player's `isFaded` state coherent so the next scene's first objective does not accidentally fade in from a non-black screen.

**Non-Goals:**

- The mission-start fade handshake at `project_player.cpp:33-43` is untouched.
- No per-project or per-scene "default transition style" setting — user explicitly wants per-call granularity ("иногда да, иногда нет").
- No configurable fade duration — the 500/600ms timings stay as-is.
- No changes to non-scene objectives' fade behavior (`core.cutscene`, `core.player_animation`).

## Decisions

### Granularity: per-objective

The `instantTransition` flag lives on each `goto_scene` / `goto_scene_if_var` objective Data struct and is forwarded through `requestSceneTransition`.

Rationale: the cutscene objective already models fade policy per-call, so this matches an existing pattern. It gives authors exact control at the call site — the same `MainMenu` scene can be entered cinematically from the intro and instantly from a pause-menu back-button.

Alternatives rejected:

- **Per-scene flag on the destination** — creates a coupling where every caller must know the destination's preference, and makes it impossible to enter the same scene instantly from one site and cinematically from another.
- **Per-project toggle** — too coarse; the user explicitly stated "иногда да, иногда нет", which rules out a single global default.
- **Hybrid (project default + per-objective override)** — two sources of truth, more surface area for bugs, and the single-flag design already covers every use case the user described. Can be layered on later if real need emerges.

### Pending transition state: single struct, not parallel fields

Replace the existing `std::optional<std::string> pendingSceneTransition` in `ProjectPlayerState` with `std::optional<PendingTransition>`, where `PendingTransition { std::string sceneId; bool instant; }`.

Rationale: the scene id and instant flag are conceptually one pending request. Wrapping them together makes it impossible to reset one without the other, and makes `has_value()` a single atomic check. The alternative — adding a parallel `bool pendingTransitionInstant` — introduces two half-states and requires resetting both in every code path that currently touches the optional (the break at `project_player.cpp:138` and the consume block at `project_player.cpp:160-162`). Easy to miss one, and a forgotten reset silently poisons the next transition.

### JSON backward compatibility via WITH_DEFAULT

Both Data structs adopt `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT`. `goto_scene_if_var` already uses it (see `goto_scene_if_var.h:40`); `goto_scene` currently uses the non-default macro and must be upgraded.

Rationale: `WITH_DEFAULT` silently substitutes the default-constructed field value when a JSON key is absent, so `{"sceneId": "..."}` from an existing project loads with `instantTransition = false` — exactly the required behavior, zero custom code.

Alternative rejected: custom `from_json` that manually defaults missing keys. More code, more review surface, identical runtime behavior. Only worth it if `WITH_DEFAULT` were unavailable — and the macro is already in active use in this same file tree, which is sufficient proof it works with the vendored nlohmann version.

### Lua API: positional optional second argument

`request_scene_transition(sceneId, instant)` where `instant` is an optional boolean defaulting to `false`.

Rationale: the current signature is single-arg and positional; a second optional positional arg preserves the call style of every existing caller and keeps the Lua binding one line. The declaration file in `../ingame/definitions/core` gets an extra optional param annotation.

Alternative rejected: options-table form `request_scene_transition(sceneId, {instant=true})`. More ceremonious for a single flag, and there is no foreseeable second option that would benefit from table syntax. If future flags arrive we can add a third overload or migrate without breaking callers of the positional form.

### Fade-state invariants on instant path

When `instant == true`, `requestSceneTransition` records the flag in the pending state but performs no immediate fade. In the consume block at `project_player.cpp:160-177`, the fade-out/wait is gated on `!instant`; `isFaded` stays `false`. The next scene's per-objective prelude at line 128 then sees `isFaded == false` and skips the fade-in — exactly the desired instant-swap behavior. The existing guard is doing the work; no new branch is needed in the prelude.

## Risks / Trade-offs

- [Risk] Visible artifacts when fade is skipped — entity pop-in, camera jumpcut, weather/time snap, scene name banner flashing mid-transition. → Mitigation: editor checkbox ships with a localized tooltip explicitly listing these artifacts so authors opt in with eyes open. No runtime mitigation — the artifacts are intrinsic to an instant swap and actually desired for hub-menu patterns.

- [Risk] `NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT` might not exist in the vendored nlohmann version. → Mitigation: `goto_scene_if_var.h:40` already uses it in the current tree, which is proof of availability. Tasks.md task 1 still verifies the version as a cheap sanity check before touching multiple files.

- [Risk] Stale `isFaded` state if instant and cinematic transitions interleave (e.g., instant transition fires while `isFaded` is somehow true from a prior uncompleted fade). → Mitigation: the current flow only sets `isFaded = true` inside the fade-out block itself, so on the instant path the flag is provably already `false` when we reach the consume site. The guard at line 128 handles the edge case where some future code path leaves `isFaded` true — the next scene's first objective will still fade back in, which is the safer of the two failure modes.

- [Risk] Lua callers with positional args may accidentally pass a non-boolean truthy value (string, number) as `instant`. → Mitigation: sol2 coerces common truthy/falsy values predictably; the binding lambda receives `sol::optional<bool>` and treats missing/nil as `false`. Documented in the declaration file.

- [Trade-off] The per-objective flag duplicates two serialized fields across `goto_scene` and `goto_scene_if_var`. Accepted: the two objectives already duplicate `sceneId` for the same reason, and a shared base struct would be over-abstraction for two fields.

## Migration Plan

No schema migration. Existing project JSON loads untouched because `WITH_DEFAULT` substitutes `instantTransition = false` for the absent key. Existing Lua scripts calling `request_scene_transition(sceneId)` continue to work because the second parameter is optional and defaults to `false`. Rollback is a clean revert: if the change is reverted, project files saved with the new field simply have an unknown key that nlohmann ignores during `from_json`, and the Lua binding reverts to single-arg (scripts passing a second arg would then silently drop it — acceptable for a rollback scenario).

## Open Questions

- Should the editor checkbox live inline next to the scene picker, or on a second row? Resolve during apply based on how crowded `goto_scene_if_var`'s editor already is (it has four rows). Purely cosmetic, no spec impact.
- Should the Lua declaration file mark the second parameter with a `@default false` annotation or just `boolean?`? Pick whatever matches the convention already used by other optional-param bindings in `../ingame/definitions/core` — verify during the declaration-update task.
