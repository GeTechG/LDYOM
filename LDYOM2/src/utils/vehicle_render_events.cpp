#include "vehicle_render_events.h"
#include "vehicle_renderer.h"

namespace VehicleRenderEvents {

// Define the post-render event
decltype(vehicleResetAfterRender) vehicleResetAfterRender;

void initializeEvents() {
	// Hook into vehicle render event for processing custom colors
	plugin::Events::vehicleRenderEvent.before += [](CVehicle* veh) {
		if (veh) {
			VehicleRenderer::instance().processRender(veh);
		}
	};

	// Hook into vehicle reset after render for restoring original colors
	vehicleResetAfterRender += [](CVehicle* veh) {
		if (veh) {
			VehicleRenderer::instance().postRender(veh);
		}
	};
}

} // namespace VehicleRenderEvents
