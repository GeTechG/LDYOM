---@meta
---@class Object
Object = {
    ---operator=(other: Object) -> Object
    ---@return Object
    ["operator="] = function(self, other) end,
	---~Object() -> void
    ---@return void
    ["~Object"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorObject() -> std::optional<CObject>
    ---@return std::optional<CObject>
    ["getEditorObject"] = function(self) end,
	---getProjectObject() -> std::optional<CObject>
    ---@return std::optional<CObject>
    ["getProjectObject"] = function(self) end,
	---getRotations() -> CQuaternion
    ---@return CQuaternion
    ["getRotations"] = function(self) end,
	---getScale() -> std::array<float>
    ---@return std::array<float>
    ["getScale"] = function(self) end,
	---getModelId() -> int
    ---@return int
    ["getModelId"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---spawnEditorObject() -> void
    ---@return void
    ["spawnEditorObject"] = function(self) end,
	---deleteEditorObject() -> void
    ---@return void
    ["deleteEditorObject"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end
}

