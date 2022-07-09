#pragma once
#include "ListWindow.h"

namespace Windows {
	class PyrotechnicsWindow : public ListWindow {
	public:
		std::string getNameList() override;
		std::string getNameOption() override;
		int getListSize() override;
		void createNewElement() override;
		char* getElementName(int i) override;
		void deleteElement(int i) override;
		void drawOptions() override;
		void close() override;
		void open() override;
	};
}
