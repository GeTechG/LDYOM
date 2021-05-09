ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeOpenDoors", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeOpenDoors");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.void, 1),
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
	imgui.Text(ldyom.langt("vehicle"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	if not self.Pins[self.id+3].link then
		imgui.ToggleButton(ldyom.langt("door_car_1"), self.Pins[self.id+3].value);
	else
		imgui.Text(ldyom.langt("door_car_1"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	if not self.Pins[self.id+4].link then
		imgui.ToggleButton(ldyom.langt("door_car_2"), self.Pins[self.id+4].value);
	else
		imgui.Text(ldyom.langt("door_car_2"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	if not self.Pins[self.id+5].link then
		imgui.ToggleButton(ldyom.langt("door_car_3"), self.Pins[self.id+5].value);
	else
		imgui.Text(ldyom.langt("door_car_3"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	if not self.Pins[self.id+6].link then
		imgui.ToggleButton(ldyom.langt("door_car_4"), self.Pins[self.id+6].value);
	else
		imgui.Text(ldyom.langt("door_car_4"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+7);
	if not self.Pins[self.id+7].link then
		imgui.ToggleButton(ldyom.langt("door_car_5"), self.Pins[self.id+7].value);
	else
		imgui.Text(ldyom.langt("door_car_5"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+8);
	if not self.Pins[self.id+8].link then
		imgui.ToggleButton(ldyom.langt("door_car_6"), self.Pins[self.id+8].value);
	else
		imgui.Text(ldyom.langt("door_car_6"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+9);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
		
	local vehicle = self:getPinValue(self.id+2,data,mission)[0];
	assert(callOpcode(0x056E, {{vehicle,"int"}}), "Not found vehicle");
	ldyom.setLastNode(self.id);
	
	for g = 0,5 do
		local door = self:getPinValue(self.id+3+g,data,mission)[0];
		if door then
			callOpcode(0x0657, {{vehicle,"int"}, {g,"int"}});
		end
	end
	
	self:callOutputLinks(data, mission, self.id+9);
end

ldyom.nodeEditor.addNodeClass("Vehicle",Node);