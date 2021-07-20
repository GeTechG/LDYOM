ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePedTurretModeObject", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodePedTurretModeObject");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]", 0.5)),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]", -1.0)),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("float[1]", 0.7)),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.number, 0, ffi.new("float[1]", 360.0)),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.number, 0, ffi.new("int[1]",1)),
		[self.id+10] = BasePin:new(self.id+10,imgui.imnodes.PinType.void, 1),
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
	imgui.Text(ldyom.langt("CoreHandleObject"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("offset").." x");
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text(ldyom.langt("offset").." y");
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+5].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	imgui.Text(ldyom.langt("offset").." z");
	if not self.Pins[self.id+6].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+6].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+7);
	imgui.Text(ldyom.langt("direction"));
	if not self.Pins[self.id+7].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+7].value, ldyom.langMenu["CoreTurretDirection"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+8);
	imgui.Text(ldyom.langt("angle"));
	if not self.Pins[self.id+8].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+8].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+9);
	if not self.Pins[self.id+9].link then
		ldyom.selectWeapon(ldyom.langt("weapon"), self.Pins[self.id+9].value);
	else
		imgui.Text(ldyom.langt("weapon"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+10);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
		
	local ped = self:getPinValue(self.id+2,data,mission)[0];
	local object = self:getPinValue(self.id+3,data,mission)[0];
	local offset_x = self:getPinValue(self.id+4,data,mission)[0];
	local offset_y = self:getPinValue(self.id+5,data,mission)[0];
	local offset_z = self:getPinValue(self.id+6,data,mission)[0];
	local direction = self:getPinValue(self.id+7,data,mission)[0];
	local angleLimit = self:getPinValue(self.id+8,data,mission)[0];
	local weapon = self:getPinValue(self.id+9,data,mission)[0];
	assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");
	assert(callOpcode(0x03CA, {{object,"int"}}), "Not found object");
	ldyom.setLastNode(self.id);
	
	local model = ffi.new("int[1]");
	callOpcode(0x0781, {{ldyom.ID_Weapons[weapon+1],"int"}, {model,"int*"}});
	callOpcode(0x0247, {{model[0], "int"}});
	
	while not callOpcode(0x0248, {{model[0],"int"}}) do
		wait(0);
	end
	
	callOpcode(0x04F4, {{ped,"int"}, {object,"int"}, {offset_x,"float"}, {offset_y,"float"}, {offset_z,"float"}, {direction,"int"}, {angleLimit,"float"}, {ldyom.ID_Weapons[weapon+1],"int"}});
	
	self:callOutputLinks(data, mission, self.id+10);
end

ldyom.nodeEditor.addNodeClass("Ped",Node);