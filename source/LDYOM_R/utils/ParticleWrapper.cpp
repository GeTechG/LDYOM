#include <sol.hpp>
#include "../Data/Particle.h"


void particleWrapper(sol::state &state) {
	state.new_usertype<Particle>("LDParticle",
	                             sol::no_constructor,
	                             "getUuid", &Particle::getUuid,
	                             "getEditorParticle", &Particle::getEditorParticle,
	                             "getProjectParticle", &Particle::getProjectParticle,
	                             "getRotations", &Particle::getRotations,
	                             "getScale", &Particle::getScale,
	                             "getParticleType", &Particle::getParticleType,
	                             "getAttachType", &Particle::getAttachType,
	                             "getAttachUuid", &Particle::getAttachUuid,
	                             "getPedBodeId", &Particle::getPedBodeId,
	                             "getProjectParticleId", &Particle::getProjectParticleId,
	                             "updateLocation", &Particle::updateLocation,
	                             "getName", &Particle::getName,
	                             "getPosition", [](Particle &particle) {
		                             return std::make_tuple(particle.getPosition()[0], particle.getPosition()[1],
		                                                    particle.getPosition()[2]);
	                             },
	                             "setPosition", [](Particle &particle, const float x, const float y, const float z) {
		                             particle.getPosition()[0] = x;
		                             particle.getPosition()[1] = y;
		                             particle.getPosition()[2] = z;
	                             },
	                             "spawnEditorParticle", &Particle::spawnEditorParticle,
	                             "deleteEditorParticle", &Particle::deleteEditorParticle,
	                             "spawnProjectEntity", &Particle::spawnProjectEntity,
	                             "deleteProjectEntity", &Particle::deleteProjectEntity,
	                             "copy", &Particle::copy
	);
}
