ffi = require "ffi"

info = {
	["name"] = "Main nodes"
}

function main()
	require "LDYOM.Scripts.Core.test"
	require "LDYOM.Scripts.Core.Main.NodeStart"
	require "LDYOM.Scripts.Core.Main.NodeMainCycle"
	require "LDYOM.Scripts.Core.Targets.NodeRunSignal"
	require "LDYOM.Scripts.Core.Targets.NodeCheckTarg"
	require "LDYOM.Scripts.Core.Targets.NodeStartedTarg"
	require "LDYOM.Scripts.Core.Targets.NodeSetTarg"
	require "LDYOM.Scripts.Core.Storyline.NodeMissionComplete"
	require "LDYOM.Scripts.Core.Storyline.NodeGotoMission"
	require "LDYOM.Scripts.Core.Storyline.NodeEndStoryline"
	require "LDYOM.Scripts.Core.Storyline.NodeActivCheck"
	require "LDYOM.Scripts.Core.Actor.NodeShowActor"
	require "LDYOM.Scripts.Core.Actor.NodeApperActor"
	require "LDYOM.Scripts.Core.Actor.NodeHideActor"
	require "LDYOM.Scripts.Core.Actor.NodeDisapperActor"
	print("Core nodes loaded")
end


