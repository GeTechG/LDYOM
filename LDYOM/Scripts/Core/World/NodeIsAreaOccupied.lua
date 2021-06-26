ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeIsAreaOccupied", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("fork")..' '..ldyom.langt("CoreNodeIsAreaOccupied");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 1;
	local x = ffi.new("float[1]");
	local y = ffi.new("float[1]");
	local z = ffi.new("float[1]");
	callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",x[0])),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",y[0])),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",z[0])),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",x[0])),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",y[0])),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",z[0])),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+10] = BasePin:new(self.id+10,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+11] = BasePin:new(self.id+11,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+12] = BasePin:new(self.id+12,imgui.imnodes.PinType.boolean, 1, ffi.new("bool[1]")),
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
	imgui.Text("x1");
	if not self.Pins[self.id+1].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+1].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text("y1");
	if not self.Pins[self.id+2].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+2].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text("z1");
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+3].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+14);
	if imgui.Button(ldyom.langt("playerCoordinates"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {self.Pins[self.id+1].value,"float*"}, {self.Pins[self.id+2].value,"float*"}, {self.Pins[self.id+3].value,"float*"}});
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text("x2");
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text("y2");
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+5].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	imgui.Text("z2");
	if not self.Pins[self.id+6].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+6].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+15);
	if imgui.Button(ldyom.langt("playerCoordinates"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {self.Pins[self.id+4].value,"float*"}, {self.Pins[self.id+5].value,"float*"}, {self.Pins[self.id+6].value,"float*"}});
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+7);
	if not self.Pins[self.id+7].link then
		imgui.ToggleButton(ldyom.langt("solid"), self.Pins[self.id+7].value);
	else
		imgui.Text(ldyom.langt("solid"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+8);
	if not self.Pins[self.id+8].link then
		imgui.ToggleButton(ldyom.langt("vehicle"), self.Pins[self.id+8].value);
	else
		imgui.Text(ldyom.langt("vehicle"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+9);
	if not self.Pins[self.id+9].link then
		imgui.ToggleButton(ldyom.langt("ped"), self.Pins[self.id+9].value);
	else
		imgui.Text(ldyom.langt("ped"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+10);
	if not self.Pins[self.id+10].link then
		imgui.ToggleButton(ldyom.langt("object"), self.Pins[self.id+10].value);
	else
		imgui.Text(ldyom.langt("object"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+11);
	if not self.Pins[self.id+11].link then
		imgui.ToggleButton(ldyom.langt("particle"), self.Pins[self.id+11].value);
	else
		imgui.Text(ldyom.langt("particle"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+12);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("result"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)

	local x1 = self:getPinValue(self.id+1,data,mission)[0];
	local y1 = self:getPinValue(self.id+2,data,mission)[0];
	local z1 = self:getPinValue(self.id+3,data,mission)[0];
	local x2 = self:getPinValue(self.id+4,data,mission)[0];
	local y2 = self:getPinValue(self.id+5,data,mission)[0];
	local z2 = self:getPinValue(self.id+6,data,mission)[0];
	local solid = self:getPinValue(self.id+7,data,mission)[0];
	local vehicle = self:getPinValue(self.id+8,data,mission)[0];
	local ped = self:getPinValue(self.id+9,data,mission)[0];
	local object = self:getPinValue(self.id+10,data,mission)[0];
	local particle = self:getPinValue(self.id+11,data,mission)[0];
	ldyom.setLastNode(self.id);
	
	local result = self.Pins[self.id+12].value;
	
	result[0] = callOpcode(0x0339, {{x1,"float"}, {y1,"float"}, {z1,"float"}, {x2,"float"}, {y2,"float"}, {z2,"float"}, {solid,"bool"}, {vehicle,"bool"}, {ped,"bool"}, {object,"bool"}, {particle,"bool"}});
end

ldyom.nodeEditor.addNodeClass("World",Node);