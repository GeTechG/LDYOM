#include <sol.hpp>
#include "../Data/VisualEffect.h"


void visualEffectWrapper(sol::state &state) {
	state.new_usertype<VisualEffect>("LDVisualEffect",
	                                 sol::no_constructor,
	                                 "getUuid", &VisualEffect::getUuid,
	                                 "getType", &VisualEffect::getType,
	                                 "getSize", &VisualEffect::getSize,
	                                 "getEffectType", &VisualEffect::getEffectType,
	                                 "getFlare", &VisualEffect::getFlare,
	                                 "getColor", &VisualEffect::getColor,
	                                 "isDrawing", &VisualEffect::isDrawing,
	                                 "getAngle", &VisualEffect::getAngle,
	                                 "getName", &VisualEffect::getName,
	                                 "getPosition", [](VisualEffect &visualEffect) {
		                                 return std::make_tuple(visualEffect.getPosition()[0],
		                                                        visualEffect.getPosition()[1],
		                                                        visualEffect.getPosition()[2]);
	                                 },
	                                 "setPosition",
	                                 [](VisualEffect &visualEffect, const float x, const float y, const float z) {
		                                 visualEffect.getPosition()[0] = x;
		                                 visualEffect.getPosition()[1] = y;
		                                 visualEffect.getPosition()[2] = z;
	                                 },
	                                 "spawnEditorVisualEffect", &VisualEffect::spawnEditorVisualEffect,
	                                 "deleteEditorVisualEffect", &VisualEffect::deleteEditorVisualEffect,
	                                 "spawnProjectEntity", &VisualEffect::spawnProjectEntity,
	                                 "deleteProjectEntity", &VisualEffect::deleteProjectEntity,
	                                 "drawVisualEffect", &VisualEffect::drawVisualEffect
	);
}
