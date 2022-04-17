// ReSharper disable CppMemberFunctionMayBeStatic
#include "WindowsRenderService.h"
#include "plugin.h"
#include "imgui.h"
#include "imguistyleloader.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Logger.h"
#include "Settings.h"
#include "../Windows/IWindow.h"
#include "../utils/fa.h"
#include <CMenuManager.h>

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

void Windows::WindowsRenderService::render() const {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (const auto& window : this->windows_) {
		if (window->isShow()) {
			window->draw();
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Windows::WindowsRenderService::shutdown() const {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	vmtHooks_->UnhookAll();
}

void Windows::WindowsRenderService::mouseProcess() const {
	// Disable player controls for controllers
	bool bMouseDisabled = false;
	const bool isController = plugin::patch::Get<BYTE>(0xBA6818);

	if (isController && (this->mouseShown_ || bMouseDisabled)) {
		CPlayerPed* player = FindPlayerPed();
		CPad* pad = player ? player->GetPadFromPlayer() : NULL;

		if (pad)
		{
			if (mouseShown_) {
				bMouseDisabled = true;
				pad->DisablePlayerControls = true;
			} else {
				bMouseDisabled = false;
				pad->DisablePlayerControls = false;
			}
		}
	}

	static bool mouseState;
	if (mouseState != mouseShown_)
	{
		ImGui::GetIO().MouseDrawCursor = mouseShown_;

		if (mouseShown_ && !FrontEndMenuManager.m_bMenuActive)
		{

			plugin::patch::SetUChar(0x6194A0, 0xC3); // psSetMousePos
			plugin::patch::Nop(0x541DD7, 5); // don't call CPad::UpdateMouse()
		}
		else
		{

			plugin::patch::SetUChar(0x6194A0, 0xE9);
			plugin::patch::SetRaw(0x541DD7, const_cast<char*>("\xE8\xE4\xD5\xFF\xFF"), 5);
		}

		CPad::NewMouseControllerState.X = 0;
		CPad::NewMouseControllerState.Y = 0;
		CPad::ClearMouseHistory();
		CPad::UpdatePads();
		mouseState = mouseShown_;
	}
}


void Windows::WindowsRenderService::process() const {
	mouseProcess();
}

void Windows::WindowsRenderService::style() {
	if (!ImGui::StyleLoader::LoadFile(fmt::format("LDYOM/Themes/{}.toml", Settings::getInstance().get<std::string>("main.theme").value()))) {
		Logger::getInstance().log("invalid load theme!");
	}
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

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT WINAPI Reset(IDirect3DDevice9* m_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	return oReset(m_pDevice, pPresentationParameters);
}

void initImGui() {
	/// Инициализируем ImGui
	if (!ImGui::GetCurrentContext())
	{
		ImGui::CreateContext();
	}

	ImGui_ImplWin32_Init(RsGlobal.ps->window);
	ImGui::GetIO().MouseDoubleClickTime = 0.8f;

	ImFontConfig font_config;
	font_config.Density = ImGui_ImplWin32_GetDpiScaleForHwnd(RsGlobal.ps->window);
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;

	const auto allocate_fira_regular = new unsigned int[fira_regular_compressed_size / 4];
	memcpy(allocate_fira_regular, fira_regular_compressed_data, fira_regular_compressed_size);

	static const ImWchar lang_ranges[] = { 0x0020,0xFFFF,0 };
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(allocate_fira_regular, static_cast<int>(fira_regular_compressed_size), 16.f, &font_config, lang_ranges);

	const auto allocate_fa = new unsigned int[fa_compressed_size / 4];
	memcpy(allocate_fa, fa_compressed_data, fa_compressed_size);

	// Грузим иконочный шрифт Font Awesome 5
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(allocate_fa, static_cast<int>(fa_compressed_size), 16.f, &icons_config, icons_ranges);

	//ImGui::GetIO().DisplayFramebufferScale = ImVec2(.2f, .2f);

	ImGui::GetIO().Fonts->Build();

	plugin::patch::Nop(0x00531155, 5); // shift trigger fix

	ImGui_ImplDX9_Init(GetD3DDevice());

	ImGui_ImplWin32_EnableDpiAwareness();

	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(RsGlobal.ps->window, GWL_WNDPROC, reinterpret_cast<LRESULT>(wndProc)));

	Windows::WindowsRenderService::getInstance().style();

	isInitImgui = true;
}

HRESULT WINAPI EndScene(IDirect3DDevice9* pDevice) {
	if (!isInitImgui)
		initImGui();
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

std::list<std::unique_ptr<Windows::IWindow>>& Windows::WindowsRenderService::getWindows() {
	return windows_;
}

bool& Windows::WindowsRenderService::isMouseShown() {
	return mouseShown_;
}

void Windows::WindowsRenderService::setMouseShown(bool mouseShown) {
	mouseShown_ = mouseShown;
}

void Windows::WindowsRenderService::closeAllWindows() const {
	for (const auto &window : windows_)
	{
		window->close();
	}
}
