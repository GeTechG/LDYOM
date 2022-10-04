---@meta
---@class Pyrotechnics
Pyrotechnics = {
    ---operator=(other: Pyrotechnics) -> Pyrotechnics
    ---@return Pyrotechnics
    ["operator="] = function(self, other) end,
	---~Pyrotechnics() -> void
    ---@return void
    ["~Pyrotechnics"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorFire() -> std::optional<CFire>
    ---@return std::optional<CFire>
    ["getEditorFire"] = function(self) end,
	---getProjectFire() -> std::optional<CFire>
    ---@return std::optional<CFire>
    ["getProjectFire"] = function(self) end,
	---getType() -> int
    ---@return int
    ["getType"] = function(self) end,
	---getTypeExplosion() -> int
    ---@return int
    ["getTypeExplosion"] = function(self) end,
	---getSizeFire() -> int
    ---@return int
    ["getSizeFire"] = function(self) end,
	---getPropagationFire() -> int
    ---@return int
    ["getPropagationFire"] = function(self) end,
	---getProjectFireId() -> std::optional<int>
    ---@return std::optional<int>
    ["getProjectFireId"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---spawnEditorPyrotechnics() -> void
    ---@return void
    ["spawnEditorPyrotechnics"] = function(self) end,
	---deleteEditorPyrotechnics() -> void
    ---@return void
    ["deleteEditorPyrotechnics"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end
}

