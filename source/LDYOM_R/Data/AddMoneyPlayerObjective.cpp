#include "AddMoneyPlayerObjective.h"

#include <CWorld.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "strUtils.h"

AddMoneyPlayerObjective::AddMoneyPlayerObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.add_money_player"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void AddMoneyPlayerObjective::draw(Localization &local) {
	ImGui::DragInt(local.get("general.add").c_str(), &this->money_, 100, 0, 0, "%d $");
}

ktwait AddMoneyPlayerObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	CWorld::Players[0].m_nMoney += this->money_;
	co_return;
}
