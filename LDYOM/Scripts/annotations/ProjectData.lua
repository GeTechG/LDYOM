---@meta
---@class ProjectInfo
ProjectInfo = {
    ---char
    ["name"] = nil,
	---char
    ["authorName"] = nil,
	---int
    ["startScene"] = nil,
	---std::filesystem::path
    ["directory"] = nil,
	---std::optional<std::unique_ptr<Texture>>
    ["icon"] = nil
}

---@class ProjectData
ProjectData = {
    ---operator=(other: ProjectData) -> ProjectData
    ---@return ProjectData
    ["operator="] = function(self, other) end,
	---getCurrentScene() -> Scene
    ---@return Scene
    ["getCurrentScene"] = function(self) end,
	---getCurrentSceneIndex() -> int
    ---@return int
    ["getCurrentSceneIndex"] = function(self) end,
	---onChangedScene() -> boost::signals2::signal<void()>
    ---@return boost::signals2::signal<void()>
    ["onChangedScene"] = function(self) end,
	---newScene(change: bool) -> void
    ---@return void
    ["newScene"] = function(self, change) end,
	---changeScene(scene: int) -> void
    ---@return void
    ["changeScene"] = function(self, scene) end,
	---deleteScene(scene: int) -> void
    ---@return void
    ["deleteScene"] = function(self, scene) end,
	---getScenes() -> std::unordered_map<int,std::unique_ptr<Scene>>
    ---@return std::unordered_map<int,std::unique_ptr<Scene>>
    ["getScenes"] = function(self) end,
	---getProjectInfo() -> std::unique_ptr<ProjectInfo>
    ---@return std::unique_ptr<ProjectInfo>
    ["getProjectInfo"] = function(self) end
}

