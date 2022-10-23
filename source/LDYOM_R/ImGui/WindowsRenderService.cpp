// ReSharper disable CppMemberFunctionMayBeStatic
#include "WindowsRenderService.h"
#include "plugin.h"
#include "imgui.h"
#include "imguistyleloader.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Logger.h"
#include "Settings.h"
#include "../Windows/AbstractWindow.h"
#include "../utils/fa.h"
#include <CMenuManager.h>
#include <fmt/core.h>

#include "imnodes/imnodes.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

extern unsigned int fa_compressed_size;
extern unsigned int fa_compressed_data[];
extern unsigned int fira_regular_compressed_size;
extern unsigned int fira_regular_compressed_data[];

#define DIMOUSE ((LPDIRECTINPUTDEVICE8)(RsGlobal.ps->diMouse))
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC oWndProc;


typedef HRESULT(WINAPI* _EndScene)(IDirect3DDevice9*);
_EndScene oEndScene;

typedef HRESULT(WINAPI* _Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
_Reset oReset;

bool isInitImgui = false;
float uiScaling = 1.0f;
bool reloadTheme = false;

void Windows::WindowsRenderService::render() const {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto currRenderList = renderList_;
	for (const auto& [name, function] : currRenderList)
		function();

	if (renderWindows_) {
		for (const auto& window : this->windows_) {
			if (window->isShow()) {
				window->draw();
			}
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

bool& Windows::WindowsRenderService::isRenderWindows() {
	return renderWindows_;
}

void Windows::WindowsRenderService::setRenderWindows(const bool renderWindows) {
	renderWindows_ = renderWindows;
}

void Windows::WindowsRenderService::shutdown() const {
	ImNodes::DestroyContext();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	vmtHooks_->UnhookAll();
}

void Windows::WindowsRenderService::mouseProcess() const {

	static bool mouseState = false;
	if (mouseState != (mouseShown_ && renderWindows_))
	{
		ImGui::GetIO().MouseDrawCursor = mouseShown_ && renderWindows_;

		if (mouseShown_ && renderWindows_ && !FrontEndMenuManager.m_bMenuActive)
		{

			plugin::patch::SetUChar(0x6194A0, 0xC3); // psSetMousePos
			plugin::patch::Nop(0x541DD7, 5); // don't call CPad::UpdateMouse()
		}
		else
		{

			plugin::patch::SetUChar(0x6194A0, 0xE9);
			plugin::patch::SetRaw(0x541DD7, const_cast<char*>("\xE8\xE4\xD5\xFF\xFF"), 5);
		}

		CPad::NewMouseControllerState.x = 0;
		CPad::NewMouseControllerState.y = 0;
		CPad::ClearMouseHistory();
		CPad::UpdatePads();
		mouseState = mouseShown_ && renderWindows_;
	}
}


void Windows::WindowsRenderService::process() const {
	mouseProcess();
}

void loadTheme() {
	if (!ImGui::StyleLoader::LoadFile(fmt::format("LDYOM/Themes/{}.toml", Settings::getInstance().get<std::string>("main.theme").value()))) {
		Logger::getInstance().log("invalid load theme!");
	}
	ImNodes::GetStyle().Colors[ImNodesCol_GridBackground] = ImGui::GetColorU32(ImGuiCol_WindowBg);
	ImNodes::GetStyle().Colors[ImNodesCol_GridLine] = ImGui::GetColorU32(ImGuiCol_Border);
}

void Windows::WindowsRenderService::style() {
	// Setup Dear ImGui style
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiStyle styleold = style; // Backup colors
	style = ImGuiStyle(); // IMPORTANT: ScaleAllSizes will change the original size, so we should reset all style config
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.TabBorderSize = 1.0f;
	style.WindowRounding = 0.0f;
	style.ChildRounding = 0.0f;
	style.PopupRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.ScaleAllSizes(uiScaling);
	CopyMemory(style.Colors, styleold.Colors, sizeof(style.Colors)); // Restore colors

	const auto allocate_fira_regular = new unsigned int[fira_regular_compressed_size / 4];
	memcpy(allocate_fira_regular, fira_regular_compressed_data, fira_regular_compressed_size);

	static const ImWchar lang_ranges[] = { 0x0020,0xFFFF,0 };
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(allocate_fira_regular, static_cast<int>(fira_regular_compressed_size), 16.0f * uiScaling, nullptr, lang_ranges);

	const auto allocate_fa = new unsigned int[fa_compressed_size / 4];
	memcpy(allocate_fa, fa_compressed_data, fa_compressed_size);

	// Грузим иконочный шрифт Font Awesome 5
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(allocate_fa, static_cast<int>(fa_compressed_size), 16.0f * uiScaling, &icons_config, icons_ranges);

	ImGui::GetIO().Fonts->Build();

	loadTheme();

	ImNodes::GetStyle().PinCircleRadius = 16.0f * uiScaling / 3.f;
	ImNodes::GetStyle().PinQuadSideLength = 16.0f * uiScaling / 1.5f;
	ImNodes::GetStyle().PinTriangleSideLength = 16.0f * uiScaling / 1.5f;
	ImNodes::GetStyle().PinLineThickness = 3.f * uiScaling;
	ImNodes::GetStyle().NodePadding = ImVec2(10.f, 10.f) * ImVec2(uiScaling, uiScaling);
	ImNodes::GetStyle().Flags |= ImNodesStyleFlags_GridSnapping;
	ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
}

LRESULT wndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	if (ImGui::GetIO().WantTextInput)
	{
#ifdef GTASA
		plugin::Call<0x53F1E0>(); // CPad::ClearKeyboardHistory
#endif
		return 1;
	}

#ifndef USER_DEFAULT_SCREEN_DPI
#define USER_DEFAULT_SCREEN_DPI 96
#endif
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif

	switch (uMsg) {
	case WM_DPICHANGED:
		RECT* rect = (RECT*)lParam;
		IM_ASSERT(LOWORD(wParam) == HIWORD(wParam));
		::SetWindowPos(hWnd, NULL, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_NOZORDER);
		uiScaling = static_cast<float>(LOWORD(wParam)) / static_cast<float>(USER_DEFAULT_SCREEN_DPI);
		Windows::WindowsRenderService::getInstance().style();
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT WINAPI Reset(IDirect3DDevice9* m_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	return oReset(m_pDevice, pPresentationParameters);
}

void initImGui() {
	ImGui_ImplWin32_EnableDpiAwareness();

	/// Инициализируем ImGui
	if (!ImGui::GetCurrentContext())
	{
		ImGui::CreateContext();
	}
	ImNodes::CreateContext();

	ImGui_ImplWin32_Init(RsGlobal.ps->window);
	ImGui::GetIO().MouseDoubleClickTime = 0.8f;

	uiScaling = max(ImGui_ImplWin32_GetDpiScaleForHwnd(RsGlobal.ps->window), 1.0f);

	plugin::patch::Nop(0x00531155, 5); // shift trigger fix

	ImGui_ImplDX9_Init(GetD3DDevice());

	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(RsGlobal.ps->window, GWL_WNDPROC, reinterpret_cast<LRESULT>(wndProc)));

	Windows::WindowsRenderService::getInstance().style();

	isInitImgui = true;
}

HRESULT WINAPI EndScene(IDirect3DDevice9* pDevice) {
	if (!isInitImgui)
		initImGui();
	if (reloadTheme) {
		loadTheme();
		reloadTheme = false;
	}
	if (!FrontEndMenuManager.m_bMenuActive)
		Windows::WindowsRenderService::getInstance().render();
	return oEndScene(pDevice);
}

void Windows::WindowsRenderService::Init() {
	void** vTableDevice = *(void***)(*(DWORD*)0xC97C28);
	vmtHooks_ = std::make_unique<VMTHookManager>(vTableDevice);
	oEndScene = (_EndScene)vmtHooks_->Hook(42, (void*)EndScene);
	oReset = (_Reset)vmtHooks_->Hook(16, (void*)Reset);

	//plugin::Events::drawHudEvent.after += [&] { render(); };
	plugin::Events::gameProcessEvent += [&] { process(); };
	plugin::Events::shutdownRwEvent.before += [&] { shutdown(); };
}

std::list<std::unique_ptr<Windows::AbstractWindow>>& Windows::WindowsRenderService::getWindows() {
	return windows_;
}

bool& Windows::WindowsRenderService::isMouseShown() {
	return mouseShown_;
}

void Windows::WindowsRenderService::setMouseShown(bool mouseShown) {
	mouseShown_ = mouseShown;
}

void Windows::WindowsRenderService::addRender(std::string name, std::function<void()> renderFunc) {
	this->renderList_.emplace(name, renderFunc);
}

void Windows::WindowsRenderService::removeRender(const std::string name) {
	this->renderList_.erase(name);
}

void Windows::WindowsRenderService::closeAllWindows() const {
	for (const auto &window : windows_) {
		if (window->isShow()) 
			window->close();
	}
}
