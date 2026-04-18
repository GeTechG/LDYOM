// ImHotKey v1.0
// https://github.com/CedricGuillemet/ImHotKey
//
// The MIT License(MIT)
//
// Copyright(c) 2019 Cedric Guillemet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once
/*
How to use it?

// Get some hotkeys composed of:
// - hotkey name
// - hotkey comment/lib
// - hotkey scancodes. Computed by the editor. Store that value in your app.

static std::vector<ImHotKey::HotKey> hotkeys = { { "Layout", "Reorder nodes in a simpler layout", 0xFFFF261D}
        ,{"Save", "Save the current graph", 0xFFFF1F1D}
        ,{"Load", "Load an existing graph file", 0xFFFF181D}
        ,{"Play/Stop", "Play or stop the animation from the current graph", 0xFFFFFF3F}
        ,{"SetKey", "Make a new animation key with the current parameters values at the current time", 0xFFFFFF1F}
        };

// The editor is a modal window. bring it with something like that
 if (ImGui::Button("Edit Hotkeys"))
{
    ImGui::OpenPopup("HotKeys Editor");
}
ImHotKey::Edit(hotkeys.data(), hotkeys.size(), "HotKeys Editor");

// ImHotKey also provides a way to retrieve HotKey
int hotkey = ImHotKey::GetHotKey(hotkeys.data(), hotkeys.size());
if (hotkey != -1)
{
    // handle the hotkey index!
}

Awesome, you are done!
To help you integrate in your app, you can get a text (like "Ctrl + O") to integrate in your menu
static void GetHotKeyLib(unsigned int functionKeys, char* buffer, size_t bufferSize);

*/

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Logger.h"
#include "fmt/core.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "localization.h"
#include <algorithm>
#include <functional>
#include <vector>

#define NOMINMAX
#include <utils/imgui_configurate.h>
#include <windows.h>

