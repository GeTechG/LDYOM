#include "fires_remover.h"
#include <CFire.h>
#include <CFireManager.h>
#include <CTheScripts.h>
#include <components/firework.h>
#include <entities_manager.h>
#include <project_player.h>
#include <set>


void checkEditorFires() {
	if (!IS_PLAYING) {
		std::set<CFire*> editorFires;
		auto fireworks = EntitiesManager::instance().getEntitiesWithComponent(components::Firework::TYPE);
		for (const auto& firework : fireworks) {
			auto fireworkComponent = components::Firework::cast(firework->getComponent(components::Firework::TYPE));
			if (fireworkComponent && fireworkComponent->fire) {
				const int actualScriptThingIndex = CTheScripts::GetActualScriptThingIndex(*fireworkComponent->fire, 5);
				auto& fire = gFireManager.m_aFires[actualScriptThingIndex];
				editorFires.insert(&fire);
			}
		}
		for (auto& fire : gFireManager.m_aFires) {
			if (fire.m_nFlags.bActive != 0 && !editorFires.contains(&fire)) {
				fire.Extinguish();
			}
		}
	}
}