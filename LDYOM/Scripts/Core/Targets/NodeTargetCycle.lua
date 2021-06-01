ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeTargetCycle", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("loop")..' '..ldyom.langt("CoreNodeTargetCycle");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 2;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.void, 1),
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
	local names = ldyom.namesTargets;
	imgui.Text(ldyom.langt("target"));
	if (self.Pins[self.id+1].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.ComboVecChars("",self.Pins[self.id+1].value,names);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+2);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local target_id = self:getPinValue(self.id+1,data,mission)[0];
	ldyom.setLastNode(self.id);
	ldyom.targetCycle(self, target_id, self.id+2, data, mission);
end

ldyom.nodeEditor.addNodeClass("Target",Node);