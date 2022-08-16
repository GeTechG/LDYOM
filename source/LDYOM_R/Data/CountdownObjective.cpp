#include "CountdownObjective.h"

#include <CMessages.h>
#include <extensions/ScriptCommands.h>

#include "imgui.h"
#include "strUtils.h"

CountdownObjective::CountdownObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.countdown"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void CountdownObjective::draw(Localization& local) {
	ImGui::InputInt(local.get("general.time").c_str(), &this->time_);
	ImGui::InputText(local.get("general.text").c_str(), this->textGo_.data(), sizeof this->textGo_);
}

ktwait CountdownObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	using namespace plugin;
	using namespace std::chrono;

	Command<Commands::SET_PLAYER_CONTROL>(0, 0);

	auto cp1251Text = utf8ToCp1251(this->textGo_.data());
	gxtEncode(cp1251Text);
	strlcpy(this->gameTextGo_.data(), cp1251Text.c_str(), sizeof this->gameTextGo_);

	for (int i = this->time_; i >= 0; i--) {
		char* text;
		auto str = std::to_string(i);
		if (i > 0) {
			text = str.data();
		} else {
			text = this->gameTextGo_.data();
		}
		CMessages::AddBigMessageQ(text, 1000, 3);
		co_await 1s;
	}

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);
}
