ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeAttachCamToVehicleLookVehicle", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeAttachCamToVehicleLookVehicle");
Node.static.mission = true;

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.void, 1),
	};
end

function attachToVehicleLookVehicle()
	ldyom.attachCameraToEntity(2, currNodeStaticCamera.Pins[currNodeStaticCamera.id+2].value[0], currNodeStaticCamera.Pins[currNodeStaticCamera.id+6].value[0], currNodeStaticCamera.Pins[currNodeStaticCamera.id+3].value, currNodeStaticCamera.Pins[currNodeStaticCamera.id+4].value, currNodeStaticCamera.Pins[currNodeStaticCamera.id+5].value, nil, nil, nil);
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
	local names = ldyom.namesCars;
	imgui.Text(ldyom.langt("car"));
	imgui.SetNextItemWidth(150);
	imgui.ComboVecChars("",self.Pins[self.id+2].value,names);
	imgui.imnodes.EndStaticAttribute();
	
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
	
	imgui.imnodes.BeginStaticAttribute(self.id+6);
	imgui.Text(ldyom.langt("car"));
	imgui.SetNextItemWidth(150);
	imgui.ComboVecChars("",self.Pins[self.id+6].value,names);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+7);
	if not self.Pins[self.id+7].link then
		imgui.ToggleButton(ldyom.langt("movecam"), self.Pins[self.id+7].value);
	else
		imgui.Text(ldyom.langt(""));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+8);
	if imgui.Button(ldyom.langt("edithand"), imgui.ImVec2:new(200,20)) and #names > 0 then
		callOpcode(0x01B4, {{0,"int"}, {0,"int"}});
		currNodeStaticCamera = self;
		ldyom.set_off_gui(true);
		addThread(attachToVehicleLookVehicle);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+9);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local car = self:getPinValue(self.id+2,data,mission)[0];
	local x = self:getPinValue(self.id+3,data,mission)[0];
	local y = self:getPinValue(self.id+4,data,mission)[0];
	local z = self:getPinValue(self.id+5,data,mission)[0];
	local car2 = self:getPinValue(self.id+6,data,mission)[0];
	local movecam = self:getPinValue(self.id+7,data,mission)[0];
	assert(mission.list_cars[car+1].missionCar, "The car is not yet established or has already disappeared.");
	assert(mission.list_cars[car2+1].missionCar, "The car is not yet established or has already disappeared.");
	ldyom.setLastNode(self.id);

	local carRef = getCarRef(mission.list_cars[car+1].missionCar);
	local car2Ref = getCarRef(mission.list_cars[car2+1].missionCar);

	callOpcode(0x067A, {{carRef,"int"}, {x,"float"}, {y,"float"}, {z,"float"}, {car2Ref,"int"}, {0.0,"float"}, {fif(movecam,1,2),"int"}});
	
	self:callOutputLinks(data, mission, self.id+9);
end

ldyom.nodeEditor.addNodeClass("Camera",Node);