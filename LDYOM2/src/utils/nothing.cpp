#include "nothing.h"
#include <CHud.h>
#include <extensions/ScriptCommands.h>
#include <plugin.h>

void nothing() {
	if (plugin::Command<0x0ADC>("TOP2009")) {
		CHud::SetHelpMessage("~r~I LOVE TWENTY ONE PILOTS", false, false, false);
		CHud::DrawHelpText();
	}

	if (plugin::Command<0x0ADC>("MISSIOMAFIA")) {
		CHud::SetHelpMessage("~w~I MISSIO MAFIA", false, false, false);
		CHud::DrawHelpText();
	}
}