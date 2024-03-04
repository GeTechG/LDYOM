#include <sol.hpp>
#include "../Data/ProjectData.h"

void projectInfoWrapper(sol::state &state) {
	auto projectInfoClass = state.new_usertype<ProjectInfo>("LDProjectInfo", sol::constructors<ProjectInfo()>());
	projectInfoClass["name"] = &ProjectInfo::name;
	projectInfoClass["authorName"] = &ProjectInfo::authorName;
	projectInfoClass["startScene"] = &ProjectInfo::startScene;
	projectInfoClass["directory"] = sol::property([](const ProjectInfo &projectInfo) {
		                                              return projectInfo.directory.string();
	                                              }, [](ProjectInfo &projectInfo, const char *directory) {
		                                              projectInfo.directory = directory;
	                                              });
}
