---@meta
---@class BaseObjective
BaseObjective = {
    ---~BaseObjective() -> void
    ---@return void
    ["~BaseObjective"] = function(self) end,
	---getUuid() -> boost::uuids::uuid
    ---@return boost::uuids::uuid
    ["getUuid"] = function(self) end,
	---getName() -> string
    ---@return string
    ["getName"] = function(self) end,
	---getCategory() -> int
    ---@return int
    ["getCategory"] = function(self) end,
	---getTypeCategory() -> int
    ---@return int
    ["getTypeCategory"] = function(self) end,
	---open() -> void
    ---@return void
    ["open"] = function(self) end,
	---close() -> void
    ---@return void
    ["close"] = function(self) end,
	---draw(local: Localization) -> void
    ---@return void
    ["draw"] = function(self, local) end,
	---execute(scene: Scene*, result: Result, tasklist: ktcoro_tasklist) -> ktwait
    ---@return ktwait
    ["execute"] = function(self, scene, result, tasklist) end
}

