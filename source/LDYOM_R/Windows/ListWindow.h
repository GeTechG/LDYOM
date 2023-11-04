#pragma once
#include "AbstractWindow.h"

#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ProjectsService.h"

namespace Windows {
	class ListWindow : public AbstractWindow {
	private:
		ImVec2 drawList();

	protected:
		int currentElement = -1;
		bool dragsItems_ = false;

		ListWindow() {
			ProjectsService::getInstance().getCurrentProject().onChangedScene().connect([&] {
				this->currentElement = -1;
			});
		}

		virtual std::string getNameList() = 0;
		virtual std::string getNameOption() = 0;
		virtual int getListSize() = 0;
		virtual void onButtonCreateNewElement();
		virtual void createNewElement() = 0;
		virtual void createNewElementFrom(int i) = 0;
		virtual char* getElementName(int i) = 0;
		virtual void deleteElement(int i) = 0;
		virtual void drawOptions() = 0;
		virtual void swapItems(int firstItem, int secondItem) {}
		virtual void drawListWindow() {}

	public:
		void draw() override;
		virtual void selectElement(int i);
		int getElement();
		void unselect();
	};
}
