ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeLerpFOV", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeLerpFOV");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",70)),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",120)),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.void, 1),
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
	imgui.Text(ldyom.langt("start"));
	if not self.Pins[self.id+2].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+2].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text(ldyom.langt("end"));
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+3].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("time"));
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+5);
	imgui.ToggleButton(ldyom.langt("linear"), self.Pins[self.id+5].value);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+6);
	imgui.ToggleButton(ldyom.langt("lock"), self.Pins[self.id+6].value);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+7);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local start = self:getPinValue(self.id+2,data,mission)[0];
	local _end = self:getPinValue(self.id+3,data,mission)[0];
	local _time = self:getPinValue(self.id+4,data,mission)[0] * 1000;
	local linear = self:getPinValue(self.id+5,data,mission)[0];
	local lock = self:getPinValue(self.id+6,data,mission)[0];
	ldyom.setLastNode(self.id);

	callOpcode(0x0922, {{start, "float"}, {_end, "float"}, {_time, "int"}, {fif(linear,0,1), "int"}});
	callOpcode(0x0931, {{lock, "bool"}});
	
	self:callOutputLinks(data, mission, self.id+7);
end

ldyom.nodeEditor.addNodeClass("Camera",Node);