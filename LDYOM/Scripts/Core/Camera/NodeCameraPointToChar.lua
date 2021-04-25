ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeCameraPointToChar", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeCameraPointToChar");
Node.static.mission = true;

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]",15)),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.void, 1),
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
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text(ldyom.langt("modeCamera"));
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+3].value, ldyom.langMenu["cameraModes"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.ToggleButton(ldyom.langt("movecam"), self.Pins[self.id+4].value);
	else
		imgui.Text(ldyom.langt("movecam"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+5);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local ped = self:getPinValue(self.id+2,data,mission)[0];
	local camera_mode = self:getPinValue(self.id+3,data,mission)[0];
	local movecam = self:getPinValue(self.id+4,data,mission)[0];
	assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");
	ldyom.setLastNode(self.id);
	
	callOpcode(0x0159, {{ped,"int"}, {camera_mode,"int"}, {fif(movecam,1,2),"int"}});
	
	self:callOutputLinks(data, mission, self.id+5);
end

ldyom.nodeEditor.addNodeClass("Camera",Node);