## Why

Actor and Vehicle components expose a separate `initialDirection` slider that is not synchronized with the entity's rotation quaternion (`Entity::rotation`). Editing rotation through the gizmo has no effect on spawn heading, and editing the slider has no effect on the gizmo — the two sources of truth diverge and confuse users.

## What Changes

- **BREAKING**: Remove the `initialDirection` field from `components::Actor` and `components::Vehicle` (JSON schema change).
- Use the entity's rotation quaternion as the single source of truth for spawn/reset orientation for actors and vehicles.
- Remove the per-component "direction" slider UI from actor and vehicle editors; orientation is edited via the transform gizmo only.
- On spawn and on rotation change, apply the **full rotation matrix** derived from the entity quaternion directly to the GTA object via `CPlaceable::SetMatrix` + `CEntity::UpdateRW` (mirroring the `SET_VEHICLE_QUATERNION` opcode path), instead of the yaw-only `SET_CHAR_HEADING` / `SET_CAR_HEADING` opcodes. This lets actors and vehicles be oriented freely on all 3 axes (pitch/roll/yaw) to match the gizmo.
- For actors, additionally sync `m_fCurrentRotation` / `m_fAimingRotation` from the yaw component so AI/aiming code stays consistent; skip the matrix write when the ped is attached to a vehicle (matches opcode 371's in-vehicle guard).
- Provide a one-time JSON migration on load: if legacy `initialDirection` is present, fold it into the entity rotation quaternion (Z-axis yaw) unless the entity already has a non-identity rotation.
- Keep the "copy player heading" helper for actors, but have it write to the entity's rotation quaternion instead of `initialDirection`.

## Capabilities

### New Capabilities
- `entity-orientation`: Single-source-of-truth orientation model for entities (actors, vehicles) driven by the entity rotation quaternion and edited via the transform gizmo.

### Modified Capabilities
<!-- none -->

## Impact

- Affected code:
  - `src/data/components/actor.h`, `src/data/components/actor.cpp`
  - `src/data/components/vehicle.h`, `src/data/components/vehicle.cpp`
  - `src/rendering/in_game/vehicle_manual_editing.cpp` (writes `initialDirection` on manual edit — switch to entity rotation)
  - Lua bindings exposing `initialDirection` on actor/vehicle (if any)
  - Localization keys `direction`, `copy_player_heading` usage
- Data: existing saved projects using `initialDirection` — handled by load-time migration.
- Runtime behavior: on reset/spawn, heading is applied from entity rotation (yaw only, matching GTA's single-axis ped/vehicle heading).
