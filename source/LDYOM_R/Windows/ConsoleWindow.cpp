#include "ConsoleWindow.h"

#include "fa.h"
#include "imgui.h"
#include "LuaEngine.h"
#include "LuaLogger.h"
#include "Localization/Localization.h"
#include <fmt/core.h>

#include "easylogging/easylogging++.h"

void Windows::ConsoleWindow::draw() {
	auto& local = Localization::getInstance();
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * 24.f, ImGui::GetFontSize() * 32.f), ImGuiCond_FirstUseEver);
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_SCROLL, local.get("console_window.title")).c_str())) {

		if (ImGui::BeginChild("##consoleLogs", ImVec2(0.f, -104.f), true)) {
			for (auto&& item : LuaLogger::getInstance().getItems()) {
				ImGui::TextUnformatted(item.c_str());
			}
		}
		ImGui::EndChild();

		static char code[1024];

		ImGui::InputTextMultiline("##codeInput", code, std::size(code), ImVec2(-ImGui::GetFontSize() * 5.f + 4.f, 100.f));
		ImGui::SameLine();
		if (ImGui::Button(local.get("console_window.run").c_str(), ImVec2(ImGui::GetFontSize() * 5.f, 0.f))) {
			if (std::strcmp(code, "clear") == 0) {
				LuaLogger::getInstance().getItems().clear();
			} else {
				LuaLogger::getInstance().getItems().emplace_back(code);
				LuaEngine::getInstance().getLuaState().safe_script(code, [](lua_State*, sol::protected_function_result pfr) {
					const sol::error err = pfr;
					LuaLogger::getInstance().getItems().emplace_back(err.what());
					CLOG(ERROR, "lua") << err.what();
					return pfr;
					});
			}
			code[0] = '\0';
		}

		ImGui::End();
	}
}
