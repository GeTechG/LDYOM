ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeComparingNumber", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("fork")..' '..ldyom.langt("CoreNodeComparingNumber");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 1;
	self.comparingType = ffi.new("int[1]", 0);
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.boolean, 1, ffi.new("bool[1]")),
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
	if (self.Pins[self.id+1].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.InputFloat("", self.Pins[self.id+1]);
	else
		imgui.Text(ldyom.langt("value").." 1");
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+2);
	imgui.SetNextItemWidth(150);
	imgui.ComboVecStr("", self.comparingType, ldyom.langMenu["compaingTypes"]);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	if (self.Pins[self.id+3].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.InputFloat("", self.Pins[self.id+3]);
	else
		imgui.Text(ldyom.langt("value").." 2");
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+4);
	imgui.Indent(120);
	imgui.Text(ldyom.langt("result"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local value1 = self:getPinValue(self.id+1,data,mission)[0];
	local value2 = self:getPinValue(self.id+3,data,mission)[0];
	local _type = self.comparingType[0];
	ldyom.setLastNode(self.id);
	
	if (_type == 0) then
		self.Pins[self.id+4].value[0] = value1 == value2;
	elseif (_type == 1) then
		self.Pins[self.id+4].value[0] = value1 ~= value2;
	elseif (_type == 2) then
		self.Pins[self.id+4].value[0] = value1 > value2;
	elseif (_type == 3) then
		self.Pins[self.id+4].value[0] = value1 >= value2;
	elseif (_type == 4) then
		self.Pins[self.id+4].value[0] = value1 < value2;
	elseif (_type == 5) then
		self.Pins[self.id+4].value[0] = value1 <= value2;
	end
end

ldyom.nodeEditor.addNodeClass("Main",Node);