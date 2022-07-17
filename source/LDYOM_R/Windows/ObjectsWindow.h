#pragma once
#include "ListWindow.h"
#include "ObjectSelectorPopup.h"
#include "ObjectViewerPopup.h"

namespace Windows {
	class ObjectsWindow: public ListWindow {
	private:
		ObjectViewerPopup objectViewerPopup_;
		ObjectSelectorPopup objectSelectorPopup_;
	public:
		std::string getNameList() override;
		std::string getNameOption() override;
		int getListSize() override;
		void createNewElement() override;
		void createNewElementFrom(int i) override;
		char* getElementName(int i) override;
		void deleteElement(int i) override;
		void drawOptions() override;
		void close() override;
		void open() override;
	};
}
