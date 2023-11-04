#include "DamageObjectObjective.h"

#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "Object.h"
#include "strUtils.h"
#include "../Windows/PopupWeaponSelector.h"

DamageObjectObjective::DamageObjectObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.damage_object"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void DamageObjectObjective::draw(Localization &local) {
	ObjectObjective::draw(local);

	ImGui::Separator();

	ImGui::SliderInt(local.get("general.type").c_str(), &this->type_, 0, 1,
	                 local.getArray("damage_object_objective.types").at(this->type_).c_str());

	if (this->type_ == 1) {
		PopupWeaponSelector::getInstance().weaponButton(&this->weaponId_);
		PopupWeaponSelector::getInstance().renderPopup([this](const int weaponId) {
			this->weaponId_ = weaponId;
		});
	}
}

ktwait DamageObjectObjective::execute(Scene *scene, Object *object, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	if (this->type_ == 0) {
		while (object->getProjectObject().value()->m_nLastWeaponDamage == 255) {
			co_await 1;
		}
	} else {
		while (!Command<Commands::HAS_OBJECT_BEEN_DAMAGED_BY_WEAPON>(object->getProjectObject().value(),
		                                                             this->weaponId_)) {
			co_await 1;
		}
	}
}
