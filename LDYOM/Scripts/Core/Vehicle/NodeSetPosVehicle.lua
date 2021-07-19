ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeSetPosVehicle", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeSetPosVehicle");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	local x = ffi.new("float[1]");
	local y = ffi.new("float[1]");
	local z = ffi.new("float[1]");
	callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",x[0])),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",y[0])),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",z[0])),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.void, 1),
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
	imgui.Text(ldyom.langt("CoreHandleCar"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text("x");
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+3].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text("y");
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text("z");
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+5].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+7);
	if imgui.Button(ldyom.langt("playerCoordinates"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {self.Pins[self.id+3].value,"float*"}, {self.Pins[self.id+4].value,"float*"}, {self.Pins[self.id+5].value,"float*"}});
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+6);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
		
	local vehicle = self:getPinValue(self.id+2,data,mission)[0];
	local x = self:getPinValue(self.id+3,data,mission)[0];
	local y = self:getPinValue(self.id+4,data,mission)[0];
	local z = self:getPinValue(self.id+5,data,mission)[0];
	assert(callOpcode(0x056E, {{vehicle,"int"}}), "Not found vehicle");	
	ldyom.setLastNode(self.id);
	
	callOpcode(0x00AB, {{vehicle,"int"}, {x,"float"}, {y,"float"}, {z,"float"}});
	
	self:callOutputLinks(data, mission, self.id+6);
end

ldyom.nodeEditor.addNodeClass("Vehicle",Node);