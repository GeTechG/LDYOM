## ADDED Requirements

### Requirement: Single source of truth for entity orientation
The system SHALL store an entity's orientation exclusively in `Entity::rotation` (a quaternion). Components that previously carried their own scalar heading (actor, vehicle) SHALL NOT maintain a separate persisted orientation field.

#### Scenario: Actor rotation edited via gizmo
- **WHEN** a user rotates an actor entity with the transform gizmo on any axis (pitch/roll/yaw)
- **THEN** `Entity::rotation` updates and the actor's in-game transform matrix is updated on the next rotation sync to match the full quaternion (all 3 axes)

#### Scenario: Vehicle rotation edited via gizmo
- **WHEN** a user rotates a vehicle entity with the transform gizmo on any axis (pitch/roll/yaw)
- **THEN** `Entity::rotation` updates and the vehicle's in-game transform matrix is updated on the next rotation sync to match the full quaternion (all 3 axes)

### Requirement: No per-component direction slider for actor and vehicle
The actor and vehicle editor panels SHALL NOT expose a scalar "direction" slider. Orientation editing for these components SHALL be performed through the transform gizmo only.

#### Scenario: Actor editor panel
- **WHEN** the actor editor panel is rendered
- **THEN** it does not contain a direction / heading slider widget

#### Scenario: Vehicle editor panel
- **WHEN** the vehicle editor panel is rendered
- **THEN** it does not contain a direction / heading slider widget

### Requirement: Spawn orientation derived from entity rotation
On spawn and on rotation-dirty updates, actor and vehicle components SHALL apply the full rotation from `Entity::rotation` by writing the quaternion directly into the GTA object's transform matrix via `CMatrix::SetRotate(CQuaternion)` followed by `UpdateRW`/`UpdateRwMatrix`/`UpdateRwFrame`. Yaw-only opcodes (`SET_CHAR_HEADING`, `SET_CAR_HEADING`) SHALL NOT be used for this sync.

#### Scenario: Actor spawn with non-identity entity rotation
- **WHEN** an actor entity with a non-identity rotation (any axis) is spawned
- **THEN** the resulting `CPed` has its transform matrix set from the quaternion on all 3 axes, and `m_fCurrentRotation` / `m_fAimingRotation` are synced from the yaw component so AI/aim code remains coherent

#### Scenario: Vehicle spawn with non-identity entity rotation
- **WHEN** a vehicle entity with a non-identity rotation (any axis) is spawned
- **THEN** the resulting `CVehicle` has its transform matrix set from the quaternion on all 3 axes

#### Scenario: Actor attached to a vehicle
- **WHEN** a rotation sync runs for an actor whose `m_pVehicle` is non-null
- **THEN** the matrix write is skipped so the vehicle's seat transform is not desynced (mirrors opcode 371's in-vehicle guard)

### Requirement: Copy-player-heading writes to entity rotation
The "copy player heading" helper available on the actor editor SHALL write a Z-axis quaternion derived from the player's heading into `Entity::rotation`, not into any per-component scalar.

#### Scenario: User invokes copy-player-heading
- **WHEN** the user clicks the "copy player heading" button on an actor
- **THEN** the entity's `rotation` is updated to a quaternion representing the player's current heading about the Z axis

### Requirement: Legacy `initialDirection` migration on load
When loading a project JSON that contains a legacy `initialDirection` field on an actor or vehicle component, the system SHALL fold that angle into the parent entity's rotation quaternion as a Z-axis rotation, provided the entity's stored rotation is the identity quaternion. The field SHALL NOT be written back on save.

#### Scenario: Legacy project with initialDirection and identity entity rotation
- **WHEN** the system loads a project where an actor/vehicle component has `initialDirection != 0` and its entity rotation is identity
- **THEN** the entity rotation is set to a Z-axis quaternion equal to `initialDirection` radians, and on subsequent save the JSON contains no `initialDirection` field

#### Scenario: Legacy project with initialDirection and non-identity entity rotation
- **WHEN** the system loads a project where the entity rotation is already non-identity and the component has a legacy `initialDirection`
- **THEN** the entity rotation is kept as stored, `initialDirection` is dropped, and a one-time warning is logged for that entity

#### Scenario: Saving after migration
- **WHEN** a previously-migrated project is saved
- **THEN** the produced JSON does not contain any `initialDirection` key on actor or vehicle components
