#pragma once
#include "ListWindow.h"

namespace Windows {
	class ObjectivesWindow: public ListWindow {
	protected:
		std::string getNameList() override;
		std::string getNameOption() override;
		int getListSize() override;
		void createNewElement() override;
		char* getElementName(int i) override;
		void deleteElement(int i) override;
		void drawOptions() override;
		void selectElement(int i) override;
		void onButtonCreateNewElement() override;
		void drawListWindow() override;
		void swapItems(int firstItem, int secondItem) override;
		void close() override;
	public:
		ObjectivesWindow();
	};
}
