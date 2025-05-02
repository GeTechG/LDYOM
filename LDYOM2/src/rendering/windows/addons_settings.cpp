#include "addons_settings.h"
#include "core/addons_manager.h"
#include "localization.h"
#include <fa_icons.h>
#include <imgui.h>
#include <string>

void AddonsSettings::refreshAddonsList() { addons = AddonsManager::instance().getAddons(); }

void AddonsSettings::renderContent(AddonsSettings* window) {

	if (ImGui::Button(_("addons_settings.refresh").c_str())) {
		window->refreshAddonsList();
	}

	ImGui::Spacing();

	auto& activeAddons = AddonsManager::instance().getActiveAddons();

	for (const auto& addon : window->addons) {
		ImGui::BeginGroup();

		ImGui::PushID(addon.id.c_str());

		bool enabled = activeAddons.find(addon.id) != activeAddons.end();
		if (ImGui::Checkbox("##enabled", &enabled)) {
			if (enabled) {
				AddonsManager::instance().enableAddon(addon.id);
			} else {
				AddonsManager::instance().disableAddon(addon.id);
			}
		}

		ImGui::SameLine();

		ImGui::BeginGroup();

		ImGui::Text(addon.name.c_str());
		ImGui::SameLine();
		ImGui::TextDisabled("v%s", addon.version.c_str());

		if (addon.author.has_value()) {
			ImGui::SameLine();
			ImGui::TextDisabled(_("addons_settings.by", addon.author.value()).c_str());
		}

		ImGui::TextWrapped(addon.description.c_str());

		ImGui::EndGroup();

		ImGui::PopID();
		ImGui::EndGroup();

		ImGui::Separator();
		ImGui::Spacing();
	}

	if (window->addons.empty()) {
		ImGui::Text(_("addons_settings.no_addons").c_str());
	}
}

AddonsSettings::AddonsSettings()
	: Window(_("addons_settings.title", ICON_FA_PUZZLE_PIECE)) {
	setSize(550, 400);
	setSizeMin(400, 300);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setRenderCallback<AddonsSettings>(AddonsSettings::renderContent);
	refreshAddonsList();
}