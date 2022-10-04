---@meta
---@class Audio
Audio = {
    ---operator=(other: Audio) -> Audio
    ---@return Audio
    ["operator="] = function(self, other) end,
	---~Audio() -> void
    ---@return void
    ["~Audio"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorAudio() -> std::optional<int>
    ---@return std::optional<int>
    ["getEditorAudio"] = function(self) end,
	---getProjectAudio() -> std::optional<int>
    ---@return std::optional<int>
    ["getProjectAudio"] = function(self) end,
	---getAudioName() -> std::string
    ---@return std::string
    ["getAudioName"] = function(self) end,
	---isAudio3D() -> bool
    ---@return bool
    ["isAudio3D"] = function(self) end,
	---getAttachType3d() -> int
    ---@return int
    ["getAttachType3d"] = function(self) end,
	---getAttachUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getAttachUuid"] = function(self) end,
	---isIsLooped() -> bool
    ---@return bool
    ["isIsLooped"] = function(self) end,
	---getVolume() -> float
    ---@return float
    ["getVolume"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---spawnEditorAudio() -> void
    ---@return void
    ["spawnEditorAudio"] = function(self) end,
	---deleteEditorAudio() -> void
    ---@return void
    ["deleteEditorAudio"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end,
	---preloadProjectAudio() -> void
    ---@return void
    ["preloadProjectAudio"] = function(self) end,
	---getAudioFilesList() -> std::vector<std::string>
    ---@return std::vector<std::string>
    ["getAudioFilesList"] = function() end,
	---loadAudioFilesList() -> void
    ---@return void
    ["loadAudioFilesList"] = function() end
}

