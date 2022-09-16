---@meta
---@class ProjectsService
ProjectsService = {
    ---getInstance() -> ProjectsService
    ---@return ProjectsService
    ["getInstance"] = function() end,
	---Init() -> void
    ---@return void
    ["Init"] = function(self) end,
	---update() -> void
    ---@return void
    ["update"] = function(self) end,
	---Reset() -> void
    ---@return void
    ["Reset"] = function(self) end,
	---createNewProject() -> void
    ---@return void
    ["createNewProject"] = function(self) end,
	---saveCurrentProject() -> void
    ---@return void
    ["saveCurrentProject"] = function(self) end,
	---loadProject(projectIdx: int) -> void
    ---@return void
    ["loadProject"] = function(self, projectIdx) end,
	---deleteProject(projectIdx: int) -> void
    ---@return void
    ["deleteProject"] = function(self, projectIdx) end,
	---getProjectsInfos() -> std::vector<std::unique_ptr<ProjectInfo>>
    ---@return std::vector<std::unique_ptr<ProjectInfo>>
    ["getProjectsInfos"] = function(self) end,
	---onUpdate() -> boost::signals2::signal<void()>
    ---@return boost::signals2::signal<void()>
    ["onUpdate"] = function(self) end,
	---getCurrentProject() -> ProjectData
    ---@return ProjectData
    ["getCurrentProject"] = function(self) end
}

