#include <sol.hpp>
#include "../Data/ProjectData.h"

void projectDataWrapper(sol::state &state) {
	state.new_usertype<ProjectData>("LDProjectData",
	                                sol::no_constructor,
	                                "getCurrentScene", &ProjectData::getCurrentScene,
	                                "getCurrentSceneIndex", &ProjectData::getCurrentSceneIndex,
	                                "onChangedScene", &ProjectData::onChangedScene,
	                                "newScene", &ProjectData::newScene,
	                                "changeScene", &ProjectData::changeScene,
	                                "deleteScene", &ProjectData::deleteScene,
	                                "getScenes", [](ProjectData &self) {
		                                std::map<int, Scene*> scenes;
		                                for (auto &[index, scene] : self.getScenes()) {
			                                scenes.insert({index, scene.get()});
		                                }
		                                return sol::as_table(scenes);
	                                },
	                                "getProjectInfo", [](ProjectData &self) {
		                                return self.getProjectInfo().get();
	                                }
	);
}
