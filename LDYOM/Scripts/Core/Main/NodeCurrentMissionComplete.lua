ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeCurrentMissionComplete", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("exit")..' '..ldyom.langt("CoreNodeCurrentMissionComplete");
Node.static.mission = true;

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 5;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0)
	};
end

function Node:draw()
	imgui.imnodes.BeginNode(self.id,self.type)
	
	imgui.imnodes.BeginNodeTitleBar();
	imgui.Text(self.class.static.name);
	if ldyom.getLastNode() == self.id then
		imgui.SameLine(0,0);
		imgui.TextColored(imgui.ImVec4.new(1.0,0.0,0.0,1.0)," \xef\x86\x88");
	end
	imgui.imnodes.EndNodeTitleBar();
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	imgui.Dummy(imgui.ImVec2:new(0,20));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	ldyom.setLastNode(self.id);
	
	ldyom.setMissionStarted(false);
end

ldyom.nodeEditor.addNodeClass("Main",Node);