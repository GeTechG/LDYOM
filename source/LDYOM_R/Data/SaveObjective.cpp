#include "SaveObjective.h"

#include <CWorld.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <extensions/ScriptCommands.h>
#include "imgui.h"
#include "ProjectsService.h"
#include "SaveService.h"
#include "strUtils.h"

#include "LuaEngine.h"
#include "ProjectPlayerService.h"
#include "WindowsRenderService.h"
#include "../Windows/SaveConfirmPopup.h"
#include "easylogging/easylogging++.h"

SaveObjective::SaveObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.save"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void SaveObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::Checkbox(local.get("save_objective.show_confirm").c_str(), &this->confirmSave_);
	ImGui::Dummy(ImVec2(200.f, 0.f));
}

ktwait SaveObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	bool end = false;
	saveGame(this->confirmSave_, -1, scene, tasklist, [&]() {
		end = true;
	});

	while (!end) {
		co_await 1;
	}
}

void SaveObjective::saveGame(bool confirm, int nodeSave, Scene *scene, ktcoro_tasklist &tasklist,
                             std::function<void()> callback) {
	tasklist.add_task(
		[](const bool confirm, const int nodeSave, Scene *scene, std::function<void()> callback) -> ktwait {
			auto saveGame = [&]() {
				auto &luaState = LuaEngine::getInstance().getLuaState();
				auto luaData = luaState.create_table();
				for (auto pairLua : luaState["global_data"]["signals"]["saveGame"].get_or_create<sol::table>()) {
					if (auto resultLua = pairLua.second.as<sol::function>()(scene->getId(), luaData); !resultLua.
						valid()) {
						sol::error err = resultLua;
						CLOG(ERROR, "lua") << err.what();
					}
				}
				const std::string luaDataStr = luaState["base64"]["encode"](luaState["bitser"]["dumps"](luaData));

				std::array<unsigned, 18> textures{};
				std::array<unsigned, 10> models{};
				const auto playerModel = CWorld::Players[0].m_pPed->m_nModelIndex;
				auto playerCoors = FindPlayerCoors(0);
				const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
				if (playerModel == 0) {
					std::memcpy(textures.data(), playerClothes->m_anTextureKeys, sizeof playerClothes->m_anTextureKeys);
					std::memcpy(models.data(), playerClothes->m_anModelKeys, sizeof playerClothes->m_anModelKeys);
				}

				SaveService::getInstance().addSave(
					ProjectsService::getInstance().getCurrentProject().getProjectInfo()->name,
					SaveData{
						scene->getId(),
						luaDataStr,
						to_string(ProjectPlayerService::getInstance().getCurrentObjective().value()->getUuid()),
						nodeSave,
						static_cast<int>(playerModel),
						{playerCoors.x, playerCoors.y, playerCoors.z},
						models,
						textures,
						playerClothes->m_fFatStat,
						playerClothes->m_fMuscleStat
					});
			};

			if (confirm) {
				using namespace plugin;

				const auto saveConfirmPopup = Windows::WindowsRenderService::getInstance().getWindow<
					Windows::SaveConfirmPopup>();
				Command<Commands::SET_PLAYER_CONTROL>(0, 0);

				Windows::WindowsRenderService::getInstance().addRender("openConfirmPopup", [&]() {
					saveConfirmPopup->open();
					ImGui::OpenPopup(Localization::getInstance().get("save_objective.show_confirm").c_str());
					Windows::WindowsRenderService::getInstance().removeRender("openConfirmPopup");
				});

				bool waitAnswer = false;

				auto onSave = [&]() {
					saveGame();
					waitAnswer = true;
				};

				auto onCancel = [&]() {
					waitAnswer = true;
				};

				const auto onSaveConnect = saveConfirmPopup->getOnSave().connect(onSave);
				const auto onCancelConnect = saveConfirmPopup->getOnCancel().connect(onCancel);

				while (!waitAnswer) {
					co_await 1;
				}

				Command<Commands::SET_PLAYER_CONTROL>(0, 1);
				saveConfirmPopup->close();
				onSaveConnect.disconnect();
				onCancelConnect.disconnect();
			} else {
				saveGame();
			}

			callback();
		}, confirm, nodeSave, scene, callback);
}
