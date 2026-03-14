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
---@field scale number[] Scale as {x, y, z}.
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
