#pragma once
#include "core/addons_manager.h"
#include "window.h"
#include <vector>

class AddonsSettings : public Window {
  private:
	static void renderContent(AddonsSettings* window);
	std::vector<AddonMetadata> addons;

  public:
	AddonsSettings();
	~AddonsSettings() override = default;

	void refreshAddonsList();
};