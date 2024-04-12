#pragma once
#include "ListWindow.h"

namespace Windows {
	class CameraPathsWindow : public ListWindow {
	private:
		int currentPoint = -1;

	public:
		std::string getNameList() override;
		std::string getNameOption() override;
		int getListSize() override;
		void createNewElement() override;
		void createNewElementFrom(int i) override;
		std::string& getElementName(int i) override;
		void deleteElement(int i) override;
		void drawOptions() override;
		void close() override;
		void open() override;
	};
}
