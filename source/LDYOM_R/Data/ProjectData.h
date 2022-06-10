#pragma once

#include <filesystem>

#include "Scene.h"
#include "Texture.h"
#include "boost/signals2/signal.hpp"
#include "boost/archive/binary_oarchive.hpp"

struct ProjectInfo {
	char name[NAME_SIZE] = "";
	char authorName[NAME_SIZE] = "UFO";
	int startScene = NULL;
	std::filesystem::path directory;
	std::optional<std::unique_ptr<Texture>> icon;
};

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive& ar, ProjectInfo& p, const unsigned int version)
		{
			ar & p.name;
			ar & p.authorName;
			ar & p.startScene;
		}

	}
}

class ProjectData {
private:
	std::unordered_map<int, std::unique_ptr<Scene>> scenes_;
	boost::signals2::signal<void()> onChangedScene_;
	std::unique_ptr<ProjectInfo> projectInfo_;

	int currentScene_{};

public:
	ProjectData();
	ProjectData& operator=(ProjectData&& other) noexcept;

	Scene* getCurrentScene();
	int& getCurrentSceneIndex();
	boost::signals2::signal<void()>& onChangedScene();

	void newScene(bool change = true);
	void changeScene(int scene);
	void deleteScene(int scene);

	std::unordered_map<int, std::unique_ptr<Scene>>& getScenes();
	std::unique_ptr<ProjectInfo>& getProjectInfo();
};
