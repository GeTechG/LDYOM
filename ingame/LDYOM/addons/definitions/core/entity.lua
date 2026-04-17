---@meta

-- ─── Dependencies ─────────────────────────────────────────────────────────────

---Component dependency descriptor.
---@class Dependencies
---@field components string[] List of component type strings required.
---@field oneOf boolean If true, only one of the listed components is required.
local Dependencies = {}

-- ─── Component ───────────────────────────────────────────────────────────────

---Base class for all entity components.
---@class Component
---@field type string Component type identifier (e.g. "actor", "vehicle").
---@field entity Entity|nil The owning entity, or nil if not attached.
local Component = {}

---Returns the localized display name of the component.
---@return string
function Component:getName() end

-- ─── Entity ───────────────────────────────────────────────────────────────────

---Scene entity that holds transform data and a list of components.
---@class Entity
---@field name string Display name of the entity.
---@field id userdata UUID userdata (use `tostring(entity.id)` for the string form).
---@field position number[] World-space position as {x, y, z}.
---@field rotation userdata CQuaternion rotation (raw userdata).
---@field areaId integer GTA interior area ID.
local Entity = {}

---Returns all components attached to this entity.
---@return Component[]
function Entity:getComponents() end

---Returns the first component matching the given type string, or nil.
---@param type string Component type identifier
---@return Component|nil
function Entity:getComponent(type) end

---Returns true if the entity has a component of the given type.
---@param type string Component type identifier
---@return boolean
function Entity:hasComponent(type) end

-- ─── ObjectComponent ─────────────────────────────────────────────────────────

---Object entity component. Wraps an in-game `CObject` instance.
---@class ObjectComponent : Component
---@field model integer GTA model id used to spawn the object.
---@field scale number Uniform scale applied via `CObject::m_fScale`. Defaults to 1.0. Negative/zero values are accepted.
local ObjectComponent = {}

---Returns the in-game object reference (`CPools::GetObjectRef`), or -1 if not spawned.
---@return integer
function ObjectComponent:getObjectRef() end

---Downcasts a generic Component to ObjectComponent. Returns nil if the component is not an Object.
---@param component Component
---@return ObjectComponent|nil
function ObjectComponent.cast(component) end
