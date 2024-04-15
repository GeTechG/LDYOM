#include "DettachPedToEntity.h"

#include "imgui.h"
#include "ProjectsService.h"
#include "Settings.h"
#include "utils.h"
#include "utilsRender.h"

DettachPedToEntity::DettachPedToEntity(void *ptr) : BaseObjective(nullptr) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.dettach_ped_to_entity"));
	this->name += suffix;
}

void DettachPedToEntity::draw(Localization &local, std::vector<std::string> &listOverlay) {
	if (ImGui::SliderInt(local.get("dettach_ped_to_entity.dettach_type").c_str(), &this->dettachType, 0, 1,
	                     local.getArray("attach_ped_to_entity.attach_types")[this->dettachType].c_str())) {
		this->entityDettachUuid_ = {};
	}

	if (this->getDettachType() == 1) {
		const auto &actors = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getActors();
		const int indexActor = utils::indexByUuid(actors, this->entityDettachUuid_);

		IncorrectHighlight(indexActor == -1, [&] {
			utils::Combo(local.get("entities.actor").c_str(), &this->entityDettachUuid_, indexActor, actors.size(),
			             [&actors](const int i) {
				             return std::ref(actors.at(i)->getName());
			             }, [&actors](const int i) {
				             return actors.at(i)->getUuid();
			             });
		});

		if (Settings::getInstance().get<bool>("main.autoBindRequireFields").value_or(true)) {
			if (indexActor == -1) {
				if (!actors.empty()) {
					this->entityDettachUuid_ = actors.back()->getUuid();
				}
			}
		}
	}
}

ktwait DettachPedToEntity::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	CPed *dettachEntity = nullptr;

	if (this->dettachType == 0) {
		dettachEntity = FindPlayerPed();
	} else if (this->dettachType == 1) {
		const auto &actors = scene->getActors();
		const auto indexAttach = utils::indexByUuid(actors, this->entityDettachUuid_);
		if (indexAttach != -1) {
			dettachEntity = actors.at(indexAttach)->getProjectPed().value();
		}
	}

	if (!dettachEntity) {
		setObjectiveError(result, *this, NotSelected, "The entity for the objective is not selected.");
		co_return;
	}

	dettachEntity->DettachPedFromEntity();

	co_return;
}

boost::uuids::uuid& DettachPedToEntity::getEntityDettachUuid() { return entityDettachUuid_; }
int& DettachPedToEntity::getDettachType() { return dettachType; }
