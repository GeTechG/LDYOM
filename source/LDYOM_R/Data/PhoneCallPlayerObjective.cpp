#include "PhoneCallPlayerObjective.h"

#include <common.h>
#include <CStreaming.h>
#include <extensions/ScriptCommands.h>

#include "imgui.h"
#include "strUtils.h"
#include "utils.h"

PhoneCallPlayerObjective::PhoneCallPlayerObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.phone_call_player"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void PhoneCallPlayerObjective::draw(Localization& local) {

	if (ImGui::Button(local.get("general.add").c_str()))
		this->dialogs_.emplace_back();

	ImGui::SameLine();

	std::vector<std::string> indexes;
	for (int i = 0; i < this->dialogs_.size(); i++) {
		indexes.push_back(std::to_string(i));
	}
	ImGui::SetNextItemWidth(120);
	static int currentDialog = 0;
	utils::Combo("##indexes", &currentDialog, indexes);

	ImGui::SameLine();

	if (!this->dialogs_.empty()) {
		if (ImGui::Button(local.get("general.delete").c_str())) {
			this->dialogs_.erase(this->dialogs_.begin() + currentDialog);
			if (currentDialog > 0)
				currentDialog--;
		}
	}

	ImGui::BeginChild("##dialogs_list", ImVec2(ImGui::GetFontSize() * 20.f, ImGui::GetFontSize() * 24.f));
	for (int d = 0; d < this->dialogs_.size(); d++) {
		if (ImGui::TreeNode(std::to_string(d).c_str())) {
			auto& dialog = this->dialogs_[d];
			ImGui::InputText(local.get("general.text").c_str(), dialog.text.data(), dialog.text.size());
			ImGui::InputFloat(local.get("general.time").c_str(), &dialog.textTime);
			utils::ToggleButton(local.get("general.move_mouth").c_str(), &dialog.moveMouth);
			ImGui::TreePop();
		}
	}
	ImGui::EndChild();
}

ktwait PhoneCallPlayerObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	using namespace plugin;
	using namespace std::chrono;

	CStreaming::RequestModel(330, 0);
	CStreaming::LoadAllRequestedModels(false);

	Command<Commands::TASK_USE_MOBILE_PHONE>(static_cast<CPed*>(FindPlayerPed()), true);
	co_await 2.5s;

	for (auto dialog : this->dialogs_) {
		auto cp1251Text = utf8ToCp1251(dialog.text.data());
		gxtEncode(cp1251Text);
		strlcpy(dialog.gameText.data(), cp1251Text.c_str(), sizeof dialog.gameText);
		if (dialog.moveMouth) 
			Command<Commands::START_CHAR_FACIAL_TALK>(static_cast<CPed*>(FindPlayerPed()), -1);
		CMessages::AddMessageJumpQ(dialog.gameText.data(), static_cast<unsigned>(dialog.textTime * 1000.f), 0, false);
		co_await milliseconds(static_cast<long long>(dialog.textTime * 1000.f));
		Command<Commands::STOP_CHAR_FACIAL_TALK>(static_cast<CPed*>(FindPlayerPed()));
	}

	Command<Commands::TASK_USE_MOBILE_PHONE>(static_cast<CPed*>(FindPlayerPed()), false);
	co_await 2.5s;
}
