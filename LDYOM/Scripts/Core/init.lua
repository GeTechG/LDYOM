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

function fif(cond,truee,falsee)
	if cond then
		return truee;
	else
		return falsee;
	end
end

function clamp(val, lower, upper)
    assert(val and lower and upper, "not very useful error message here")
    if lower > upper then lower, upper = upper, lower end -- swap if boundaries supplied the wrong way
    return math.max(lower, math.min(upper, val))
end

function main()
	bitser.registerClass(BaseNode);
	bitser.registerClass(BasePin);
	bitser.registerClass(BaseLink);
	bitser.registerClass(Variable);
	
	ldyom.langMenu["CoreCalcParam"] = ldyom.parseJsonArray(ldyom.langt("CoreCalcParam"));
	ldyom.langMenu["CoreTypeMovingObject"] = ldyom.parseJsonArray(ldyom.langt("CoreTypeMovingObject"));
	ldyom.langMenu["cameraModes"] = ldyom.parseJsonArray(ldyom.langt("cameraModes"));
	ldyom.langMenu["compaingTypes"] = ldyom.parseJsonArray(ldyom.langt("compaingTypes"));
	ldyom.langMenu["typeLocate"] = ldyom.parseJsonArray(ldyom.langt("typeLocate"));
	ldyom.langMenu["CoreTurretDirection"] = ldyom.parseJsonArray(ldyom.langt("CoreTurretDirection"));
	ldyom.langMenu["target_type"] = ldyom.parseJsonArray(ldyom.langt("target_type"));
	
	callOpcode(0x0ADF,{{"HENAA","string"}, {GXTEncode(UTF8_to_CP1251(ldyom.langt("HENAA"))),"string"}});
	callOpcode(0x0ADF,{{"HVIEW","string"}, {GXTEncode(UTF8_to_CP1251(ldyom.langt("HVIEW"))),"string"}});
	callOpcode(0x0ADF,{{"HAPA","string"}, {GXTEncode(UTF8_to_CP1251(ldyom.langt("HAPA"))),"string"}});
	
	require "LDYOM.Scripts.Core.VariableNode"
	
	require "LDYOM.Scripts.Core.Main.NodeBranch"
	require "LDYOM.Scripts.Core.Main.NodeBoolInvert"
	require "LDYOM.Scripts.Core.Main.NodeStart"
	require "LDYOM.Scripts.Core.Main.NodeCurrentMissionComplete"
	require "LDYOM.Scripts.Core.Main.NodeMainCycle"
	require "LDYOM.Scripts.Core.Main.NodePrintLog"
	require "LDYOM.Scripts.Core.Main.NodeNumberToString"
	require "LDYOM.Scripts.Core.Main.NodeBoolToString"
	require "LDYOM.Scripts.Core.Main.NodeWaitUntil"
	require "LDYOM.Scripts.Core.Main.NodeComparingNumber"
	require "LDYOM.Scripts.Core.Main.NodeComparingString"
	
	require "LDYOM.Scripts.Core.Math.NodeCalc"
	
	require "LDYOM.Scripts.Core.Targets.NodeRunSignal"
	require "LDYOM.Scripts.Core.Targets.NodeCheckTarg"
	require "LDYOM.Scripts.Core.Targets.NodeStartedTarg"
	require "LDYOM.Scripts.Core.Targets.NodeEndedTarg"
	require "LDYOM.Scripts.Core.Targets.NodeSetTarg"
	require "LDYOM.Scripts.Core.Targets.NodeTargetCycle"
	
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
	
	require "LDYOM.Scripts.Core.Train.NodeTrainToHandle"
	require "LDYOM.Scripts.Core.Train.NodeShowTrain"
	require "LDYOM.Scripts.Core.Train.NodeApperTrain"
	require "LDYOM.Scripts.Core.Train.NodeHideTrain"
	require "LDYOM.Scripts.Core.Train.NodeDisapperTrain"
	
	require "LDYOM.Scripts.Core.Object.NodeObjectToHandle"
	require "LDYOM.Scripts.Core.Object.NodeShowObject"
	require "LDYOM.Scripts.Core.Object.NodeApperObject"
	require "LDYOM.Scripts.Core.Object.NodeHideObject"
	require "LDYOM.Scripts.Core.Object.NodeDisapperObject"
	require "LDYOM.Scripts.Core.Object.NodeMoveObject"
	require "LDYOM.Scripts.Core.Object.NodeTriggerMove"
	require "LDYOM.Scripts.Core.Object.NodeGetPosObject"
	require "LDYOM.Scripts.Core.Object.NodeSlideObject"
	require "LDYOM.Scripts.Core.Object.NodeObjectIsOnScreen"
	require "LDYOM.Scripts.Core.Object.NodeObjectHasBeenDamaged"
	require "LDYOM.Scripts.Core.Object.NodeObjectIsInWater"
	require "LDYOM.Scripts.Core.Object.NodeIsObjectInArea"
	require "LDYOM.Scripts.Core.Object.NodeObjectHasBeenPhotographed"
	require "LDYOM.Scripts.Core.Object.NodeObjectHasBeenDamagedByWeapon"
	require "LDYOM.Scripts.Core.Object.NodeObjectLocateInCoord"
	
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
	require "LDYOM.Scripts.Core.Pickup.NodeGetPosPickup"
	
	require "LDYOM.Scripts.Core.Explosion.NodeExplosionToHandle"
	require "LDYOM.Scripts.Core.Explosion.NodeShowExplosion"
	require "LDYOM.Scripts.Core.Explosion.NodeApperExplosion"
	require "LDYOM.Scripts.Core.Explosion.NodeHideExplosion"
	require "LDYOM.Scripts.Core.Explosion.NodeDisapperExplosion"
	require "LDYOM.Scripts.Core.Explosion.NodeGetPosFire"
	
	require "LDYOM.Scripts.Core.Audio.NodeAudioGet"
	require "LDYOM.Scripts.Core.Audio.NodeShowAudio"
	require "LDYOM.Scripts.Core.Audio.NodeApperAudio"
	require "LDYOM.Scripts.Core.Audio.NodeHideAudio"
	require "LDYOM.Scripts.Core.Audio.NodeDisapperAudio"
	
	require "LDYOM.Scripts.Core.Ped.NodePlayAnim"
	require "LDYOM.Scripts.Core.Ped.NodePedPath"
	require "LDYOM.Scripts.Core.Ped.NodePedPathOnCar"
	require "LDYOM.Scripts.Core.Ped.NodePedExitCar"
	require "LDYOM.Scripts.Core.Ped.NodePedChaseCar"
	require "LDYOM.Scripts.Core.Ped.NodePedEnterCar"
	require "LDYOM.Scripts.Core.Ped.NodePedGotoPed"
	require "LDYOM.Scripts.Core.Ped.NodePedGiveWeapon"
	require "LDYOM.Scripts.Core.Ped.NodePedAddAmmo"
	require "LDYOM.Scripts.Core.Ped.NodePedTakeWeapons"
	require "LDYOM.Scripts.Core.Ped.NodePedChangeSkin"
	require "LDYOM.Scripts.Core.Ped.NodeGetPosChar"
	require "LDYOM.Scripts.Core.Ped.NodeSetPosChar"
	require "LDYOM.Scripts.Core.Ped.NodePedSetBleeding"
	require "LDYOM.Scripts.Core.Ped.NodePedSetMoney"
	require "LDYOM.Scripts.Core.Ped.NodePedSetIsChrisCriminal"
	require "LDYOM.Scripts.Core.Ped.NodePedTurretModeCar"
	require "LDYOM.Scripts.Core.Ped.NodePedTurretModeObject"
	require "LDYOM.Scripts.Core.Ped.NodePedTurretModeOff"
	require "LDYOM.Scripts.Core.Ped.NodePedGetCurrentWeapon"
	require "LDYOM.Scripts.Core.Ped.NodeGetPedHealth"
	require "LDYOM.Scripts.Core.Ped.NodeIsInArea"
	require "LDYOM.Scripts.Core.Ped.NodePedLocateInCoord"
	require "LDYOM.Scripts.Core.Ped.NodePedLocateInPed"
	require "LDYOM.Scripts.Core.Ped.NodeIsPedInVehicle"
	require "LDYOM.Scripts.Core.Ped.NodeIsPedInVehicleModel"
	require "LDYOM.Scripts.Core.Ped.NodeIsPedDead"
	require "LDYOM.Scripts.Core.Ped.NodeIsPedInZone"
	require "LDYOM.Scripts.Core.Ped.NodePedTouchingObjectOnFoot"
	require "LDYOM.Scripts.Core.Ped.NodePedIsOnScreen"
	require "LDYOM.Scripts.Core.Ped.NodePedIsShooting"
	require "LDYOM.Scripts.Core.Ped.NodePedExplodeHead"
	require "LDYOM.Scripts.Core.Ped.NodePedIsSittingInCar"
	require "LDYOM.Scripts.Core.Ped.NodeHasGotWeapon"
	require "LDYOM.Scripts.Core.Ped.NodePedIsInWater"
	require "LDYOM.Scripts.Core.Ped.NodePedHasBeenPhotographed"
	require "LDYOM.Scripts.Core.Ped.NodePedHasBeenDamagedByChar"
	require "LDYOM.Scripts.Core.Ped.NodePedHasBeenDamagedByCar"
	
	require "LDYOM.Scripts.Core.Vehicle.NodeOpenDoors"
	require "LDYOM.Scripts.Core.Vehicle.NodeLockVehicle"
	require "LDYOM.Scripts.Core.Vehicle.NodeGetPosVehicle"
	require "LDYOM.Scripts.Core.Vehicle.NodeVehicleGetHealth"
	
	require "LDYOM.Scripts.Core.Player.NodePlayerToPed"
	
	require "LDYOM.Scripts.Core.Camera.NodeGetPosActiveCam"
	require "LDYOM.Scripts.Core.Camera.NodeInterpolationParameters"
	require "LDYOM.Scripts.Core.Camera.NodeStaticCutscene"
	require "LDYOM.Scripts.Core.Camera.NodeCameraPointToPos"
	require "LDYOM.Scripts.Core.Camera.NodeCameraPointToVehicle"
	require "LDYOM.Scripts.Core.Camera.NodeCameraPointToChar"
	require "LDYOM.Scripts.Core.Camera.NodeRestoreCamera"
	require "LDYOM.Scripts.Core.Camera.NodeRestoreCameraJumpcut"
	require "LDYOM.Scripts.Core.Camera.NodeAttachCamToVehicle"
	require "LDYOM.Scripts.Core.Camera.NodeAttachCamToChar"
	require "LDYOM.Scripts.Core.Camera.NodeAttachCamToVehicleLookVehicle"
	require "LDYOM.Scripts.Core.Camera.NodeAttachCamToVehicleLookChar"
	require "LDYOM.Scripts.Core.Camera.NodeAttachCamToCharLookChar"
	require "LDYOM.Scripts.Core.Camera.NodeGetFOV"
	require "LDYOM.Scripts.Core.Camera.NodeLerpFOV"
	require "LDYOM.Scripts.Core.Camera.NodeSetCinemaCamera"
	require "LDYOM.Scripts.Core.Camera.NodeSetDrawCrosshair"
	require "LDYOM.Scripts.Core.Camera.NodeSetPhotocameraMode"
	require "LDYOM.Scripts.Core.Camera.NodeSetWidescreen"
	require "LDYOM.Scripts.Core.Camera.NodeShakeSimulation"
	
	require "LDYOM.Scripts.Core.World.NodeIsAreaOccupied"
	require "LDYOM.Scripts.Core.World.NodeAddTimer"
	require "LDYOM.Scripts.Core.World.NodeRemoveTimer"
	require "LDYOM.Scripts.Core.World.NodeGetTimerTime"
	require "LDYOM.Scripts.Core.World.NodeAddCounter"
	require "LDYOM.Scripts.Core.World.NodeRemoveCounter"
	
	require "LDYOM.Scripts.Core.VisualEffect.NodeShowVisualEffect"
	require "LDYOM.Scripts.Core.VisualEffect.NodeApperVisualEffect"
	require "LDYOM.Scripts.Core.VisualEffect.NodeHideVisualEffect"
	require "LDYOM.Scripts.Core.VisualEffect.NodeDisapperVisualEffect"
	
	print("Core nodes loaded")
end


