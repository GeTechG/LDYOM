#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

#include "jsonUtils.h"
#include "Scene.h"
#include "Texture.h"
#include "boost/signals2/signal.hpp"


using json = nlohmann::json;

struct ProjectInfo {
	std::string name;
	std::string authorName = "UFO";
	int startScene = NULL;
	std::filesystem::path directory;
	std::optional<std::unique_ptr<Texture>> icon;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<ProjectInfo> {
		static void to_json(json &j, const ProjectInfo &p) {
			j = json{
				{"name", p.name},
				{"authorName", p.authorName},
				{"startScene", p.startScene},
			};
		}

		static void from_json(const json &j, ProjectInfo &p) {
			j.at("name").get_to(p.name);
			j.at("authorName").get_to(p.authorName);
			j.at("startScene").get_to(p.startScene);
		}
	};

NLOHMANN_JSON_NAMESPACE_END

class ProjectData {
private:
	std::unordered_map<int, std::unique_ptr<Scene>> scenes_;
	boost::signals2::signal<void()> onChangedScene_;
	std::unique_ptr<ProjectInfo> projectInfo_;

	int currentScene_{};

public:
	ProjectData();
	ProjectData& operator=(ProjectData &&other) noexcept;

	Scene* getCurrentScene();
	int& getCurrentSceneIndex();
	boost::signals2::signal<void()>& onChangedScene();

	void newScene(bool change = true);
	void changeScene(int scene);
	void deleteScene(int scene);

	std::unordered_map<int, std::unique_ptr<Scene>>& getScenes();
	std::unique_ptr<ProjectInfo>& getProjectInfo();
};
