---@meta

-- ─── SceneInfo ────────────────────────────────────────────────────────────────

---Metadata for a scene (name and UUID string).
---@class SceneInfo
---@field name string Display name of the scene (read-write).
---@field id string UUID string of the scene (read-only).
local SceneInfo = {}

-- ─── Objective ────────────────────────────────────────────────────────────────

---An objective in the current scene's objective list.
---@class Objective
---@field name string Display name of the objective (read-write).
---@field type string Objective type identifier (read-only).
---@field id string UUID string of the objective (read-only, computed property).
---@field is_initialized boolean Whether the objective has been started (read-only).
local Objective = {}

-- ─── scenes ───────────────────────────────────────────────────────────────────

---Scene management API for the currently loaded scene.
---@class scenes
scenes = {}

---Returns the SceneInfo (name and id) of the currently loaded scene.
---@return SceneInfo
function scenes.get_info() end

---Returns all objectives in the currently loaded scene as Objective usertypes.
---@return Objective[]
function scenes.get_objectives() end

---Returns all entities in the currently loaded scene as Entity usertypes.
---@return Entity[]
function scenes.get_entities() end

---Finds an entity by its UUID string. Returns nil if not found.
---@param id string UUID string
---@return Entity|nil
function scenes.find_entity(id) end

---Finds the first entity whose name matches the given string. Returns nil if not found.
---@param name string Entity display name
---@return Entity|nil
function scenes.find_entity_by_name(name) end

---Calls `spawn()` on all components of the given entity.
---Accepts either an Entity usertype or a UUID string.
---@overload fun(entity: Entity)
---@overload fun(id: string)
---@param entity_or_id Entity|string
function scenes.spawn_entity(entity_or_id) end

---Calls `despawn()` on all components of the given entity.
---Accepts either an Entity usertype or a UUID string.
---@overload fun(entity: Entity)
---@overload fun(id: string)
---@param entity_or_id Entity|string
function scenes.despawn_entity(entity_or_id) end
