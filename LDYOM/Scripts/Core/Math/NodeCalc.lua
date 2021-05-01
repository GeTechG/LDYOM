ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeCalc", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeCalc");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.mode = ffi.new("int[1]", 0);
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("float[1]", 0)),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("float[1]", 0)),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 1, ffi.new("float[1]", 0)),
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
	
	imgui.imnodes.BeginStaticAttribute(self.id+4);
	imgui.SetNextItemWidth(150);
	imgui.ComboVecStr("",self.mode,ldyom.langMenu["CoreCalcParam"]);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	if (self.Pins[self.id+1].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.InputFloat("",self.Pins[self.id+1].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	if (self.Pins[self.id+2].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.InputFloat("",self.Pins[self.id+2].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+3);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local value1 = self:getPinValue(self.id+1,data,mission)[0];
	local value2 = self:getPinValue(self.id+2,data,mission)[0];
	ldyom.setLastNode(self.id);
	if self.mode[0] == 0 then
		self.Pins[self.id+3].value[0] = value1 + value2;
	elseif self.mode[0] == 1 then
		self.Pins[self.id+3].value[0] = value1 - value2;
	elseif self.mode[0] == 2 then
		self.Pins[self.id+3].value[0] = value1 * value2;
	elseif self.mode[0] == 3 then
		self.Pins[self.id+3].value[0] = value1 / value2;
	end
end

ldyom.nodeEditor.addNodeClass("CoreMath",Node);