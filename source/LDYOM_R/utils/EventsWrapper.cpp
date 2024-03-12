#include <sol.hpp>

void eventsWrapper(sol::state &state) {
	auto ldTable = state["ld"].get<sol::table>();
	auto eventsTable = ldTable.create("events");
	eventsTable.create("onStartProject");
	eventsTable.create("onStartScene");
	eventsTable.create("mainLoop");
	eventsTable.create("onSaveProject");
	eventsTable.create("onLoadProject");
	eventsTable.create("onStartObjective");
	eventsTable.create("onCreateNewProject");
}
