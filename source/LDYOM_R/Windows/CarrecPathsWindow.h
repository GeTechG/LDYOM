#pragma once

#include <boost/uuid/uuid.hpp>

#include "AbstractWindow.h"
#include "../Data/Vehicle.h"

namespace Windows {
	class CarrecPathsWindow : public AbstractWindow {
	private:
		int selectedPath = -1;
		boost::uuids::uuid vehicleUuid{};

	public:
		void selectVehiclePopup(const char *namePopup, bool *open,
		                        const std::function<void(Vehicle *)> &callback);
		void draw() override;
	};
}
