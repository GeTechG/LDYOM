ffi = require "ffi"
bitser = require "LDYOM.Scripts.bitser"
local debug_table = require "LDYOM.Scripts.debug_table"
require "LDYOM.Scripts.baseNode"

info = {
	["name"] = "Main nodes"
}

nodes = {};

function addNode(id,node)
	nodes[id] = node;
end

function serilize(data)
	return bitser.dumps(data);
end

function deserilize(data)
	return bitser.loads(data);
end

function main()
	bitser.registerClass(BaseNode);
	bitser.registerClass(BasePin);
	bitser.registerClass(BaseLink);
	bitser.registerClass(Variable);
	
	ldyom.langMenu["CoreCalcParam"] = ldyom.parseJsonArray(ldyom.langt("CoreCalcParam"));
	
	require "LDYOM.Scripts.Core.VariableNode"
	
	require "LDYOM.Scripts.Core.test"
	require "LDYOM.Scripts.Core.Main.NodeStart"
	require "LDYOM.Scripts.Core.Main.NodeMainCycle"
	require "LDYOM.Scripts.Core.Main.NodePrintLog"
	require "LDYOM.Scripts.Core.Main.NodeNumberToString"
	require "LDYOM.Scripts.Core.Main.NodeBoolToString"
	
	require "LDYOM.Scripts.Core.Math.NodeCalc"
	
	require "LDYOM.Scripts.Core.Targets.NodeRunSignal"
	require "LDYOM.Scripts.Core.Targets.NodeCheckTarg"
	require "LDYOM.Scripts.Core.Targets.NodeStartedTarg"
	require "LDYOM.Scripts.Core.Targets.NodeEndedTarg"
	require "LDYOM.Scripts.Core.Targets.NodeSetTarg"
	
	require "LDYOM.Scripts.Core.Storyline.NodeMissionComplete"
	require "LDYOM.Scripts.Core.Storyline.NodeGotoMission"
	require "LDYOM.Scripts.Core.Storyline.NodeEndStoryline"
	require "LDYOM.Scripts.Core.Storyline.NodeActivCheck"
	
	require "LDYOM.Scripts.Core.Actor.NodeActorToPed"
	require "LDYOM.Scripts.Core.Actor.NodeShowActor"
	require "LDYOM.Scripts.Core.Actor.NodeApperActor"
	require "LDYOM.Scripts.Core.Actor.NodeHideActor"
	require "LDYOM.Scripts.Core.Actor.NodeDisapperActor"
	
	require "LDYOM.Scripts.Core.Car.NodeCarToHandle"
	require "LDYOM.Scripts.Core.Car.NodeShowCar"
	require "LDYOM.Scripts.Core.Car.NodeApperCar"
	require "LDYOM.Scripts.Core.Car.NodeHideCar"
	require "LDYOM.Scripts.Core.Car.NodeDisapperCar"
	
	require "LDYOM.Scripts.Core.Object.NodeObjectToHandle"
	require "LDYOM.Scripts.Core.Object.NodeShowObject"
	require "LDYOM.Scripts.Core.Object.NodeApperObject"
	require "LDYOM.Scripts.Core.Object.NodeHideObject"
	require "LDYOM.Scripts.Core.Object.NodeDisapperObject"
	
	require "LDYOM.Scripts.Core.Particle.NodeParticleToHandle"
	require "LDYOM.Scripts.Core.Particle.NodeShowParticle"
	require "LDYOM.Scripts.Core.Particle.NodeApperParticle"
	require "LDYOM.Scripts.Core.Particle.NodeHideParticle"
	require "LDYOM.Scripts.Core.Particle.NodeDisapperParticle"
	
	require "LDYOM.Scripts.Core.Pickup.NodePickupToHandle"
	require "LDYOM.Scripts.Core.Pickup.NodeShowPickup"
	require "LDYOM.Scripts.Core.Pickup.NodeApperPickup"
	require "LDYOM.Scripts.Core.Pickup.NodeHidePickup"
	require "LDYOM.Scripts.Core.Pickup.NodeDisapperPickup"
	
	require "LDYOM.Scripts.Core.Explosion.NodeExplosionToHandle"
	require "LDYOM.Scripts.Core.Explosion.NodeShowExplosion"
	require "LDYOM.Scripts.Core.Explosion.NodeApperExplosion"
	require "LDYOM.Scripts.Core.Explosion.NodeHideExplosion"
	require "LDYOM.Scripts.Core.Explosion.NodeDisapperExplosion"
	
	require "LDYOM.Scripts.Core.Audio.NodeAudioGet"
	require "LDYOM.Scripts.Core.Audio.NodeShowAudio"
	require "LDYOM.Scripts.Core.Audio.NodeApperAudio"
	require "LDYOM.Scripts.Core.Audio.NodeHideAudio"
	require "LDYOM.Scripts.Core.Audio.NodeDisapperAudio"
	
	print("Core nodes loaded")
end


