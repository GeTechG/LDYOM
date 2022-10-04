---@meta
---@class Particle
Particle = {
    ---operator=(other: Particle) -> Particle
    ---@return Particle
    ["operator="] = function(self, other) end,
	---~Particle() -> void
    ---@return void
    ["~Particle"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getEditorParticle() -> std::optional<FxSystem_c>
    ---@return std::optional<FxSystem_c>
    ["getEditorParticle"] = function(self) end,
	---getProjectParticle() -> std::optional<FxSystem_c>
    ---@return std::optional<FxSystem_c>
    ["getProjectParticle"] = function(self) end,
	---getRotations() -> CQuaternion
    ---@return CQuaternion
    ["getRotations"] = function(self) end,
	---getScale() -> std::array<float>
    ---@return std::array<float>
    ["getScale"] = function(self) end,
	---getParticleType() -> int
    ---@return int
    ["getParticleType"] = function(self) end,
	---getAttachType() -> int
    ---@return int
    ["getAttachType"] = function(self) end,
	---getAttachUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getAttachUuid"] = function(self) end,
	---getPedBodeId() -> int
    ---@return int
    ["getPedBodeId"] = function(self) end,
	---getProjectParticleId() -> std::optional<int>
    ---@return std::optional<int>
    ["getProjectParticleId"] = function(self) end,
	---updateLocation() -> void
    ---@return void
    ["updateLocation"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---spawnEditorParticle() -> void
    ---@return void
    ["spawnEditorParticle"] = function(self) end,
	---deleteEditorParticle() -> void
    ---@return void
    ["deleteEditorParticle"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end
}

