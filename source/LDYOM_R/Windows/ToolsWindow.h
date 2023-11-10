#pragma once
#include <boost/uuid/uuid.hpp>

#include "AbstractWindow.h"

namespace Windows {
	class ToolsWindow : public AbstractWindow {
	private:
		bool enexMarker = false;
		boost::uuids::uuid actorUuid_{};
		boost::uuids::uuid vehicleUuid_{};
		boost::uuids::uuid objectUuid_{};

	public:
		void draw() override;
	};
}
