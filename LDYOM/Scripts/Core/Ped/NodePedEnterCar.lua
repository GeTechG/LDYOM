ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePedEnterCar", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodePedEnterCar");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.void, 1),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.void, 1),
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
	imgui.Text(ldyom.langt("CoreHandleCar"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("speed_walk"));
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+4].value, ldyom.langMenu["speed_walk_to_car"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text(ldyom.langt("seat"));
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+5].value, ldyom.langMenu["place_in_car"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	if not self.Pins[self.id+6].link then
		imgui.ToggleButton(ldyom.langt("teleport"), self.Pins[self.id+6].value);
	else
		imgui.Text(ldyom.langt("teleport"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+7);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("whenItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+8);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
		
	local ped = self:getPinValue(self.id+2,data,mission)[0];
	local car = self:getPinValue(self.id+3,data,mission)[0];
	local speed_walk = self:getPinValue(self.id+4,data,mission)[0]
	local seat = self:getPinValue(self.id+5,data,mission)[0]
	local teleport = self:getPinValue(self.id+6,data,mission)[0]
	assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");
	assert(callOpcode(0x056E, {{car,"int"}}), "Not found vehicle");
	ldyom.setLastNode(self.id);
	
	ldyom.nodeEnterCar(ped, car, speed_walk, seat, teleport, self, self.id+7, data, mission);

	self:callOutputLinks(data, mission, self.id+8);
end

ldyom.nodeEditor.addNodeClass("Ped",Node);