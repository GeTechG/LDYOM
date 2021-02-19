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
	
	require "LDYOM.Scripts.Core.VariableNode"
	
	require "LDYOM.Scripts.Core.test"
	require "LDYOM.Scripts.Core.Main.NodeStart"
	require "LDYOM.Scripts.Core.Main.NodeMainCycle"
	require "LDYOM.Scripts.Core.Main.NodePrintLog"
	
	require "LDYOM.Scripts.Core.Targets.NodeRunSignal"
	require "LDYOM.Scripts.Core.Targets.NodeCheckTarg"
	require "LDYOM.Scripts.Core.Targets.NodeStartedTarg"
	require "LDYOM.Scripts.Core.Targets.NodeEndedTarg"
	require "LDYOM.Scripts.Core.Targets.NodeSetTarg"
	
	require "LDYOM.Scripts.Core.Storyline.NodeMissionComplete"
	require "LDYOM.Scripts.Core.Storyline.NodeGotoMission"
	require "LDYOM.Scripts.Core.Storyline.NodeEndStoryline"
	require "LDYOM.Scripts.Core.Storyline.NodeActivCheck"
	
	require "LDYOM.Scripts.Core.Actor.NodeShowActor"
	require "LDYOM.Scripts.Core.Actor.NodeApperActor"
	require "LDYOM.Scripts.Core.Actor.NodeHideActor"
	require "LDYOM.Scripts.Core.Actor.NodeDisapperActor"
	require "LDYOM.Scripts.Core.Actor.NodeActorToPed"
	
	print("Core nodes loaded")
end


