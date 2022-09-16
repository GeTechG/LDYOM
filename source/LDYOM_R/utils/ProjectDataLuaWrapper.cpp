#include <sol.hpp>
#include "../Data/ProjectData.h"

void ProjectDataLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<ProjectInfo>("ProjectInfo", sol::constructors<ProjectInfo()>());
	clazz["name"] = &ProjectInfo::name;
	clazz["authorName"] = &ProjectInfo::authorName;
	clazz["startScene"] = &ProjectInfo::startScene;
	//clazz["directory"] = &ProjectInfo::directory;
	//clazz["icon"] = &ProjectInfo::icon;
	auto clazz2 = state.new_usertype<ProjectData>("ProjectData", sol::constructors<ProjectData()>());
	clazz2["getCurrentScene"] = &ProjectData::getCurrentScene;
	clazz2["getCurrentSceneIndex"] = &ProjectData::getCurrentSceneIndex;
	//clazz2["onChangedScene"] = &ProjectData::onChangedScene;
	clazz2["newScene"] = &ProjectData::newScene;
	clazz2["changeScene"] = &ProjectData::changeScene;
	clazz2["deleteScene"] = &ProjectData::deleteScene;
	//clazz2["getScenes"] = &ProjectData::getScenes;
	//clazz2["getProjectInfo"] = &ProjectData::getProjectInfo;
}
