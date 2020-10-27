#pragma once

#include "samp.h"

bool SAMP::IsInitialized()
{
	g_SAMP = *(stSAMP**)(dwSAMPAddr + SAMP_INFO_OFFSET);
	if (g_SAMP == nullptr) return false;
	g_Chat = *(stChatInfo**)(dwSAMPAddr + SAMP_CHAT_INFO_OFFSET);
	if (g_Chat == nullptr) return false;
	g_Input = *(stInputInfo**)(dwSAMPAddr + SAMP_CHAT_INPUT_INFO_OFFSET);
	if (g_Input == nullptr) return false;

	g_Vehicles = g_SAMP->pPools->pVehicle;
	g_Players = g_SAMP->pPools->pPlayer;

	isInited = true;

	return true;
};

void SAMP::AddChatMessage(D3DCOLOR cColor, char *szMsg)
{
	if (g_Chat == nullptr)
		return;

	void(__thiscall *AddToChatWindowBuffer) (const void *_this, int iType, char *szText, char *szPrefix, DWORD cColor, DWORD cPrefixColor) =
		(void(__thiscall *) (const void *, int, char *, char *, DWORD, DWORD)) (dwSAMPAddr + SAMP_FUNC_ADDTOCHATWND);

	if (szMsg == NULL)
		return;

	va_list ap;
	char tmp[512];
	memset(tmp, 0, 512);
	va_start(ap, szMsg);
	vsnprintf(tmp, sizeof(tmp) - 1, szMsg, ap);
	va_end(ap);

	return AddToChatWindowBuffer((void *) g_Chat, 8, tmp, NULL, cColor, 0x00);
}

void SAMP::registerChatCommand(char *szCmd, CMDPROC pFunc)
{
	if (g_Input == nullptr)
		return;

	void(__thiscall *AddClientCommand) (const void *_this, char *szCommand, CMDPROC pFunc) =
		(void(__thiscall *) (const void *, char *, CMDPROC)) (dwSAMPAddr + SAMP_FUNC_ADDCLIENTCMD);

	if (szCmd == NULL)
		return;

	return AddClientCommand(g_Input, szCmd, pFunc);
}

void SAMP::ToggleCursor(bool bMode)
{
	if (getInput()->iInputEnabled) return;

	void* obj = *(void**)(dwSAMPAddr + SAMP_INFO_OFFSET); 
	((void(__thiscall*) (void*, int, bool)) (dwSAMPAddr + SAMP_TOGGLECURSOR))(obj, bMode ? 3 : 0, !bMode);
	if (!bMode) ((void(__thiscall*) (void*)) (dwSAMPAddr + SAMP_CURSORUNLOCKACTORCAM))(obj);
}