namespace ImHotKey {
struct HotKey {
	std::string functionName;
	unsigned int functionKeys;
};

struct Key {
	const char* lib = nullptr;
	unsigned int order;
	unsigned int scanCodePage1 = 0; // win32 scancode
	unsigned int scanCodePage7 = 0; // HID (SDL,...)
	float offset = 0;
	float width = 40;
};

static const Key Keys[6][18] = {{{"Esc", 4, 0x1, 0x29, 18},
                                 {"F1", 5, 0x3B, 0x3A, 18},
                                 {"F2", 6, 0x3C, 0x3B},
                                 {"F3", 7, 0x3D, 0x3C},
                                 {"F4", 8, 0x3E, 0x3D},
                                 {"F5", 9, 0x3F, 0x3E, 24},
                                 {"F6", 10, 0x40, 0x3F},
                                 {"F7", 11, 0x41, 0x40},
                                 {"F8", 12, 0x42, 0x41},
                                 {"F9", 13, 0x43, 0x42, 24},
                                 {"F10", 14, 0x44, 0x43},
                                 {"F11", 15, 0x57, 0x44},
                                 {"F12", 16, 0x58, 0x45},
                                 {"PrSn", 17, 0x37, 0x46, 24},
                                 {"ScLk", 18, 0x46},
                                 {"Brk", 19, 126, 0x47}},
                                {{"~", 20, 0x29, 0x35},
                                 {"1", 21, 0x2, 0x1E},
                                 {"2", 22, 0x3, 0x1F},
                                 {"3", 23, 0x4, 0x20},
                                 {"4", 24, 0x5, 0x21},
                                 {"5", 25, 0x6, 0x22},
                                 {"6", 26, 0x7, 0x23},
                                 {"7", 27, 0x8, 0x24},
                                 {"8", 28, 0x9, 0x25},
                                 {"9", 29, 0xA, 0x26},
                                 {"0", 30, 0xB, 0x27},
                                 {"-", 31, 0xC, 0x2D},
                                 {"+", 32, 0xD, 0x2E},
                                 {"Backspace", 33, 0xE, 0x2A, 0, 80},
                                 {"Ins", 34, 0x52, 0x49, 24},
                                 {"Hom", 35, 0x47, 0x4A},
                                 {"PgU", 36, 0x49, 0x4B}},
                                {{"Tab", 3, 0xF, 0x2B, 0, 60},
                                 {"Q", 37, 0x10, 0x14},
                                 {"W", 38, 0x11, 0x1A},
                                 {"E", 39, 0x12, 0x08},
                                 {"R", 40, 0x13, 0x15},
                                 {"T", 41, 0x14, 0x17},
                                 {"Y", 42, 0x15, 0x1C},
                                 {"U", 43, 0x16, 0x18},
                                 {"I", 44, 0x17, 0x0C},
                                 {"O", 45, 0x18, 0x12},
                                 {"P", 46, 0x19, 0x13},
                                 {"[", 47, 0x1A, 0x2F},
                                 {"]", 48, 0x1B, 0x30},
                                 {"|", 49, 0x2B, 0x31, 0, 60},
                                 {"Del", 50, 0x53, 0x4C, 24},
                                 {"End", 51, 0x4F, 0x4D},
                                 {"PgD", 52, 0x51, 0x4E}},
                                {{"Caps Lock", 53, 0x3A, 0x39, 0, 80},
                                 {"A", 54, 0x1E, 0x04},
                                 {"S", 55, 0x1F, 0x16},
                                 {"D", 56, 0x20, 0x07},
                                 {"F", 57, 0x21, 0x09},
                                 {"G", 58, 0x22, 0x0A},
                                 {"H", 59, 0x23, 0x0B},
                                 {"J", 60, 0x24, 0x0D},
                                 {"K", 61, 0x25, 0x0E},
                                 {"L", 62, 0x26, 0x0F},
                                 {";", 63, 0x27, 0x33},
                                 {"'", 64, 0x28, 0x34},
                                 {"Ret", 65, 0x1C, 0X28, 0, 84}},
                                {{"Shift", 2, 0x2A, 0xE1, 0, 104},
                                 {"Z", 66, 0x2C, 0x1D},
                                 {"X", 67, 0x2D, 0x1B},
                                 {"C", 68, 0x2E, 0x06},
                                 {"V", 69, 0x2F, 0x19},
                                 {"B", 70, 0x30, 0x05},
                                 {"N", 71, 0x31, 0x11},
                                 {"M", 72, 0x32, 0x10},
                                 {",", 73, 0x33, 0x36},
                                 {".", 74, 0x34, 0x37},
                                 {"/", 75, 0x35, 0x38},
                                 {"Shift", 2, 0x2A, 0xE5, 0, 104},
                                 {"Up", 76, 0x48, 0x52, 68}},
                                {{"Ctrl", 0, 0x1D, 0xE0, 0, 60},
                                 {"Alt", 1, 0x38, 0xE2, 68, 60},
                                 {"Space", 77, 0x39, 0X2c, 0, 260},
                                 {"Alt", 1, 0x38, 0xE6, 0, 60},
                                 {"Ctrl", 0, 0x1D, 0xE4, 68, 60},
                                 {"Left", 78, 0x4B, 0x50, 24},
                                 {"Down", 79, 0x50, 0x51},
                                 {"Right", 80, 0x4D, 0x52}}};

static const Key& GetKeyForScanCode(unsigned int scancode) {
	for (unsigned int y = 0; y < 6; y++) {
		int x = 0;
		while (Keys[y][x].lib) {
			if (Keys[y][x].scanCodePage1 == scancode)
				return Keys[y][x];
			x++;
		}
	}
	return Keys[0][0];
}

static const Key& GetKeyForName(std::string& name) {
	for (unsigned int y = 0; y < 6; y++) {
		int x = 0;
		while (Keys[y][x].lib) {
			if (name._Equal(Keys[y][x].lib))
				return Keys[y][x];
			x++;
		}
	}
	return Keys[0][0];
}

static unsigned int GetOrderedScanCodes(unsigned char scanCodes[4], unsigned char order[4]) {
	for (int pass = 0; pass < 2; pass++) {
		for (int o = 0; o < 3; o++) {
			if (order[o] > order[o + 1]) {
				ImSwap(order[o], order[o + 1]);
				ImSwap(scanCodes[o], scanCodes[o + 1]);
			}
		}
	}
	return (scanCodes[3] << 24) + (scanCodes[2] << 16) + (scanCodes[1] << 8) + scanCodes[0];
}

static void HotKeySPrintf(char* buffer, size_t bufferSize, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	ImFormatStringV(buffer, bufferSize, fmt, args);
	va_end(args);
}

static void GetHotKeyLib(unsigned int functionKeys, char* buffer, size_t bufferSize,
                         const char* functionLib = nullptr) {
	static const char* str[4] = {"%s", "%s + %s", "%s + %s + %s", "%s + %s + %s + %s"};
	static const char* strLib[4] = {"%s (%s)", "%s (%s + %s)", "%s (%s + %s + %s)", "%s (%s + %s + %s + %s)"};
	static const char* lib[4];
	int scanCodeCount = 0;
	for (int i = 0; i < 4; i++) {
		unsigned char scanCode = (unsigned char)(functionKeys >> i * 8);
		if (scanCode == 0xFF) {
			continue;
		}
		lib[scanCodeCount] = GetKeyForScanCode(scanCode).lib;
		scanCodeCount++;
	}
	if (!scanCodeCount) {
		buffer[0] = 0;
		return;
	}

	if (functionLib) {
		const char* fmt = strLib[scanCodeCount - 1];
		HotKeySPrintf(buffer, bufferSize, fmt, functionLib, lib[0], lib[1], lib[2], lib[3]);
	} else {
		const char* fmt = str[scanCodeCount - 1];
		HotKeySPrintf(buffer, bufferSize, fmt, lib[0], lib[1], lib[2], lib[3]);
	}
}

// ---------------------------------------------------------------------------
// KeyboardStyle – visual constants for the three-layer keycap renderer
// ---------------------------------------------------------------------------
struct KeyboardStyle {
	ImU32  KeyNormal;        // outer fill, unpressed key
	ImU32  KeyHover;         // outer fill, hovered but not pressed
	ImU32  KeyPressed;       // outer fill, key is part of current binding
	ImU32  KeyRecorded;      // outer fill, key toggled by physical press this session
	ImU32  KeyBorder;        // border stroke color
	ImU32  KeyFace;          // inner "face" rect fill
	ImU32  KeyLabel;         // text / icon color
	float  OuterRounding;
	float  InnerRounding;
	float  BorderThickness;
	float  FaceInset;
};

// ---------------------------------------------------------------------------
// BuildKeyboardStyleFromTheme – sample ImGuiStyle every Edit() call so colors
// always match the active theme. Opaque fill colors force alpha=1.0 to avoid
// translucent keycaps when ImGuiCol_Button has partial alpha (e.g. default theme
// uses alpha 0.40).
// ---------------------------------------------------------------------------
static KeyboardStyle BuildKeyboardStyleFromTheme() {
	const ImGuiStyle& s = ImGui::GetStyle();

	// Source colors from ImGuiStyle
	ImVec4 btnNormal   = s.Colors[ImGuiCol_Button];
	ImVec4 btnHovered  = s.Colors[ImGuiCol_ButtonHovered];
	ImVec4 header      = s.Colors[ImGuiCol_Header];       // KeyPressed
	ImVec4 checkMark   = s.Colors[ImGuiCol_CheckMark];    // KeyRecorded
	ImVec4 border      = s.Colors[ImGuiCol_Border];
	ImVec4 text        = s.Colors[ImGuiCol_Text];

	// Force alpha=1.0 on opaque background colors so partially-transparent theme
	// button colors don't leak through the popup.
	btnNormal.w  = 1.0f;
	btnHovered.w = 1.0f;
	header.w     = 1.0f;

	// Derive KeyFace by lifting KeyNormal in HSV (+0.08 V, clamped to [0,1])
	float h, sv, v;
	ImGui::ColorConvertRGBtoHSV(btnNormal.x, btnNormal.y, btnNormal.z, h, sv, v);
	v = ImClamp(v + 0.08f, 0.0f, 1.0f);
	float fr, fg, fb;
	ImGui::ColorConvertHSVtoRGB(h, sv, v, fr, fg, fb);
	const ImVec4 keyFace = ImVec4(fr, fg, fb, 1.0f);

	KeyboardStyle style;
	style.KeyNormal      = ImGui::ColorConvertFloat4ToU32(btnNormal);
	style.KeyHover       = ImGui::ColorConvertFloat4ToU32(btnHovered);
	style.KeyPressed     = ImGui::ColorConvertFloat4ToU32(header);
	style.KeyRecorded    = ImGui::ColorConvertFloat4ToU32(checkMark);
	style.KeyBorder      = ImGui::ColorConvertFloat4ToU32(border);
	style.KeyFace        = ImGui::ColorConvertFloat4ToU32(keyFace);
	style.KeyLabel       = ImGui::ColorConvertFloat4ToU32(text);
	style.OuterRounding  = 4.0f;
	style.InnerRounding  = 3.0f;
	style.BorderThickness = 1.0f;
	style.FaceInset      = 2.5f;
	return style;
}

// ---------------------------------------------------------------------------
// DrawKeyIcon – draw a vector icon for special keys onto dl inside faceRect.
// Returns true if an icon was drawn, false if the caller should fall back to text.
// ---------------------------------------------------------------------------
static bool DrawKeyIcon(ImDrawList* dl, const ImRect& faceRect, const char* lib, ImU32 color) {
	const float h = faceRect.GetHeight();
	// Task 4.7 – minimum size guard: if the icon would be tiny, tell caller to use text.
	if (h * 0.4f < 4.0f)
		return false;

	const float size   = ImMax(4.0f, h * 0.4f);
	const ImVec2 c     = faceRect.GetCenter();

	// Task 4.2 – directional arrow keys
	if (strcmp(lib, "Up") == 0) {
		// Apex at top, base at bottom
		ImVec2 p0 = ImVec2(c.x,            c.y - size * 0.6f); // apex
		ImVec2 p1 = ImVec2(c.x - size * 0.6f, c.y + size * 0.4f); // bottom-left
		ImVec2 p2 = ImVec2(c.x + size * 0.6f, c.y + size * 0.4f); // bottom-right
		dl->AddTriangleFilled(p0, p1, p2, color);
		return true;
	}
	if (strcmp(lib, "Down") == 0) {
		ImVec2 p0 = ImVec2(c.x,            c.y + size * 0.6f); // apex bottom
		ImVec2 p1 = ImVec2(c.x - size * 0.6f, c.y - size * 0.4f); // top-left
		ImVec2 p2 = ImVec2(c.x + size * 0.6f, c.y - size * 0.4f); // top-right
		dl->AddTriangleFilled(p0, p1, p2, color);
		return true;
	}
	if (strcmp(lib, "Left") == 0) {
		ImVec2 p0 = ImVec2(c.x - size * 0.6f, c.y);            // apex left
		ImVec2 p1 = ImVec2(c.x + size * 0.4f, c.y - size * 0.6f); // top-right
		ImVec2 p2 = ImVec2(c.x + size * 0.4f, c.y + size * 0.6f); // bottom-right
		dl->AddTriangleFilled(p0, p1, p2, color);
		return true;
	}
	if (strcmp(lib, "Right") == 0) {
		ImVec2 p0 = ImVec2(c.x + size * 0.6f, c.y);            // apex right
		ImVec2 p1 = ImVec2(c.x - size * 0.4f, c.y - size * 0.6f); // top-left
		ImVec2 p2 = ImVec2(c.x - size * 0.4f, c.y + size * 0.6f); // bottom-left
		dl->AddTriangleFilled(p0, p1, p2, color);
		return true;
	}

	// Task 4.3 – Shift: upward chevron (same shape as Up arrow)
	if (strcmp(lib, "Shift") == 0) {
		ImVec2 p0 = ImVec2(c.x,            c.y - size * 0.6f);
		ImVec2 p1 = ImVec2(c.x - size * 0.6f, c.y + size * 0.4f);
		ImVec2 p2 = ImVec2(c.x + size * 0.6f, c.y + size * 0.4f);
		dl->AddTriangleFilled(p0, p1, p2, color);
		return true;
	}

	// Task 4.4 – Ret (Enter): return-arrow glyph — L-shape polyline + arrowhead
	if (strcmp(lib, "Ret") == 0) {
		const float thick = ImMax(1.0f, size * 0.15f);
		// L-shape: start top-right, go down, then left
		// p0 = upper-right, p1 = lower-right (corner), p2 = lower-left (left end of shaft)
		ImVec2 pts[3] = {
			ImVec2(c.x + size * 0.5f, c.y - size * 0.4f), // top-right
			ImVec2(c.x + size * 0.5f, c.y + size * 0.2f), // corner
			ImVec2(c.x - size * 0.3f, c.y + size * 0.2f), // left terminus
		};
		dl->AddPolyline(pts, 3, color, 0, thick);
		// Arrowhead pointing left at left terminus
		ImVec2 ah0 = ImVec2(c.x - size * 0.6f, c.y + size * 0.2f);          // apex left
		ImVec2 ah1 = ImVec2(c.x - size * 0.2f, c.y + size * 0.2f - size * 0.25f); // top
		ImVec2 ah2 = ImVec2(c.x - size * 0.2f, c.y + size * 0.2f + size * 0.25f); // bottom
		dl->AddTriangleFilled(ah0, ah1, ah2, color);
		return true;
	}

	// Task 4.5 – Tab: right-pointing arrow (shaft + arrowhead)
	if (strcmp(lib, "Tab") == 0) {
		const float thick = ImMax(1.0f, size * 0.15f);
		// Horizontal shaft from left to where arrowhead base starts
		ImVec2 shaftL = ImVec2(c.x - size * 0.5f, c.y);
		ImVec2 shaftR = ImVec2(c.x + size * 0.2f, c.y);
		dl->AddLine(shaftL, shaftR, color, thick);
		// Arrowhead pointing right
		ImVec2 ah0 = ImVec2(c.x + size * 0.6f, c.y);               // apex right
		ImVec2 ah1 = ImVec2(c.x + size * 0.1f, c.y - size * 0.35f); // top-left
		ImVec2 ah2 = ImVec2(c.x + size * 0.1f, c.y + size * 0.35f); // bottom-left
		dl->AddTriangleFilled(ah0, ah1, ah2, color);
		return true;
	}

	return false;
}

// ---------------------------------------------------------------------------
// PackKeyDown – pack the keyDown[512] bitmask into a sorted functionKeys word.
// Returns 0xFFFFFFFF if no keys are pressed, 0 if the packed result is zero.
// ---------------------------------------------------------------------------
static unsigned int PackKeyDown(const bool keyDown[512]) {
	unsigned char scanCodes[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char order[4]     = {0xFF, 0xFF, 0xFF, 0xFF};
	int scanCodeCount = 0;
	for (int i = 1; i < 512 && scanCodeCount < 4; i++) {
		if (keyDown[i]) {
			scanCodes[scanCodeCount] = (unsigned char)i;
			order[scanCodeCount]     = (unsigned char)GetKeyForScanCode(i).order;
			scanCodeCount++;
		}
	}
	if (scanCodeCount == 0)
		return 0;
	return GetOrderedScanCodes(scanCodes, order);
}

static void Edit(HotKey* hotkey, size_t hotkeyCount, const char* popupModal, std::function<void(int)> onSetHotKey) {
	static int editingHotkey = -1;
	if (!hotkeyCount)
		return;
	static bool keyDown[512] = {};
	static bool recordingActive = false;

	// Section 6.1 – dirty-state tracking
	static unsigned int savedFunctionKeysAtSelect = 0;
	static int lastEditingHotkey = -1;
	static int pendingSelection  = -1;

	// Section 6.2 – snapshot saved state when editingHotkey changes
	if (lastEditingHotkey != editingHotkey && editingHotkey >= 0) {
		savedFunctionKeysAtSelect = hotkey[editingHotkey].functionKeys;
		lastEditingHotkey = editingHotkey;
	}

	// Section 6.3 – compute current packed keys each frame
	const unsigned int currentPacked = PackKeyDown(keyDown);

	// Sample theme-derived style once per Edit() call so colors always match
	// the active ImGui theme — cheap (6 color conversions + 1 HSV round-trip).
	const KeyboardStyle style = BuildKeyboardStyleFromTheme();

	// Per-frame transient signal: which scancodes were toggled by a physical key press this frame.
	// Declared as a non-static local so it is zero-initialised on every Edit() call.
	bool recordedThisFrame[512] = {};

	// Layout constants driven by a target key size. Everything else is derived so that
	// the window matches the natural content size and does not leave empty space.
	const ImVec2 viewportSize = ImGui::GetMainViewport()->WorkSize;
	const ImVec2 scale = SCL_PX;
	const float targetKeyW = 44.0f * scale.x;
	const float kbNaturalWidth = targetKeyW * 20.0f + 40.0f;
	const float leftPanelNaturalWidth = 220.0f * scale.x;

	const ImGuiStyle& _style = ImGui::GetStyle();
	// Right-panel components — each ImGui::Separator() adds a 1px line + ItemSpacing.y gap
	const float separatorH = 1.0f + _style.ItemSpacing.y;
	const float kbRowH = targetKeyW * 0.8f + _style.ItemSpacing.y;
	const float kbNaturalHeight = kbRowH * 6.0f;
	// Header: title line + description line + separator
	const float headerNaturalHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + separatorH;
	// Toolbar: separator + button row (FrameHeight*1.6 + ItemSpacing.y advance)
	const float toolbarNaturalHeight = separatorH + ImGui::GetFrameHeight() * 1.6f + _style.ItemSpacing.y;
	const float rightNaturalHeight = headerNaturalHeight + kbNaturalHeight + toolbarNaturalHeight;
	// Left list height: each row is two text lines + spacing
	const float leftRowH = ImGui::GetTextLineHeightWithSpacing() * 2.0f + _style.ItemSpacing.y;
	const float leftNaturalHeight = leftRowH * static_cast<float>(hotkeyCount);
	const float contentNaturalHeight = ImMax(rightNaturalHeight, leftNaturalHeight);
	const float contentNaturalWidth = leftPanelNaturalWidth + kbNaturalWidth + _style.ItemSpacing.x;

	const float titleBarH = ImGui::GetFontSize() + _style.FramePadding.y * 2.0f;
	ImVec2 windowSize = ImVec2(
		contentNaturalWidth + _style.WindowPadding.x * 2.0f,
		contentNaturalHeight + _style.WindowPadding.y * 2.0f + titleBarH
	);
	windowSize.x = ImMin(windowSize.x, viewportSize.x);
	windowSize.y = ImMin(windowSize.y, viewportSize.y);

	ImGui::SetNextWindowSize(windowSize);
	if (!ImGui::BeginPopupModal(popupModal, NULL, ImGuiWindowFlags_NoResize))
		return;

	// Calculate dynamic sizes for layout
	float contentHeight = ImGui::GetContentRegionAvail().y;
	float leftPanelWidth = leftPanelNaturalWidth;
	float rightPanelWidth = kbNaturalWidth;

	// Section 7.1 – build conflict list: indices of hotkeys that share the same packed value as currentPacked
	// (only when currentPacked != 0 to avoid flagging multiple unbound entries)
	std::vector<int> conflictingIndices;
	if (currentPacked != 0) {
		// Count how many entries share currentPacked
		int matchCount = 0;
		for (size_t ci = 0; ci < hotkeyCount; ci++) {
			if (hotkey[ci].functionKeys == currentPacked)
				matchCount++;
		}
		// Also check if the editing row's currentPacked differs from its saved state
		// (i.e. the user is editing and the new packed conflicts with another entry)
		if (matchCount >= 2) {
			// Multiple entries share this packed value — all of them are conflicting
			for (size_t ci = 0; ci < hotkeyCount; ci++) {
				if (hotkey[ci].functionKeys == currentPacked)
					conflictingIndices.push_back(static_cast<int>(ci));
			}
		} else if (matchCount == 1 && editingHotkey >= 0 &&
		           hotkey[editingHotkey].functionKeys != currentPacked) {
			// The editing row has an unsaved value that collides with exactly one other entry
			for (size_t ci = 0; ci < hotkeyCount; ci++) {
				if (hotkey[ci].functionKeys == currentPacked && int(ci) != editingHotkey)
					conflictingIndices.push_back(static_cast<int>(ci));
			}
			conflictingIndices.push_back(editingHotkey);
		}
	}

	// Left panel with hotkeys list
	ImGui::BeginChild(127, ImVec2(leftPanelWidth, contentHeight));
	for (size_t i = 0; i < hotkeyCount; i++) {
		// Section 9.2 – function name (localized), used on the first line
		const std::string functionNameLoc = _(fmt::format("hotkey_editor.hk_{}", hotkey[i].functionName));

		// Section 9.3 – bare binding string (no parenthesized wrapper), used on the second line
		char bindingBuf[128];
		GetHotKeyLib(hotkey[i].functionKeys, bindingBuf, sizeof(bindingBuf));
		// bindingBuf now holds "Ctrl + S" or "" (unbound) — no parens wrapper

		// Section 6.4 – a row is dirty when it is the editing row and keys differ
		const bool isDirty = (editingHotkey == int(i)) && (currentPacked != savedFunctionKeysAtSelect);

		// Section 7.2 – draw red conflict bullet before the selectable for conflicting rows
		const bool isConflicting = std::find(conflictingIndices.begin(), conflictingIndices.end(), int(i))
		                           != conflictingIndices.end();

		// Section 9.1 – two-line Selectable: height covers both lines, AllowOverlap keeps click area full
		const float rowHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f;
		const ImVec2 selectableMin = ImGui::GetCursorScreenPos();
		const ImGuiStyle& imStyle = ImGui::GetStyle();
		const ImVec2 framePadding = imStyle.FramePadding;

		// Use empty label so ImGui doesn't render any text inside the Selectable itself
		const std::string selectableId = fmt::format("##{}_selectable", i);
		const bool selected = ImGui::Selectable(selectableId.c_str(), editingHotkey == int(i),
		                                        ImGuiSelectableFlags_AllowOverlap,
		                                        ImVec2(0.0f, rowHeight));
		const bool selectableHovered = ImGui::IsItemHovered();
		if (selected || editingHotkey == -1) {
			// Section 6.6 – if current row is dirty and user clicked a different row, show discard popup
			const bool switchingRow = (editingHotkey != int(i));
			const bool currentDirty = (editingHotkey >= 0) &&
			                          (currentPacked != savedFunctionKeysAtSelect);
			if (switchingRow && currentDirty) {
				pendingSelection = int(i);
				ImGui::OpenPopup("unsaved_discard");
			} else {
				editingHotkey = int(i);
				recordingActive = false;
				memset(keyDown, 0, sizeof(keyDown));
				for (int j = 0; j < 4; j++) {
					int scan = (hotkey[editingHotkey].functionKeys >> (8 * j)) & 0xFF;
					if (scan != 0xFF) {
						keyDown[scan] = true;
					}
				}
			}
		}

		// Section 9.2 – rewind cursor to Selectable's top-left (plus frame padding) and render function name
		ImGui::SetCursorScreenPos(selectableMin + framePadding);
		// Section 9.4 / 6.5 – dirty-asterisk prefix on the function-name line
		const std::string firstLine = isDirty
			? std::string("* ") + functionNameLoc
			: functionNameLoc;
		ImGui::Text("%s", firstLine.c_str());

		// Section 9.3 – binding preview on the second line in dimmed color
		// Re-use the same X position, advance Y by one line height
		ImGui::SetCursorScreenPos(ImVec2(selectableMin.x + framePadding.x,
		                                 selectableMin.y + ImGui::GetTextLineHeightWithSpacing() + framePadding.y));
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		const char* previewText = (bindingBuf[0] != '\0') ? bindingBuf : "---";
		ImGui::TextUnformatted(previewText);
		ImGui::PopStyleColor();

		// Advance cursor past the full two-line row so the next iteration starts below.
		// Dummy() after SetCursorScreenPos is required to grow the parent window bounds
		// (ImGui asserts if a cursor move is not followed by a submitted item).
		ImGui::SetCursorScreenPos(ImVec2(selectableMin.x,
		                                 selectableMin.y + rowHeight + imStyle.ItemSpacing.y));
		ImGui::Dummy(ImVec2(0.0f, 0.0f));

		// Section 7.2 / 9.4 – conflict bullet: centered on the function-name line (top half of the row)
		if (isConflicting) {
			const float nameCenterY = selectableMin.y + ImGui::GetTextLineHeight() * 0.5f + framePadding.y;
			const ImVec2 bulletCenter = ImVec2(selectableMin.x + 4.0f, nameCenterY);
			ImGui::GetWindowDrawList()->AddCircleFilled(bulletCenter, 4.0f, IM_COL32(220, 50, 50, 255));
		}

		// Section 7.3 – conflict tooltip describing the other function(s) sharing the combo
		// Use selectableHovered (captured right after Selectable) since cursor was rewound for text rendering
		if (isConflicting && selectableHovered) {
			// Collect the names of other conflicting entries
			for (int ci : conflictingIndices) {
				if (ci == int(i))
					continue;
				const std::string conflictKey = fmt::format("hotkey_editor.hk_{}", hotkey[ci].functionName);
				const std::string conflictName = _(conflictKey);
				const std::string tooltipKey = _("hotkey_editor.conflict_tooltip");
				const std::string tooltipText = fmt::format(fmt::runtime(tooltipKey), conflictName);
				ImGui::SetTooltip("%s", tooltipText.c_str());
				break; // show tooltip for the first conflicting peer only
			}
		}
	}

	// Section 6.6 / 6.7 – unsaved discard popup
	if (ImGui::BeginPopup("unsaved_discard")) {
		const std::string titleStr = _("hotkey_editor.unsaved_discard_title");
		ImGui::TextUnformatted(titleStr.c_str());
		ImGui::Separator();
		const std::string msgStr = _("hotkey_editor.unsaved_discard_message");
		ImGui::TextWrapped("%s", msgStr.c_str());
		ImGui::Spacing();
		// Discard button: switch to pending selection
		const std::string discardStr = _("hotkey_editor.discard");
		if (ImGui::Button(discardStr.c_str())) {
			editingHotkey = pendingSelection;
			pendingSelection = -1;
			recordingActive = false;
			memset(keyDown, 0, sizeof(keyDown));
			for (int j = 0; j < 4; j++) {
				int scan = (hotkey[editingHotkey].functionKeys >> (8 * j)) & 0xFF;
				if (scan != 0xFF) {
					keyDown[scan] = true;
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		// Cancel button: keep current state
		const std::string cancelStr = _("hotkey_editor.cancel");
		if (ImGui::Button(cancelStr.c_str())) {
			pendingSelection = -1;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginGroup();

	// Header: selected function name, current binding chip (right-aligned), description
	{
		const std::string funcName = _(fmt::format("hotkey_editor.hk_{}", hotkey[editingHotkey].functionName));
		const std::string funcDesc = _(fmt::format("hotkey_editor.hk_{}_desc", hotkey[editingHotkey].functionName));
		char bindingPreview[128] = {};
		GetHotKeyLib(hotkey[editingHotkey].functionKeys, bindingPreview, sizeof(bindingPreview));

		// Title (selected function name)
		ImGui::TextUnformatted(funcName.c_str());

		// Binding chip — right-aligned on the same line
		if (bindingPreview[0]) {
			const ImVec2 textSize = ImGui::CalcTextSize(bindingPreview);
			const float chipW = textSize.x + ImGui::GetStyle().FramePadding.x * 4.0f;
			const float avail = ImGui::GetContentRegionAvail().x;
			if (avail > chipW + 4.0f) {
				ImGui::SameLine(0, avail - chipW);
			} else {
				ImGui::SameLine();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			ImGui::Button(bindingPreview, ImVec2(chipW, 0));
			ImGui::PopStyleColor(3);
		}

		// Description (muted)
		if (!funcDesc.empty()) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
			ImGui::TextWrapped("%s", funcDesc.c_str());
			ImGui::PopStyleColor();
		}
		ImGui::Separator();
	}

	// Keyboard sizing is driven by the target key width computed up-front (outside the popup).
	// baseKeyHeight == baseKeyWidth keeps keys square (original keyboard proportions).
	const float baseKeyWidth = targetKeyW;
	const float baseKeyHeight = targetKeyW;

	// Новая система обработки клавиш
	bool recordingFirstKey = recordingActive; // track if we've cleared keyDown yet this loop
	for (int i = ImGuiKey_Tab; i <= ImGuiKey_KeypadEqual; i++) {
		// Escape is reserved as the cancel/close shortcut for the popup — never capture it
		// into the staged binding, otherwise pressing Esc to dismiss the editor would
		// corrupt the hotkey being edited.
		if (i == ImGuiKey_Escape) continue;
		if (ImGui::IsKeyPressed((ImGuiKey)i, false)) {
			int vk = 0;
			switch ((ImGuiKey)i) {
				case ImGuiKey_Tab: vk = VK_TAB; break;
				case ImGuiKey_LeftArrow: vk = VK_LEFT; break;
				case ImGuiKey_RightArrow: vk = VK_RIGHT; break;
				case ImGuiKey_UpArrow: vk = VK_UP; break;
				case ImGuiKey_DownArrow: vk = VK_DOWN; break;
				case ImGuiKey_PageUp: vk = VK_PRIOR; break;
				case ImGuiKey_PageDown: vk = VK_NEXT; break;
				case ImGuiKey_Home: vk = VK_HOME; break;
				case ImGuiKey_End: vk = VK_END; break;
				case ImGuiKey_Delete: vk = VK_DELETE; break;
				case ImGuiKey_Backspace: vk = VK_BACK; break;
				case ImGuiKey_Enter: vk = VK_RETURN; break;
				case ImGuiKey_Space: vk = VK_SPACE; break;
				case ImGuiKey_GraveAccent: vk = VK_OEM_3; break;
				case ImGuiKey_Minus: vk = VK_OEM_MINUS; break;
				case ImGuiKey_Equal: vk = VK_OEM_PLUS; break;
				case ImGuiKey_LeftBracket: vk = VK_OEM_4; break;
				case ImGuiKey_RightBracket: vk = VK_OEM_6; break;
				case ImGuiKey_Backslash: vk = VK_OEM_5; break;
				case ImGuiKey_Semicolon: vk = VK_OEM_1; break;
				case ImGuiKey_Apostrophe: vk = VK_OEM_7; break;
				case ImGuiKey_Comma: vk = VK_OEM_COMMA; break;
				case ImGuiKey_Period: vk = VK_OEM_PERIOD; break;
				case ImGuiKey_Slash: vk = VK_OEM_2; break;
				default:
					if (i >= ImGuiKey_A && i <= ImGuiKey_Z)
						vk = 'A' + (i - ImGuiKey_A);
					else if (i >= ImGuiKey_0 && i <= ImGuiKey_9)
						vk = '0' + (i - ImGuiKey_0);
					else if (i >= ImGuiKey_F1 && i <= ImGuiKey_F12)
						vk = VK_F1 + (i - ImGuiKey_F1);
					break;
			}
			if (vk != 0) {
				int imKey = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
				if (recordingActive) {
					// On the first key captured while recording, clear the current binding.
					if (recordingFirstKey) {
						memset(keyDown, 0, sizeof(keyDown));
						recordingFirstKey = false;
					}
					// Set (not toggle) the key and mark it as recorded this frame.
					keyDown[imKey] = true;
					recordedThisFrame[imKey] = true;
					// Determine if this is a modifier key (Ctrl / Alt / Shift).
					const bool isModifier = (i == ImGuiKey_LeftCtrl  || i == ImGuiKey_RightCtrl  ||
					                         i == ImGuiKey_LeftAlt   || i == ImGuiKey_RightAlt   ||
					                         i == ImGuiKey_LeftShift || i == ImGuiKey_RightShift);
					if (!isModifier) {
						recordingActive = false;
					}
				} else {
					keyDown[imKey] = !keyDown[imKey];
					recordedThisFrame[imKey] = true;
				}
			}
		}
	}

	// Остальная часть функции Edit
	for (unsigned int y = 0; y < 6; y++) {
		int x = 0;
		ImGui::BeginGroup();
		while (Keys[y][x].lib) {
			const Key& key = Keys[y][x];
			const float ofs =
				(key.offset + (x ? 4.f : 0.f)) * (baseKeyWidth / 40.0f); // Scale offset relative to base key width

			const float width = key.width * (baseKeyWidth / 40.0f); // Scale key width
			if (x) {
				ImGui::SameLine(0.f, ofs);
			} else {
				if (ofs >= 1.f) {
					ImGui::Indent(ofs);
				}
			}

			bool& butSwtch = keyDown[key.scanCodePage1];

			// Scale-dependent style values
			const float scaleFactor    = baseKeyWidth / 40.0f;
			const float borderThick    = style.BorderThickness * scaleFactor;
			const float faceInset      = style.FaceInset       * scaleFactor;
			const ImVec2 keySize       = ImVec2(width, baseKeyHeight * 0.8f);

			// 2.1 – InvisibleButton for hit-testing (preserves SameLine/Indent layout)
			const std::string btnId = fmt::format("{}##{}", Keys[y][x].lib, key.scanCodePage7);
			bool clicked = ImGui::InvisibleButton(btnId.c_str(), keySize);

			// Section 8.1 – tooltip for icon-only keys so the user knows their name
			{
				static const char* iconKeys[] = {"Shift", "Ret", "Tab", "Up", "Down", "Left", "Right"};
				for (const char* ik : iconKeys) {
					if (strcmp(key.lib, ik) == 0) {
						ImGui::SetItemTooltip("%s", key.lib);
						break;
					}
				}
			}

			// 2.6 – toggle on click (same semantic as the old Button).
			// While recording, skip mouse clicks so they don't interfere with capture.
			if (clicked && !recordingActive) {
				butSwtch = !butSwtch;
			}

			// 2.2 – grab item rect and draw list after the InvisibleButton
			const ImVec2 pMin = ImGui::GetItemRectMin();
			const ImVec2 pMax = ImGui::GetItemRectMax();
			ImDrawList* dl    = ImGui::GetWindowDrawList();

			// Choose fill color by state: Recorded > Pressed > Hover > Normal.
			// recordedThisFrame is a per-frame local zeroed at the top of Edit(),
			// so KeyRecorded is only shown for the single frame the key was toggled.
			const bool isHovered = ImGui::IsItemHovered();
			const ImU32 fillColor = recordedThisFrame[key.scanCodePage1]
				? style.KeyRecorded
				: (butSwtch
					? style.KeyPressed
					: (isHovered
						? style.KeyHover
						: style.KeyNormal));

			// 2.3 – Layer 1: outer rect (background fill + border stroke)
			dl->AddRectFilled(pMin, pMax, fillColor,     style.OuterRounding);
			dl->AddRect      (pMin, pMax, style.KeyBorder,
			                  style.OuterRounding, 0, borderThick);

			// 2.4 – Layer 2: inner face rect (inset from outer)
			const ImVec2 fMin = ImVec2(pMin.x + faceInset, pMin.y + faceInset);
			const ImVec2 fMax = ImVec2(pMax.x - faceInset, pMax.y - faceInset * 2.0f);
			dl->AddRectFilled(fMin, fMax, style.KeyFace, style.InnerRounding);

			// 2.5 – Layer 3: centered label — icon first, text fallback
			{
				const ImRect faceRectIR   = ImRect(fMin, fMax);
				const bool   drewIcon     = DrawKeyIcon(dl, faceRectIR, key.lib, style.KeyLabel);
				if (!drewIcon) {
					const char* labelStr  = key.lib;
					const ImVec2 textSz   = ImGui::CalcTextSize(labelStr);
					const ImVec2 faceCenter = ImVec2((fMin.x + fMax.x) * 0.5f, (fMin.y + fMax.y) * 0.5f);
					const ImVec2 textPos  = ImVec2(faceCenter.x - textSz.x * 0.5f,
					                               faceCenter.y - textSz.y * 0.5f);

					// Task 5.1 – clip text to face rect if it overflows.
					// smallInset keeps text off the inner-face edge (matches faceInset rounding).
					// TODO (5.2): if overflow persists at min popup size (960×400), apply an
					//   abbreviation table gated behind a width-below-threshold check, e.g.:
					//   "Backspace" → "Bksp", "Caps Lock" → "Caps", "Space" → "Spc".
					const float smallInset = 2.0f;
					const bool  overflows  = textSz.x > faceRectIR.GetWidth() - smallInset * 2.0f;
					if (overflows) {
						dl->PushClipRect(faceRectIR.Min, faceRectIR.Max, true);
					}
					dl->AddText(textPos, style.KeyLabel, labelStr);
					if (overflows) {
						dl->PopClipRect();
					}
				}
			}
			x++;
		}
		ImGui::EndGroup();
	}

	// 4.7 / 4.8 – Recording overlay and accent outline
	if (recordingActive) {
		// Capture the bounding rect of the last rendered row group (approximates keyboard region).
		const ImVec2 kbMin = ImGui::GetItemRectMin();
		const ImVec2 kbMax = ImGui::GetItemRectMax();
		ImDrawList* dlKb   = ImGui::GetWindowDrawList();

		// 4.7 – "Listening…" label centered above the keyboard region.
		// Pulsing alpha derived from frame count gives a subtle animated cue.
		const float pulse = 0.6f + 0.4f * ImSin(static_cast<float>(ImGui::GetFrameCount()) * 0.1f);
		const ImVec4 checkMarkVec4 = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
		const ImU32 pulseColor = ImGui::ColorConvertFloat4ToU32(
			ImVec4(checkMarkVec4.x, checkMarkVec4.y, checkMarkVec4.z, checkMarkVec4.w * pulse));
		const std::string listenLabelStr = _("hotkey_editor.recording");
		const char* listenLabel = listenLabelStr.c_str();
		const ImVec2 labelSz    = ImGui::CalcTextSize(listenLabel);
		const ImVec2 labelPos   = ImVec2((kbMin.x + kbMax.x) * 0.5f - labelSz.x * 0.5f,
		                                 kbMin.y - labelSz.y - 4.0f);
		dlKb->AddText(labelPos, pulseColor, listenLabel);

		// 4.8 – 2px accent-colored outline around the keyboard region (0.5 alpha).
		const ImU32 outlineColor = ImGui::ColorConvertFloat4ToU32(
			ImVec4(checkMarkVec4.x, checkMarkVec4.y, checkMarkVec4.z, checkMarkVec4.w * 0.5f));
		dlKb->AddRect(kbMin, kbMax, outlineColor, 0.0f, 0, 2.0f);
	}

	// Bottom horizontal toolbar: Clear | Record | Set ......... Done (right-aligned)
	ImGui::Separator();

	int keyDownCount = 0;
	for (auto d : keyDown) keyDownCount += d ? 1 : 0;

	const float toolbarButtonW = 120.0f * scale.x;
	const float toolbarButtonH = ImGui::GetFrameHeight() * 1.6f;

	if (ImGui::Button(_("hotkey_editor.clear").c_str(), ImVec2(toolbarButtonW, toolbarButtonH))) {
		memset(keyDown, 0, sizeof(keyDown));
	}
	ImGui::SameLine();
	{
		const std::string recLabelStr = recordingActive
			? _("hotkey_editor.recording")
			: _("hotkey_editor.record");
		if (ImGui::Button(recLabelStr.c_str(), ImVec2(toolbarButtonW, toolbarButtonH))) {
			recordingActive = !recordingActive;
			if (!recordingActive) memset(recordedThisFrame, 0, sizeof(recordedThisFrame));
		}
	}
	ImGui::SameLine();
	{
		const bool setDisabled = (keyDownCount == 0 || keyDownCount > 4);
		ImGui::BeginDisabled(setDisabled);
		if (ImGui::Button(_("hotkey_editor.set").c_str(), ImVec2(toolbarButtonW, toolbarButtonH))) {
			hotkey[editingHotkey].functionKeys = PackKeyDown(keyDown);
			onSetHotKey(editingHotkey);
			savedFunctionKeysAtSelect = hotkey[editingHotkey].functionKeys;
		}
		if (setDisabled) {
			const std::string tooltipKey = (keyDownCount == 0)
				? _("hotkey_editor.set_disabled_empty_tooltip")
				: _("hotkey_editor.set_disabled_toomany_tooltip");
			ImGui::SetItemTooltip("%s", tooltipKey.c_str());
		}
		ImGui::EndDisabled();
	}
	// Right-align Done
	{
		const float avail = ImGui::GetContentRegionAvail().x;
		if (avail > toolbarButtonW) {
			ImGui::SameLine(0, avail - toolbarButtonW);
		} else {
			ImGui::SameLine();
		}
		if (ImGui::Button(_("hotkey_editor.done").c_str(), ImVec2(toolbarButtonW, toolbarButtonH))) {
			recordingActive = false;
			ImGui::CloseCurrentPopup();
		}
	}
	ImGui::EndGroup();

	// Section 10.1 – Escape handling: stop recording first, then close popup on second press
	if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
		if (recordingActive) {
			recordingActive = false;
		} else {
			ImGui::CloseCurrentPopup();
		}
	}

	ImGui::EndPopup();
}

static HotKey* GetHotKey(HotKey* hotkey, size_t hotkeyCount, bool repeat) {
	static unsigned int lastHotKey = 0xFFFFFFFF;
	unsigned char scanCodes[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char order[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	int scanCodeCount = 0;

	ImGuiIO& io = ImGui::GetIO();

	if (io.WantTextInput) {
		// Если ImGui захватывает клавиатуру, то не обрабатываем горячие клавиши
		return nullptr;
	}

	// Добавляем модификаторы первыми, так как они имеют наименьший порядок
	if (io.KeyCtrl) {
		scanCodes[scanCodeCount] = MapVirtualKeyA(VK_CONTROL, MAPVK_VK_TO_VSC);
		order[scanCodeCount] = 0; // Ctrl имеет order = 0 в массиве Keys
		scanCodeCount++;
	}

	if (io.KeyAlt) {
		scanCodes[scanCodeCount] = MapVirtualKeyA(VK_MENU, MAPVK_VK_TO_VSC);
		order[scanCodeCount] = 1; // Alt имеет order = 1 в массиве Keys
		scanCodeCount++;
	}

	if (io.KeyShift) {
		scanCodes[scanCodeCount] = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
		order[scanCodeCount] = 2; // Shift имеет order = 2 в массиве Keys
		scanCodeCount++;
	}

	// Проверяем остальные клавиши
	for (int i = ImGuiKey_Tab; i <= ImGuiKey_KeypadEqual; i++) {
		if (ImGui::IsKeyDown((ImGuiKey)i)) {
			int vk = 0;
			switch ((ImGuiKey)i) {
				case ImGuiKey_Tab: vk = VK_TAB; break;
				case ImGuiKey_LeftArrow: vk = VK_LEFT; break;
				case ImGuiKey_RightArrow: vk = VK_RIGHT; break;
				case ImGuiKey_UpArrow: vk = VK_UP; break;
				case ImGuiKey_DownArrow: vk = VK_DOWN; break;
				case ImGuiKey_PageUp: vk = VK_PRIOR; break;
				case ImGuiKey_PageDown: vk = VK_NEXT; break;
				case ImGuiKey_Home: vk = VK_HOME; break;
				case ImGuiKey_End: vk = VK_END; break;
				case ImGuiKey_Delete: vk = VK_DELETE; break;
				case ImGuiKey_Backspace: vk = VK_BACK; break;
				case ImGuiKey_Enter: vk = VK_RETURN; break;
				case ImGuiKey_Escape: vk = VK_ESCAPE; break;
				case ImGuiKey_Space: vk = VK_SPACE; break;
				case ImGuiKey_GraveAccent: vk = VK_OEM_3; break;
				case ImGuiKey_Minus: vk = VK_OEM_MINUS; break;
				case ImGuiKey_Equal: vk = VK_OEM_PLUS; break;
				case ImGuiKey_LeftBracket: vk = VK_OEM_4; break;
				case ImGuiKey_RightBracket: vk = VK_OEM_6; break;
				case ImGuiKey_Backslash: vk = VK_OEM_5; break;
				case ImGuiKey_Semicolon: vk = VK_OEM_1; break;
				case ImGuiKey_Apostrophe: vk = VK_OEM_7; break;
				case ImGuiKey_Comma: vk = VK_OEM_COMMA; break;
				case ImGuiKey_Period: vk = VK_OEM_PERIOD; break;
				case ImGuiKey_Slash: vk = VK_OEM_2; break;
				default:
					if (i >= ImGuiKey_A && i <= ImGuiKey_Z)
						vk = 'A' + (i - ImGuiKey_A);
					else if (i >= ImGuiKey_0 && i <= ImGuiKey_9)
						vk = '0' + (i - ImGuiKey_0);
					else if (i >= ImGuiKey_F1 && i <= ImGuiKey_F12)
						vk = VK_F1 + (i - ImGuiKey_F1);
					break;
			}
			if (vk != 0) {
				int imKey = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
				if (scanCodeCount < 4) {
					scanCodes[scanCodeCount] = (unsigned char)imKey;
					order[scanCodeCount] = (unsigned char)GetKeyForScanCode(imKey).order;
					scanCodeCount++;
				}
			}
		}
	}

	unsigned int newHotKey = GetOrderedScanCodes(scanCodes, order);

	if (scanCodeCount) {
		for (size_t i = 0; i < hotkeyCount; i++) {
			if (hotkey[i].functionKeys == newHotKey) {
				if (lastHotKey == newHotKey && !repeat)
					return nullptr;

				lastHotKey = newHotKey;
				return &hotkey[i];
			}
		}
		lastHotKey = 0xFFFFFFFF;
		return nullptr;
	}
	lastHotKey = 0xFFFFFFFF;
	return nullptr;
}
}; // namespace ImHotKey
