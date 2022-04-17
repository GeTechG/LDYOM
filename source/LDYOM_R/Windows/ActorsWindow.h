#pragma once
#include "ListWindow.h"

namespace Windows {
	class ActorsWindow : public ListWindow {
	private:
		std::string getNameList() override;
		std::string getNameOption() override;
		int getListSize() override;
		void createNewElement() override;
		char* getElementName(int i) override;
		void deleteElement(int i) override;
		void drawOptions() override;
	};
}
