---@meta
---@class Pickup
Pickup = {
    ---operator=(other: Pickup) -> Pickup
    ---@return Pickup
    ["operator="] = function(self, other) end,
	---~Pickup() -> void
    ---@return void
    ["~Pickup"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorPickup() -> std::optional<CPickup>
    ---@return std::optional<CPickup>
    ["getEditorPickup"] = function(self) end,
	---getProjectPickup() -> std::optional<CPickup>
    ---@return std::optional<CPickup>
    ["getProjectPickup"] = function(self) end,
	---getModelId() -> int
    ---@return int
    ["getModelId"] = function(self) end,
	---getType() -> int
    ---@return int
    ["getType"] = function(self) end,
	---getSpawnType() -> int
    ---@return int
    ["getSpawnType"] = function(self) end,
	---getWeapon() -> int
    ---@return int
    ["getWeapon"] = function(self) end,
	---getAmmo() -> int
    ---@return int
    ["getAmmo"] = function(self) end,
	---getProjectPickupIndex() -> std::optional<int>
    ---@return std::optional<int>
    ["getProjectPickupIndex"] = function(self) end,
	---getEditorPickupIndex() -> std::optional<int>
    ---@return std::optional<int>
    ["getEditorPickupIndex"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---spawnEditorPickup() -> void
    ---@return void
    ["spawnEditorPickup"] = function(self) end,
	---deleteEditorPickup() -> void
    ---@return void
    ["deleteEditorPickup"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end
}

