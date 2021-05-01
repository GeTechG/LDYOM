ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeSlideObject", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..imgui.imnodes.getNodeIcon("loop")..' '..ldyom.langt("CoreNodeSlideObject");

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
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+11] = BasePin:new(self.id+11,imgui.imnodes.PinType.void, 1),
		[self.id+12] = BasePin:new(self.id+12,imgui.imnodes.PinType.void, 1),
	};
end

function slideObject()
	ldyom.slideObject(currNodeSlideObject.Pins[currNodeSlideObject.id+2].value[0], currNodeSlideObject.Pins[currNodeSlideObject.id+3].value, currNodeSlideObject.Pins[currNodeSlideObject.id+4].value, currNodeSlideObject.Pins[currNodeSlideObject.id+5].value);
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
	local names = ldyom.namesObjects;
	imgui.Text(ldyom.langt("object"));
	imgui.SetNextItemWidth(200);
	imgui.ComboVecChars("",self.Pins[self.id+2].value,names);
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
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	imgui.Text(ldyom.langt("speed").." x");
	if not self.Pins[self.id+6].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+6].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+7);
	imgui.Text(ldyom.langt("speed").." y");
	if not self.Pins[self.id+7].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+7].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+8);
	imgui.Text(ldyom.langt("speed").." z");
	if not self.Pins[self.id+8].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+8].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+9);
	if not self.Pins[self.id+9].link then
		imgui.SetNextItemWidth(200);
		imgui.ToggleButton(ldyom.langt("CoreTestCollision"), self.Pins[self.id+9].value);
	else
		imgui.Text(ldyom.langt("CoreTestCollision"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+10);
	if imgui.Button(ldyom.langt("edithand"), imgui.ImVec2:new(200,20)) then
		if (#names > 0) then
			callOpcode(0x01B4, {{0,"int"}, {0,"int"}});
			currNodeSlideObject = self;
			ldyom.set_off_gui(true);
			addThread(slideObject);
		end
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+11);
	imgui.Indent(120);
	imgui.Text(ldyom.langt("CoreSliding"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+12);
	imgui.Indent(120);
	imgui.Text(ldyom.langt("CoreNotSliding"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local object = self:getPinValue(self.id+2,data,mission)[0];
	local x = self:getPinValue(self.id+3,data,mission)[0];
	local y = self:getPinValue(self.id+4,data,mission)[0];
	local z = self:getPinValue(self.id+5,data,mission)[0];
	local speed_x = self:getPinValue(self.id+6,data,mission)[0];
	local speed_y = self:getPinValue(self.id+7,data,mission)[0];
	local speed_z = self:getPinValue(self.id+8,data,mission)[0];
	local check_collision = self:getPinValue(self.id+9,data,mission)[0];
	ldyom.setLastNode(self.id);
	assert(mission.list_objects[object+1].missionObject,"The object is not yet established or has already disappeared.");
	local obj = getObjectRef(mission.list_objects[object+1].missionObject);
	
	local sliding = callOpcode(0x034E, {{obj,"int"}, {x,"float"}, {y,"float"}, {z,"float"}, {speed_x,"float"}, {speed_y,"float"}, {speed_z,"float"}, {fif(check_collision,1,0),"int"}});
	
	if not sliding then
		self:callOutputLinks(data, mission, self.id+11);
	else
		self:callOutputLinks(data, mission, self.id+12);
	end
end

ldyom.nodeEditor.addNodeClass("Object",Node);