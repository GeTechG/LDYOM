#include "SaveObjective.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <boost/lexical_cast.hpp>
#include <extensions/ScriptCommands.h>
#include "imgui.h"
#include "ProjectsService.h"

#include "ProjectPlayerService.h"

SaveObjective::SaveObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.save"));
	this->name += suffix;
}

void SaveObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::Checkbox(local.get("save_objective.show_confirm").c_str(), &this->confirmSave_);
	ImGui::Dummy(ImVec2(200.f, 0.f));
}

ktwait SaveObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	//TODO : implement
	co_return;
}
