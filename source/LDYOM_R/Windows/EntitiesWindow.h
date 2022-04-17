#pragma once
#include "IWindow.h"

namespace Windows
{
	class EntitiesWindow : public IWindow
	{
		void draw() override;
	};
}
