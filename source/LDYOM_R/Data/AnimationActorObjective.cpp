#include "AnimationActorObjective.h"

#include <CTimer.h>
#include <extensions/ScriptCommands.h>

#include "imgui.h"
#include "ModelsService.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "utils.h"
#include "../Windows/utilsRender.h"

AnimationActorObjective::AnimationActorObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.animation_actor"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void AnimationActorObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ActorObjective::draw(local, listOverlay);

	const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
	const int indexActor = utils::indexByUuid(actors, this->actorUuid_);

	using namespace plugin;

	const auto &packsNames = ModelsService::getInstance().getPacksNames();
	bool play = false;

	ImGui::BeginDisabled(indexActor == -1);
	if (utils::Combo(local.get("animation.pack").c_str(), &this->pack_, packsNames)) {
		this->animation_ = 0;
		play = true;
	}

	const auto animations = ModelsService::getInstance().getAnimations();
	if (utils::Combo(local.get("animation.title").c_str(), &this->animation_,
	                 animations.at(packsNames[this->pack_]))) {
		play |= true;
	}
	ImGui::EndDisabled();

	utils::ToggleButton(local.get("animation.looped").c_str(), &this->looped_);
	ImGui::DragFloat(local.get("animation.smoothness").c_str(), &this->blend_, 0.01f, FLT_EPSILON, 1.f);
	ImGui::DragFloat(local.get("general.time").c_str(), &this->time_, 1.0f, -1.0f, FLT_MAX);
	ImGui::Checkbox(local.get("general.wait_end").c_str(), &this->endWait_);

	static auto lastTime = 0u;
	if (ImGui::IsKeyPressed(ImGuiKey_Y, true) && CTimer::m_snTimeInMilliseconds - lastTime > 500) {
		this->animation_++;
		if (this->animation_ >= static_cast<int>(animations.at(packsNames[this->pack_]).size())) {
			this->pack_++;
			if (this->pack_ >= static_cast<int>(packsNames.size())) {
				this->pack_ %= static_cast<int>(packsNames.size());
			}
			this->animation_ %= static_cast<int>(animations.at(packsNames[this->pack_]).size());
		}

		play = true;

		lastTime = CTimer::m_snTimeInMilliseconds;
	} else if (ImGui::IsKeyPressed(ImGuiKey_N, true) && CTimer::m_snTimeInMilliseconds - lastTime > 500) {
		this->animation_--;
		if (this->animation_ < 0) {
			this->pack_--;
			if (this->pack_ < 0) {
				this->pack_ = static_cast<int>(packsNames.size()) - 1;
			}
			this->animation_ = static_cast<int>(animations.at(packsNames[this->pack_]).size()) - 1;
		}

		play = true;

		lastTime = CTimer::m_snTimeInMilliseconds;
	}

	if (play && indexActor != -1) {
		const auto &actor = actors.at(indexActor);
		if (!Command<Commands::HAS_ANIMATION_LOADED>(packsNames[this->pack_].c_str()))
			Command<Commands::REQUEST_ANIMATION>(packsNames[this->pack_].c_str());
		const auto &anims = animations.at(packsNames[this->pack_]);
		Command<Commands::TASK_PLAY_ANIM_NON_INTERRUPTABLE>(
			actor->getEditorPed().value(),
			anims[this->animation_].c_str(),
			packsNames[this->pack_].c_str(), 10.f * (1.f - (this->blend_ - FLT_EPSILON)),
			this->looped_, false, false, false, 0);
	}

	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##controlOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text(local.get("animation.help").c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::End();
}

ktwait AnimationActorObjective::execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	auto animationActorFunc = [](AnimationActorObjective *_this, Actor *actor) -> ktwait {
		const auto &packsNames = ModelsService::getInstance().getPacksNames();

		if (!Command<Commands::HAS_ANIMATION_LOADED>(packsNames[_this->pack_].c_str()))
			Command<Commands::REQUEST_ANIMATION>(packsNames[_this->pack_].c_str());
		const auto &anims = ModelsService::getInstance().getAnimations().at(packsNames[_this->pack_]);
		Command<Commands::TASK_PLAY_ANIM_NON_INTERRUPTABLE>(
			actor->getProjectPed().value(),
			anims[_this->animation_].c_str(),
			packsNames[_this->pack_].c_str(), 10.f * (1.f - (_this->blend_ - FLT_EPSILON)),
			_this->looped_, false, false, false, static_cast<int>(_this->time_ * 1000.f));

		while (Command<
			Commands::IS_CHAR_PLAYING_ANIM>(actor->getProjectPed().value(), anims[_this->animation_].c_str())) {
			co_await 1;
		}
	};

	if (this->endWait_)
		co_await animationActorFunc(this, actor);
	else
		tasklist.add_task(animationActorFunc, this, actor);
}

void AnimationActorObjective::close() {
	ActorObjective::close();
	const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
	const int indexActor = utils::indexByUuid(actors, this->actorUuid_);
	if (indexActor != -1)
		actors[indexActor]->getEditorPed().value()->m_pIntelligence->ClearTasks(true, true);
}
