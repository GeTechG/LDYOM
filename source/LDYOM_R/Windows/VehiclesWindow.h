#pragma once
#include "ListWindow.h"
#include "PopupVehicleSelector.h"

namespace Windows {
	class VehiclesWindow : public ListWindow {
	private:
		PopupVehicleSelector popupVehicleSelector_;
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
