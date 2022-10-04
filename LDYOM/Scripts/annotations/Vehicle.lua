---@meta
---@class Vehicle
Vehicle = {
    ---operator=(other: Vehicle) -> Vehicle
    ---@return Vehicle
    ["operator="] = function(self, other) end,
	---~Vehicle() -> void
    ---@return void
    ["~Vehicle"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorVehicle() -> std::optional<CVehicle>
    ---@return std::optional<CVehicle>
    ["getEditorVehicle"] = function(self) end,
	---getProjectVehicle() -> std::optional<CVehicle>
    ---@return std::optional<CVehicle>
    ["getProjectVehicle"] = function(self) end,
	---getModelId() -> int
    ---@return int
    ["getModelId"] = function(self) end,
	---isShouldNotDie() -> bool
    ---@return bool
    ["isShouldNotDie"] = function(self) end,
	---getHealth() -> int
    ---@return int
    ["getHealth"] = function(self) end,
	---isBulletproof() -> bool
    ---@return bool
    ["isBulletproof"] = function(self) end,
	---isFireproof() -> bool
    ---@return bool
    ["isFireproof"] = function(self) end,
	---isExplosionproof() -> bool
    ---@return bool
    ["isExplosionproof"] = function(self) end,
	---isCollisionproof() -> bool
    ---@return bool
    ["isCollisionproof"] = function(self) end,
	---isMeleeproof() -> bool
    ---@return bool
    ["isMeleeproof"] = function(self) end,
	---isTiresVulnerability() -> bool
    ---@return bool
    ["isTiresVulnerability"] = function(self) end,
	---isExtendedColor() -> bool
    ---@return bool
    ["isExtendedColor"] = function(self) end,
	---isLocked() -> bool
    ---@return bool
    ["isLocked"] = function(self) end,
	---getColors() -> std::vector<std::pair<unsigned char,std::array<float>>>
    ---@return std::vector<std::pair<unsigned char,std::array<float>>>
    ["getColors"] = function(self) end,
	---getComponentTypeA() -> int
    ---@return int
    ["getComponentTypeA"] = function(self) end,
	---getComponentTypeB() -> int
    ---@return int
    ["getComponentTypeB"] = function(self) end,
	---getNumberplateCity() -> int
    ---@return int
    ["getNumberplateCity"] = function(self) end,
	---getPrimaryColor() -> float
    ---@return float
    ["getPrimaryColor"] = function(self) end,
	---getSecondaryColor() -> float
    ---@return float
    ["getSecondaryColor"] = function(self) end,
	---getNumberplate() -> string
    ---@return string
    ["getNumberplate"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---getHeadingAngle() -> float
    ---@return float
    ["getHeadingAngle"] = function(self) end,
	---setEditorPrimaryColor() -> void
    ---@return void
    ["setEditorPrimaryColor"] = function(self) end,
	---setEditorSecondaryColor() -> void
    ---@return void
    ["setEditorSecondaryColor"] = function(self) end,
	---spawnEditorVehicle(recolor: bool) -> void
    ---@return void
    ["spawnEditorVehicle"] = function(self, recolor) end,
	---deleteEditorVehicle() -> void
    ---@return void
    ["deleteEditorVehicle"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end
}

