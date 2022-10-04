---@meta
---@class Train
Train = {
    ---operator=(other: Train) -> Train
    ---@return Train
    ["operator="] = function(self, other) end,
	---~Train() -> void
    ---@return void
    ["~Train"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorTrain() -> std::optional<CTrain>
    ---@return std::optional<CTrain>
    ["getEditorTrain"] = function(self) end,
	---getProjectTrain() -> std::optional<CTrain>
    ---@return std::optional<CTrain>
    ["getProjectTrain"] = function(self) end,
	---isRotate() -> bool
    ---@return bool
    ["isRotate"] = function(self) end,
	---getHealth() -> int
    ---@return int
    ["getHealth"] = function(self) end,
	---getTrainType() -> int
    ---@return int
    ["getTrainType"] = function(self) end,
	---getSpeed() -> float
    ---@return float
    ["getSpeed"] = function(self) end,
	---getCruiseSpeed() -> float
    ---@return float
    ["getCruiseSpeed"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---spawnEditorTrain() -> void
    ---@return void
    ["spawnEditorTrain"] = function(self) end,
	---deleteEditorTrain() -> void
    ---@return void
    ["deleteEditorTrain"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end
}

