#include <sol.hpp>
#include <boost/uuid/uuid.hpp>

#include "utils.h"
#include "utilsRender.h"

#include "../Data/Actor.h"
#include "../Data/Audio.h"
#include "../Data/BaseObjective.h"
#include "../Data/Checkpoint.h"
#include "../Data/Object.h"
#include "../Data/Particle.h"
#include "../Data/Pickup.h"
#include "../Data/Pyrotechnics.h"
#include "../Data/Train.h"
#include "../Data/Vehicle.h"
#include "../Data/VisualEffect.h"

void indexUuidWrapper(sol::state &state) {
	state.set("IncorrectHighlight", IncorrectHighlight);

	auto indexByUuuidGeneric = []<typename T>(const std::vector<std::unique_ptr<T>> &vector, const sol::object &uuid) {
		boost::uuids::uuid uuid_{};
		memcpy(uuid_.data, uuid.pointer(), sizeof uuid_.data);
		return utils::indexByUuid(vector, uuid_);
	};

	state.set("indexByUuid", sol::overload(
		          [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Actor>> &vector, const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Audio>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Checkpoint>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Object>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<BaseObjective>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Particle>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Pickup>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Pyrotechnics>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Train>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Vehicle>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<VisualEffect>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }));
}
