---@meta
---@class Actor
Actor = {
    ---operator=(other: Actor) -> Actor
    ---@return Actor
    ["operator="] = function(self, other) end,
	---~Actor() -> void
    ---@return void
    ["~Actor"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---getEditorPed() -> std::optional<CPed>
    ---@return std::optional<CPed>
    ["getEditorPed"] = function(self) end,
	---getProjectPed() -> std::optional<CPed>
    ---@return std::optional<CPed>
    ["getProjectPed"] = function(self) end,
	---getHeadingAngle() -> float
    ---@return float
    ["getHeadingAngle"] = function(self) end,
	---getGroup() -> int
    ---@return int
    ["getGroup"] = function(self) end,
	---getModelId() -> int
    ---@return int
    ["getModelId"] = function(self) end,
	---getWeapons() -> std::vector<Weapon>
    ---@return std::vector<Weapon>
    ["getWeapons"] = function(self) end,
	---getDefaultWeapon() -> int
    ---@return int
    ["getDefaultWeapon"] = function(self) end,
	---getModelType() -> unsigned char
    ---@return unsigned char
    ["getModelType"] = function(self) end,
	---getSlot() -> int
    ---@return int
    ["getSlot"] = function(self) end,
	---getAccuracy() -> int
    ---@return int
    ["getAccuracy"] = function(self) end,
	---getHealth() -> int
    ---@return int
    ["getHealth"] = function(self) end,
	---isRandomSpawn() -> bool
    ---@return bool
    ["isRandomSpawn"] = function(self) end,
	---isShouldNotDie() -> bool
    ---@return bool
    ["isShouldNotDie"] = function(self) end,
	---isStayInSamePlace() -> bool
    ---@return bool
    ["isStayInSamePlace"] = function(self) end,
	---isKindaStayInSamePlace() -> bool
    ---@return bool
    ["isKindaStayInSamePlace"] = function(self) end,
	---isHeadshot() -> bool
    ---@return bool
    ["isHeadshot"] = function(self) end,
	---isDropWeapons() -> bool
    ---@return bool
    ["isDropWeapons"] = function(self) end,
	---isDressUp() -> bool
    ---@return bool
    ["isDressUp"] = function(self) end,
	---getClotherMAnModelKeys() -> std::array<unsigned>
    ---@return std::array<unsigned>
    ["getClotherMAnModelKeys"] = function(self) end,
	---getClotherMAnTextureKeys() -> std::array<unsigned>
    ---@return std::array<unsigned>
    ["getClotherMAnTextureKeys"] = function(self) end,
	---getFatStat() -> float
    ---@return float
    ["getFatStat"] = function(self) end,
	---getMusculeStat() -> float
    ---@return float
    ["getMusculeStat"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getSignalDeleteActor() -> boost::signals2::signal<void()>
    ---@return boost::signals2::signal<void()>
    ["getSignalDeleteActor"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---spawnEditorPed() -> void
    ---@return void
    ["spawnEditorPed"] = function(self) end,
	---deleteEditorPed() -> void
    ---@return void
    ["deleteEditorPed"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end
}

