ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeTriggerMove", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeTriggerMove");

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
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[3]",{x[0],y[0],z[0]})),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[3]",{0,0,0})),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",1)),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",1)),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.void, 1),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]",true)),
	};
end

function moveObject()
	ldyom.moveObject(currNodeMoveObject.Pins[currNodeMoveObject.id+2].value[0], currNodeMoveObject.Pins[currNodeMoveObject.id+3].value, currNodeMoveObject.Pins[currNodeMoveObject.id+4].value);
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
	
	imgui.imnodes.BeginStaticAttribute(self.id+2);
	local names = ldyom.namesObjects;
	imgui.Text(ldyom.langt("object"));
	imgui.SetNextItemWidth(150);
	imgui.ComboVecChars("",self.Pins[self.id+2].value,names);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+3);
	imgui.Text(ldyom.langt("position"));
	imgui.SetNextItemWidth(200);
	imgui.InputFloat3("", self.Pins[self.id+3].value);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+4);
	imgui.Text(ldyom.langt("rotate"));
	imgui.SetNextItemWidth(200);
	imgui.InputFloat3("", self.Pins[self.id+4].value);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text(ldyom.langt("speed"));
	imgui.SetNextItemWidth(200);
	imgui.InputFloat("", self.Pins[self.id+5].value);
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	imgui.Text(ldyom.langt("radius"));
	imgui.SetNextItemWidth(200);
	imgui.InputFloat("", self.Pins[self.id+6].value);
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+7);
	if imgui.Button(ldyom.langt("edithand"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x01B4, {{0,"int"}, {0,"int"}});
		currNodeMoveObject = self;
		ldyom.set_off_gui(true);
		addThread(moveObject);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+8);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+9);
	if (self.Pins[self.id+9].link == nil) then
		imgui.Text(ldyom.langt("truee"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.EndNode();
	
	
	--update to Beta 0.7.2
	if (self.Pins[self.id+9] == nil) then
		self.Pins[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]",true))
	end
end

function Node:play(data, mission)
	
	local object = self:getPinValue(self.id+2,data,mission)[0];
	local pos = self:getPinValue(self.id+3,data,mission);
	local rot = self:getPinValue(self.id+4,data,mission);
	local speed = self:getPinValue(self.id+5,data,mission)[0] / 10;
	local radius = self:getPinValue(self.id+6,data,mission)[0];
	
	ldyom.triggerMoveObject(self, self.id+9, object, pos[0], pos[1], pos[2], rot[0], rot[1], rot[2], speed, radius, data, mission);
	
	self:callOutputLinks(data, mission, self.id+8);
end

ldyom.nodeEditor.addNodeClass("Object",Node);