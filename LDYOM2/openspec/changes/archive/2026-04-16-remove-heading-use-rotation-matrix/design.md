## Context

`Entity` stores a full orientation as `CQuaternion rotation`, edited via the transform gizmo. Two components — `components::Actor` and `components::Vehicle` — additionally store a scalar `float initialDirection` and expose it as an `ImGui::SliderAngle` titled "direction". On spawn, they apply only this scalar via `SET_CHAR_HEADING` / `SET_CAR_HEADING` and ignore the entity's quaternion. Manual in-game vehicle editing writes back to `initialDirection` on placement (`vehicle_manual_editing.cpp:97`).

The two fields are unsynchronized: gizmo edits never touch `initialDirection`, slider edits never touch `Entity::rotation`. The user wants a single source of truth — the entity rotation — edited via the gizmo.

GTA internals confirm the model: `CPlaceable::SetHeading(float)` calls `CMatrix::SetRotateZOnly`, and `SET_CHAR_HEADING` ends at the same call. So applying yaw-only from the quaternion's Z component is faithful to existing runtime behavior; no broader rotation semantics change.

## Goals / Non-Goals

**Goals:**
- Remove `initialDirection` from actor and vehicle components (field, UI slider, JSON key, Lua binding if present).
- Derive spawn heading from `Entity::rotation` (yaw/Z-axis component).
- Edit orientation via the transform gizmo only.
- Migrate legacy projects silently on load.

**Non-Goals:**
- Changing pitch/roll behavior for peds/vehicles (GTA itself only applies yaw to peds via `SetHeading`; full-matrix placement for vehicles is out of scope).
- Extending orientation support to other components (object, pickup, etc.) beyond what they already do.
- Reworking the gizmo, entity rotation storage, or serialization.

## Decisions

### Decision: Use yaw extracted from quaternion, applied via existing `SET_*_HEADING` commands
**Rationale:** The reverse-engineered `CPlaceable::SetHeading` path already reduces to `SetRotateZOnly`, which is what peds and (for initial placement) vehicles use. Keeping the existing command path avoids new hook surface and preserves behavior for actor AI flags (`m_fCurrentRotation`, `m_fAimingRotation` are set inside the opcode handler).

**Alternative considered:** Apply the full `CMatrix` from the quaternion directly. Rejected: pitch/roll on peds is not a supported runtime concept in GTA's ped code; applying it invites visual artifacts and fights engine updates.

**Yaw extraction:** compute `yaw = atan2(2 * (w*z + x*y), 1 - 2 * (y*y + z*z))` from the quaternion, then convert to degrees as the opcode expects.

### Decision: Load-time migration, no runtime fallback
Introduce a one-shot migration in `Actor::from_json` / `Vehicle::from_json`: if the JSON contains `initialDirection` and the parent entity rotation is identity (within epsilon), fold the angle into `Entity::rotation` as a Z-axis quaternion. Drop the field from `to_json`. No version bump required for the project file; the key is simply ignored on subsequent loads.

**Alternative considered:** Keep `initialDirection` as a transient cache. Rejected: leaves two fields in memory and re-introduces the sync problem.

**Entity-level migration note:** `Actor::from_json` / `Vehicle::from_json` do not have direct access to the parent `Entity` rotation at the component level. The component will store the legacy angle in a transient member during `from_json`, and `Entity::from_json` (or the first `onStart`) will fold it into `Entity::rotation` if rotation is identity. Exact integration point is an implementation detail in tasks.

### Decision: Remove the "direction" UI entirely for actors and vehicles
The gizmo is the editing surface. The "copy player heading" helper stays (useful), but it writes a Z-axis quaternion into `Entity::rotation` instead of a scalar.

### Decision: Keep `updateDirection()` methods but drive them from entity rotation
`Actor::updateDirection()` / `Vehicle::updateDirection()` continue to exist as the sync path to the live GTA entity. They now read the yaw from `Entity::rotation` and call the same `SET_*_HEADING` opcode. Dirty flag `Direction` is still raised when the rotation callback fires.

## Risks / Trade-offs

- **Risk:** Existing projects with non-zero `initialDirection` and an already-set entity rotation will lose the slider angle. → **Mitigation:** migration only skips when entity rotation is non-identity; in that case log a one-time warning per project load.
- **Risk:** Lua scripts reading `actor.initialDirection` or `vehicle.initialDirection` break. → **Mitigation:** list affected bindings in tasks; if present, replace with a getter/setter that operates on entity rotation yaw, or document the break in the migration note (user preference: flag as BREAKING).
- **Risk:** Manual in-game vehicle placement (`vehicle_manual_editing.cpp`) writes to `initialDirection`. → **Mitigation:** update it to write a Z-axis quaternion into `Entity::rotation`.
- **Trade-off:** Roll/pitch set via gizmo on an actor is silently discarded at spawn (GTA limitation). Acceptable and matches prior behavior.

## Migration Plan

1. Land the code change with the load-time migration in place.
2. On first load of a legacy project, `initialDirection` is folded into `Entity::rotation`; on save the field disappears.
3. No schema version bump; old and new projects both load cleanly.
4. Rollback: revert the commit — projects saved with the new code will have lost `initialDirection` on disk, but the yaw is preserved in the entity rotation, which old code ignored. Rolling back would reset affected entities to zero heading. Document this in the commit message.
