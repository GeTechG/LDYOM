#include "QuickCommandsWindow.h"
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

void Windows::QuickCommandsWindow::draw() {
	const auto center = ImGui::GetIO().DisplaySize / ImVec2(2.0f, 2.0f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::Begin("test", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		
	}
	ImGui::End();
}
