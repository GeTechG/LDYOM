#include "CountdownObjective.h"

#include <CMessages.h>
#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_stdlib.h"
#include "strUtils.h"
#include "TimeUtils.h"

CountdownObjective::CountdownObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.countdown"));
	this->name += suffix;
}

int& CountdownObjective::getTime() { return time; }

std::string& CountdownObjective::getTextGo() { return textGo; }

void CountdownObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::InputInt(local.get("general.time").c_str(), &this->time);
	ImGui::InputText(local.get("general.text").c_str(), &this->textGo);
}

ktwait CountdownObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;
	using namespace std::chrono;

	Command<Commands::SET_PLAYER_CONTROL>(0, 0);

	auto cp1251Text = utf8ToCp1251(this->textGo);
	gxtEncode(cp1251Text);
	this->gameTextGo = cp1251Text;

	for (int i = this->time; i >= 0; i--) {
		char *text;
		auto str = std::to_string(i);
		if (i > 0) {
			text = str.data();
		} else {
			text = this->gameTextGo.data();
		}
		CMessages::AddBigMessageQ(text, 1000, 3);
		co_await waitInGame(1000);
	}

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);
}
