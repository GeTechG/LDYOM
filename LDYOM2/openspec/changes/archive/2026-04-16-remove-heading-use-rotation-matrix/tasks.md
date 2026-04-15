## 1. Utility: quaternion → yaw

- [x] 1.1 Add a helper (e.g. `utils/quat_yaw.h`) that extracts yaw in radians from `CQuaternion` using `atan2(2*(w*z + x*y), 1 - 2*(y*y + z*z))`.
- [x] 1.2 Add an inverse helper that builds an identity-Z-axis `CQuaternion` from a yaw in radians, for use by copy-player-heading and the legacy migration.

## 2. Actor component

- [x] 2.1 Remove `float initialDirection` from `components::Actor` (header and all initializers).
- [x] 2.2 Remove the direction slider UI block in `Actor::editorRender` (including the "copy_player_heading" button — re-wire it in 2.3).
- [x] 2.3 Re-wire the "copy player heading" helper to set `entity->rotation` via the helper from 1.2 (using the player's current heading in radians).
- [x] 2.4 Update `Actor::updateDirection()` to compute yaw from `entity->rotation` via 1.1 and apply it via `SET_CHAR_HEADING` (convert rad → deg).
- [x] 2.5 Stop writing `initialDirection` in `Actor::to_json`; remove the key.
- [x] 2.6 In `Actor::from_json`, if legacy `initialDirection` is present, stash it on a transient member (e.g. `std::optional<float> legacyInitialDirection`) for the entity-level migration step.

## 3. Vehicle component

- [x] 3.1 Remove `float initialDirection` from `components::Vehicle` (header and all initializers).
- [x] 3.2 Remove the direction slider UI block in `Vehicle::editorRender`.
- [x] 3.3 Update `Vehicle::updateDirection()` to compute yaw from `entity->rotation` via 1.1 and apply it via `SET_CAR_HEADING` (convert rad → deg).
- [x] 3.4 Stop writing `initialDirection` in `Vehicle::to_json`; remove the key.
- [x] 3.5 In `Vehicle::from_json`, stash any legacy `initialDirection` on a transient member for the entity-level migration.

## 4. Entity-level migration

- [x] 4.1 In `Entity::from_json` (after components are loaded), if any component reports a legacy direction and the entity rotation is identity (within epsilon), set `entity.rotation` from the legacy yaw using the helper from 1.2.
- [x] 4.2 If the entity rotation is already non-identity, drop the legacy value and log a one-time `spdlog` warning identifying the entity.
- [x] 4.3 Clear the transient legacy field after migration so it never round-trips.

## 5. Manual in-game vehicle placement

- [x] 5.1 In `src/rendering/in_game/vehicle_manual_editing.cpp` (around line 97), replace the write to `m_vehicle->initialDirection` with a write to `m_vehicle->entity->rotation` using the helper from 1.2 (converting the GTA heading in radians to a Z-axis quaternion).

## 6. Lua bindings

- [x] 6.1 Grep `sol_lua_register` blocks for `initialDirection` in actor/vehicle. If exposed, remove the binding and note it in the change summary (BREAKING).
- [x] 6.2 Verify `ActorComponentDirtyFlags::Direction` still has a valid meaning (triggered when rotation callback fires).

## 7. Lua declarations and localization

- [x] 7.1 Update the Lua declaration files under `../ingame/definitions/core` to remove `initialDirection` from actor/vehicle types.
- [x] 7.2 Remove unused localization keys (`direction`) from `../ingame/languages/en.json` if no other component still uses them. Keep `copy_player_heading` (still used).

## 8. Validation

- [x] 8.1 Build Debug; fix all compile errors from removed field.
- [ ] 8.2 Manual test: create an actor, rotate via gizmo, play the scene, verify in-game heading matches.
- [ ] 8.3 Manual test: same for a vehicle (including manual in-game placement round-trip).
- [ ] 8.4 Manual test: load an existing project that used `initialDirection`, verify heading is preserved and the field is gone from the re-saved JSON.
- [ ] 8.5 Manual test: "copy player heading" updates the gizmo rotation and spawned actor heading.

## 9. Full rotation via direct matrix write

Goal: replace the yaw-only `SET_CHAR_HEADING` / `SET_CAR_HEADING` path with a direct write of the full rotation matrix built from `entity->rotation`, so actors and vehicles can be oriented on all 3 axes (pitch/roll/yaw), matching the gizmo.

Reference (reverse-engineered opcodes):
- `SET_CHAR_HEADING` (371): writes `m_fCurrentRotation`/`m_fAimingRotation`, calls `CPlaceable::SetHeading` (yaw only via `SetRotateZOnly`), then `CEntity::UpdateRW_`. Guards: skips when ped is in a vehicle (`m_nPedFlags[0] & 0x100`).
- `SET_VEHICLE_QUATERNION`: builds a `CMatrix` from a quaternion via `CQuaternion::Get`, preserves the existing position (from attached entity matrix or placeable), calls `CPlaceable::SetMatrix`, then restores position via `setPosition`.

Tasks:
- [x] 9.1 Use existing `CMatrix::SetRotate(CQuaternion)` + `UpdateRW` instead of introducing a new helper — position is preserved by `SetRotate` (it only writes the 3x3 rotation block), so a bespoke quat-to-matrix helper isn't needed.
- [x] 9.2 Replace `Vehicle::updateDirection()` (yaw via `SET_CAR_HEADING`) with a direct matrix write: `SetRotate(entity->rotation)` + `m_matrix->UpdateRW()` + `UpdateRwMatrix()` + `UpdateRwFrame()`. Renamed method to `updateRotation()`, dirty flag `Direction` → `Rotation` in both components, updated `vehicle_manual_editing.cpp`.
- [x] 9.3 Replace `Actor::updateDirection()` similarly for `CPed`: full matrix write + RW update, plus sync of `m_fCurrentRotation` / `m_fAimingRotation` from yaw. In-vehicle guard implemented via `ped->m_pVehicle != nullptr` (matches opcode 371's intent).
- [x] 9.4 Dropped the `SET_CHAR_HEADING` / `SET_CAR_HEADING` calls from both components. `quaternionToEuler` remains only for the actor aim-rotation sync.
- [x] 9.5 Spawn paths verified: `Actor::spawn()` ends with `updatePosition(); updateRotation();` and `Vehicle::spawn()` calls `updateRotation()`, so freshly spawned entities receive the full matrix.
- [x] 9.6 Manual test: rotate an actor on pitch/roll via gizmo, play scene → in-game ped matches all 3 axes (allowing for ragdoll/AI snap-back — document if the engine forces upright after a frame).
- [x] 9.7 Manual test: rotate a vehicle on pitch/roll (e.g. tilted on a ramp pose), play scene → vehicle matrix matches before physics settles it.
- [x] 9.8 Manual test: actor inside a vehicle — confirm the ped rotation write is skipped and does not desync the vehicle seat transform.
