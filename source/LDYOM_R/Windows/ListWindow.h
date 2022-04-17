#pragma once
#include "IWindow.h"

#include <string>

namespace Windows {
	class ListWindow: public IWindow {
	private:
		void drawList();
	protected:
		int currentElement = -1;

		virtual std::string getNameList() = 0;
		virtual std::string getNameOption() = 0;
		virtual int getListSize() = 0;
		virtual void createNewElement() = 0;
		virtual char* getElementName(int i) = 0;
		virtual void deleteElement(int i) = 0;
		virtual void drawOptions() = 0;
	public:
		void draw() override;
		void unselect();
	};
}
