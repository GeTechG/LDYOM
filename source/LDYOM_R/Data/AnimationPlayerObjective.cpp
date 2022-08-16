#include "AnimationPlayerObjective.h"

#include <CTimer.h>
#include <extensions/ScriptCommands.h>

#include "imgui.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"
#include "common.h"

AnimationPlayerObjective::AnimationPlayerObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.animation_player"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void AnimationPlayerObjective::draw(Localization& local) {
	using namespace plugin;

	bool play = false;
	const auto& packsNames = ModelsService::getInstance().getPacksNames();
	if (utils::Combo(local.get("animation.pack").c_str(), &this->pack_, packsNames)) {
		this->animation_ = 0;
		play = true;
	}


	play |= utils::Combo(local.get("animation.title").c_str(), &this->animation_, ModelsService::getInstance().getAnimations().at(packsNames[this->pack_]));
	utils::ToggleButton(local.get("animation.looped").c_str(), &this->looped_);
	ImGui::DragFloat(local.get("animation.smoothness").c_str(), &this->blend_, 0.01f, FLT_EPSILON, 1.f);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->time_, 1.0f, -1.0f, FLT_MAX);

	static auto lastTime = 0u;
	if (ImGui::IsKeyPressed(ImGuiKey_Y, true) && CTimer::m_snTimeInMilliseconds - lastTime > 500) {
		this->animation_++;
		if (this->animation_ >= ModelsService::getInstance().getAnimations().at(packsNames[this->pack_]).size()) {
			this->pack_++;
			this->animation_ %= ModelsService::getInstance().getAnimations().at(packsNames[this->pack_]).size();
		}
		if (this->pack_ >= packsNames.size()) {
			this->pack_ %= packsNames.size();
		}

		play = true;

		lastTime = CTimer::m_snTimeInMilliseconds;
	}
	else if (ImGui::IsKeyPressed(ImGuiKey_N, true) && CTimer::m_snTimeInMilliseconds - lastTime > 500) {
		this->animation_--;
		if (this->animation_ < 0) {
			this->pack_--;
			this->animation_ = ModelsService::getInstance().getAnimations().at(packsNames[this->pack_]).size() - 1;
		}
		if (this->pack_ < 0) {
			this->pack_ = packsNames.size() - 1;
		}

		play = true;

		lastTime = CTimer::m_snTimeInMilliseconds;
	}

	if (play) {
		if (!Command<Commands::HAS_ANIMATION_LOADED>(packsNames[this->pack_].c_str()))
			Command<Commands::REQUEST_ANIMATION>(packsNames[this->pack_].c_str());
		const auto& anims = ModelsService::getInstance().getAnimations().at(packsNames[this->pack_]);
		Command<Commands::TASK_PLAY_ANIM_NON_INTERRUPTABLE>(
			static_cast<CPed*>(FindPlayerPed()),
			anims[this->animation_].c_str(),
			packsNames[this->pack_].c_str(), 10.f * (1.f - (this->blend_ - FLT_EPSILON)),
			this->looped_, false, false, false, 0);
	}

	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text(local.get("animation.help").c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::End();
}

ktwait AnimationPlayerObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	using namespace plugin;

	const auto& packsNames = ModelsService::getInstance().getPacksNames();

	if (!Command<Commands::HAS_ANIMATION_LOADED>(packsNames[this->pack_].c_str()))
		Command<Commands::REQUEST_ANIMATION>(packsNames[this->pack_].c_str());
	const auto& anims = ModelsService::getInstance().getAnimations().at(packsNames[this->pack_]);
	Command<Commands::TASK_PLAY_ANIM_NON_INTERRUPTABLE>(
		static_cast<CPed*>(FindPlayerPed()),
		anims[this->animation_].c_str(),
		packsNames[this->pack_].c_str(), 10.f * (1.f - (this->blend_ - FLT_EPSILON)),
		this->looped_, false, false, false, static_cast<int>(this->time_ * 1000.f));

	co_return;
}

void AnimationPlayerObjective::close() {
	PlayerObjective::close();
	FindPlayerPed()->m_pIntelligence->ClearTasks(true, true);
}
