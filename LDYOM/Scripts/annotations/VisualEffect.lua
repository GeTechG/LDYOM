---@meta
---@class VisualEffect
VisualEffect = {
    ---operator=(other: VisualEffect) -> VisualEffect
    ---@return VisualEffect
    ["operator="] = function(self, other) end,
	---~VisualEffect() -> void
    ---@return void
    ["~VisualEffect"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getType() -> int
    ---@return int
    ["getType"] = function(self) end,
	---getSize() -> float
    ---@return float
    ["getSize"] = function(self) end,
	---getEffectType() -> int
    ---@return int
    ["getEffectType"] = function(self) end,
	---getFlare() -> int
    ---@return int
    ["getFlare"] = function(self) end,
	---getColor() -> std::array<float>
    ---@return std::array<float>
    ["getColor"] = function(self) end,
	---isDrawing() -> bool
    ---@return bool
    ["isDrawing"] = function(self) end,
	---getAngle() -> float
    ---@return float
    ["getAngle"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getPosition() -> float
    ---@return float
    ["getPosition"] = function(self) end,
	---spawnEditorVisualEffect() -> void
    ---@return void
    ["spawnEditorVisualEffect"] = function(self) end,
	---deleteEditorVisualEffect() -> void
    ---@return void
    ["deleteEditorVisualEffect"] = function(self) end,
	---spawnProjectEntity() -> void
    ---@return void
    ["spawnProjectEntity"] = function(self) end,
	---deleteProjectEntity() -> void
    ---@return void
    ["deleteProjectEntity"] = function(self) end,
	---drawVisualEffect() -> void
    ---@return void
    ["drawVisualEffect"] = function(self) end
}

