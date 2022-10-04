#include <sol.hpp>
#include "../Data/VisualEffect.h"

void VisualEffectLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<VisualEffect>("VisualEffect");
	clazz["getUuid"] = &VisualEffect::getUuid;
	clazz["getType"] = &VisualEffect::getType;
	clazz["getSize"] = &VisualEffect::getSize;
	clazz["getEffectType"] = &VisualEffect::getEffectType;
	clazz["getFlare"] = &VisualEffect::getFlare;
	clazz["getColor"] = &VisualEffect::getColor;
	clazz["isDrawing"] = &VisualEffect::isDrawing;
	clazz["getAngle"] = &VisualEffect::getAngle;
	clazz["getName"] = &VisualEffect::getName;
	clazz["getPosition"] = &VisualEffect::getPosition;
	clazz["spawnEditorVisualEffect"] = &VisualEffect::spawnEditorVisualEffect;
	clazz["deleteEditorVisualEffect"] = &VisualEffect::deleteEditorVisualEffect;
	clazz["spawnProjectEntity"] = &VisualEffect::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &VisualEffect::deleteProjectEntity;
	clazz["drawVisualEffect"] = &VisualEffect::drawVisualEffect;
}
