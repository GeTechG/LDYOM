#include "plugin.h"
#include "CLEO.h"
#include "Init.h"
#include <CHud.h>

using namespace plugin;

int gameVersion = -1;
HMODULE hldyom;
char bufferA[128] = { 0 };

Mission* (*getCurrentMission)();

class LCAPI {
public:

	static OpcodeResult WINAPI getIntByOffsetAddress(CScriptThread* thread)
	{
		DWORD address = CLEO_GetIntOpcodeParam(thread);
		address += CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		int* num = reinterpret_cast<int*>(address);
		CLEO_SetIntOpcodeParam(thread, *num);
		result = true;

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI setIntByOffsetAddress(CScriptThread* thread)
	{
		DWORD address = CLEO_GetIntOpcodeParam(thread);
		address += CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		int* num = reinterpret_cast<int*>(address);
		*num = CLEO_GetIntOpcodeParam(thread);
		result = true;

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI getFloatByOffsetAddress(CScriptThread* thread)
	{
		DWORD address = CLEO_GetIntOpcodeParam(thread);
		address += CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		auto* num = reinterpret_cast<float*>(address);
		CLEO_SetFloatOpcodeParam(thread, *num);
		result = true;

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI setFloatByOffsetAddress(CScriptThread* thread)
	{
		DWORD address = CLEO_GetIntOpcodeParam(thread);
		address += CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		auto* num = reinterpret_cast<float*>(address);
		*num = CLEO_GetFloatOpcodeParam(thread);
		result = true;

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI getStringByOffsetAddress(CScriptThread* thread)
	{
		DWORD address = CLEO_GetIntOpcodeParam(thread);
		address += CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		char* string = reinterpret_cast<char*>(address);
		CLEO_WriteStringOpcodeParam(thread, string);
		result = true;

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI setStringByOffsetAddress(CScriptThread* thread)
	{
		DWORD address = CLEO_GetIntOpcodeParam(thread);
		address += CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		char* string = reinterpret_cast<char*>(address);
		if (string)
		{
			const std::string text = CLEO_ReadStringOpcodeParam(thread, string, 129);
			result = true;
		}

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}
	
    static OpcodeResult WINAPI getActor(CScriptThread* thread) {
		Mission* mission = getCurrentMission();
		const DWORD idx = CLEO_GetIntOpcodeParam(thread);
		bool result = false;
		if (idx < mission->list_actors.size())
		{
			CLEO_SetIntOpcodeParam(thread, reinterpret_cast<DWORD>(mission->list_actors.at(idx)));
			result = true;
		} else
		{
			CLEO_SetIntOpcodeParam(thread, 0);
		}
		CLEO_SetThreadCondResult(thread, result);
    	
        return OR_CONTINUE;
    }

	static OpcodeResult WINAPI getTarget(CScriptThread* thread) {
		Mission* mission = getCurrentMission();
		const DWORD idx = CLEO_GetIntOpcodeParam(thread);
		bool result = false;
		if (idx < mission->list_targets.size())
		{
			CLEO_SetIntOpcodeParam(thread, reinterpret_cast<DWORD>(mission->list_targets.at(idx)));
			result = true;
		} else
		{
			CLEO_SetIntOpcodeParam(thread, 0);
		}
		CLEO_SetThreadCondResult(thread, result);

		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI getElementVector(CScriptThread* thread) {
		const DWORD type = CLEO_GetIntOpcodeParam(thread);
		const DWORD address = CLEO_GetIntOpcodeParam(thread);
		const DWORD idx = CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		switch (type)
		{
		case 0: {
			auto* int_vector = reinterpret_cast<std::vector<int>*>(address);
			CLEO_SetIntOpcodeParam(thread, int_vector->at(idx));
			break;
		}
		case 1: {
			auto* float_vector = reinterpret_cast<std::vector<float>*>(address);
			CLEO_SetFloatOpcodeParam(thread, float_vector->at(idx));
			break;
		}
		case 2: {
			auto* char_vector = reinterpret_cast<std::vector<char*>*>(address);
			CLEO_WriteStringOpcodeParam(thread, char_vector->at(idx));
			break;
		}
		case 3: {
			auto* string_vector = reinterpret_cast<std::vector<string>*>(address);
			CLEO_WriteStringOpcodeParam(thread, string_vector->at(idx).c_str());
			break;
		}
		case 4: {
			auto* dialog_vector = reinterpret_cast<std::vector<Dialog>*>(address);
			CLEO_SetIntOpcodeParam(thread, reinterpret_cast<DWORD>(&dialog_vector->at(idx)));
			break;
		}
		default: {
			CLEO_SkipOpcodeParams(thread, 1);
			break;
		}
		}
		result = true;

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}

	static OpcodeResult WINAPI setElementVector(CScriptThread* thread) {
		const DWORD type = CLEO_GetIntOpcodeParam(thread);
		const DWORD address = CLEO_GetIntOpcodeParam(thread);
		const DWORD idx = CLEO_GetIntOpcodeParam(thread);
		bool result = false;

		switch (type)
		{
		case 0: {
			auto* int_vector = reinterpret_cast<std::vector<int>*>(address);
			int_vector->at(idx) = CLEO_GetIntOpcodeParam(thread);
			break;
		}
		case 1: {
			auto* float_vector = reinterpret_cast<std::vector<float>*>(address);
			float_vector->at(idx) = CLEO_GetFloatOpcodeParam(thread);
			break;
		}
		case 2: {
			auto* char_vector = reinterpret_cast<std::vector<char*>*>(address);
			CLEO_ReadStringOpcodeParam(thread, char_vector->at(idx), 129);
			break;
		}
		case 3: {
			auto* string_vector = reinterpret_cast<std::vector<string>*>(address);
			CLEO_ReadStringOpcodeParam(thread, string_vector->at(idx).data(), 129);
			break;
		}
		case 4: {
			auto* dialog_vector = reinterpret_cast<std::vector<Dialog>*>(address);
			dialog_vector->at(idx) = *reinterpret_cast<Dialog*>(CLEO_GetIntOpcodeParam(thread));
			break;
		}
		default: {
			CLEO_SkipOpcodeParams(thread, 1);
			break;
		}
		}
		result = true;

		CLEO_SetThreadCondResult(thread, result);
		return OR_CONTINUE;
	}

    LCAPI() {

		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, "Outdated CLEO. Update your CLEO Library.", "LCAPI.cleo", MB_ICONERROR);
			return;
		}
		gameVersion = GetGameVersion();
		if (gameVersion != GAME_10US_HOODLUM && gameVersion != GAME_10US_COMPACT) {
			MessageBox(HWND_DESKTOP, "CLEO+ is only compatible with EXE 1.0 US (Hoodlum or Compact)", "LCAPI.cleo", MB_ICONERROR);
			return;
		}

		Events::initRwEvent.after += []
		{
			hldyom = GetModuleHandle("LDYOM_ASI.SA.asi");
			if (hldyom)
			{
				getCurrentMission = reinterpret_cast<Mission*(*)()>(GetProcAddress(hldyom, "getCurrentMissionPtr"));
				
				CLEO_RegisterOpcode(0x1000, getIntByOffsetAddress);
				CLEO_RegisterOpcode(0x1001, setIntByOffsetAddress);
				CLEO_RegisterOpcode(0x1002, getFloatByOffsetAddress);
				CLEO_RegisterOpcode(0x1003, setFloatByOffsetAddress);
				CLEO_RegisterOpcode(0x1004, getStringByOffsetAddress);
				CLEO_RegisterOpcode(0x1005, setStringByOffsetAddress);
				CLEO_RegisterOpcode(0x1006, getTarget);
				CLEO_RegisterOpcode(0x1007, getActor);
				CLEO_RegisterOpcode(0x1008, getElementVector);
				CLEO_RegisterOpcode(0x1009, setElementVector);
			}
		};
    }
} lCAPI;
