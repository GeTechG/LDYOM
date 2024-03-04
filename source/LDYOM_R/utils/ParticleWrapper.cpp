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
	                             "getPosition", &Particle::getPosition,
	                             "spawnEditorParticle", &Particle::spawnEditorParticle,
	                             "deleteEditorParticle", &Particle::deleteEditorParticle,
	                             "spawnProjectEntity", &Particle::spawnProjectEntity,
	                             "deleteProjectEntity", &Particle::deleteProjectEntity,
	                             "copy", &Particle::copy
	);
}
