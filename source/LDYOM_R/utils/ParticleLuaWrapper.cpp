#include <sol.hpp>
#include "../Data/Particle.h"


void ParticleLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Particle>("Particle");
	clazz["getUuid"] = &Particle::getUuid;
	clazz["getEditorParticle"] = &Particle::getEditorParticle;
	clazz["getProjectParticle"] = &Particle::getProjectParticle;
	clazz["getRotations"] = &Particle::getRotations;
	clazz["getScale"] = &Particle::getScale;
	clazz["getParticleType"] = &Particle::getParticleType;
	clazz["getAttachType"] = &Particle::getAttachType;
	clazz["getAttachUuid"] = &Particle::getAttachUuid;
	clazz["getPedBodeId"] = &Particle::getPedBodeId;
	clazz["getProjectParticleId"] = &Particle::getProjectParticleId;
	clazz["updateLocation"] = &Particle::updateLocation;
	clazz["getName"] = &Particle::getName;
	clazz["getPosition"] = &Particle::getPosition;
	clazz["spawnEditorParticle"] = &Particle::spawnEditorParticle;
	clazz["deleteEditorParticle"] = &Particle::deleteEditorParticle;
	clazz["spawnProjectEntity"] = &Particle::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Particle::deleteProjectEntity;
}
