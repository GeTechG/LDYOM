---@meta
---@class Checkpoint
Checkpoint = {
    ---operator=(other: Checkpoint) -> Checkpoint
    ---@return Checkpoint
    ["operator="] = function(self, other) end,
	---~Checkpoint() -> void
    ---@return void
    ["~Checkpoint"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorCheckpoint() -> std::optional<CCheckpoint>
    ---@return std::optional<CCheckpoint>
    ["getEditorCheckpoint"] = function(self) end,
	---getProjectCheckpoint() -> std::optional<CCheckpoint>
    ---@return std::optional<CCheckpoint>
    ["getProjectCheckpoint"] = function(self) end,
	---getEditorSphere() -> std::optional<unsigned>
    ---@return std::optional<unsigned>
    ["getEditorSphere"] = function(self) end,
	---getProjectSphere() -> std::optional<unsigned>
    ---@return std::optional<unsigned>
    ["getProjectSphere"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---getType() -> int
    ---@return int
    ["getType"] = function(self) end,
	---getRadius() -> float
    ---@return float
    ["getRadius"] = function(self) end,
	---getBlipColor() -> int
    ---@return int
    ["getBlipColor"] = function(self) end,
	---getColor() -> std::array<float>
    ---@return std::array<float>
    ["getColor"] = function(self) end,
	---getCheckpointType() -> int
    ---@return int
    ["getCheckpointType"] = function(self) end,
	---getPulsePeriod() -> int
    ---@return int
    ["getPulsePeriod"] = function(self) end,
	---getPulseFraction() -> float
    ---@return float
    ["getPulseFraction"] = function(self) end,
	---getRotateRate() -> int
    ---@return int
    ["getRotateRate"] = function(self) end,
	---getBlipType() -> int
    ---@return int
    ["getBlipType"] = function(self) end,
	---getBlipSprite() -> int
    ---@return int
    ["getBlipSprite"] = function(self) end,
	---getAngle() -> float
    ---@return float
    ["getAngle"] = function(self) end,
	---spawnEditorCheckpoint() -> void
    ---@return void
    ["spawnEditorCheckpoint"] = function(self) end,
	---deleteEditorCheckpoint() -> void
    ---@return void
    ["deleteEditorCheckpoint"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end,
	---spawnEditorBlip() -> void
    ---@return void
    ["spawnEditorBlip"] = function(self) end,
	---deleteEditorBlip() -> void
    ---@return void
    ["deleteEditorBlip"] = function(self) end,
	---spawnProjectBlip() -> void
    ---@return void
    ["spawnProjectBlip"] = function(self) end,
	---deleteProjectBlip() -> void
    ---@return void
    ["deleteProjectBlip"] = function(self) end,
	---drawSphere() -> void
    ---@return void
    ["drawSphere"] = function(self) end,
	---existProjectEntity() -> bool
    ---@return bool
    ["existProjectEntity"] = function(self) end,
	---getProjectEntityPosition() -> CVector
    ---@return CVector
    ["getProjectEntityPosition"] = function(self) end
}

