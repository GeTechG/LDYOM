ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeStaticCutscene", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeStaticCutscene");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.pointPos = ffi.new("bool[1]",true);
	local x = ffi.new("float[1]");
	local y = ffi.new("float[1]");
	local z = ffi.new("float[1]");
	callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",x[0])),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",y[0])),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",z[0])),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+10] = BasePin:new(self.id+10,imgui.imnodes.PinType.void, 1),
	};
end

function staticCam()
	ldyom.staticCutscene(currNodeStaticCamera.Pins[currNodeStaticCamera.id+2].value, currNodeStaticCamera.Pins[currNodeStaticCamera.id+3].value, currNodeStaticCamera.Pins[currNodeStaticCamera.id+4].value, currNodeStaticCamera.Pins[currNodeStaticCamera.id+5].value, currNodeStaticCamera.Pins[currNodeStaticCamera.id+6].value, currNodeStaticCamera.Pins[currNodeStaticCamera.id+7].value);
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
	imgui.Text("x");
	if not self.Pins[self.id+2].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+2].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text("y");
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+3].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text("z");
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+10);
	imgui.ToggleButton(ldyom.langt("turn"), self.pointPos);
	imgui.imnodes.EndStaticAttribute();
	
	if (self.pointPos[0]) then
		imgui.imnodes.BeginInputAttribute(self.id+5);
		imgui.Text(ldyom.langt("rotate").." x");
		if not self.Pins[self.id+5].link then
			imgui.SetNextItemWidth(200);
			imgui.InputInt("", self.Pins[self.id+5].value);
		end
		imgui.imnodes.EndInputAttribute();
		
		imgui.imnodes.BeginInputAttribute(self.id+6);
		imgui.Text(ldyom.langt("rotate").." y");
		if not self.Pins[self.id+6].link then
			imgui.SetNextItemWidth(200);
			imgui.InputInt("", self.Pins[self.id+6].value);
		end
		imgui.imnodes.EndInputAttribute();
		
		imgui.imnodes.BeginInputAttribute(self.id+7);
		imgui.Text(ldyom.langt("rotate").." z");
		if not self.Pins[self.id+7].link then
			imgui.SetNextItemWidth(200);
			imgui.InputInt("", self.Pins[self.id+7].value);
		end
		imgui.imnodes.EndInputAttribute();
		
		imgui.imnodes.BeginInputAttribute(self.id+8);
		if not self.Pins[self.id+8].link then
			imgui.ToggleButton(ldyom.langt("movecam"), self.Pins[self.id+8].value);
		else
			imgui.Text(ldyom.langt(""));
		end
		imgui.imnodes.EndInputAttribute();
		
	end
	
	imgui.imnodes.BeginStaticAttribute(self.id+9);
	if imgui.Button(ldyom.langt("edithand"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x01B4, {{0,"int"}, {0,"int"}});
		currNodeStaticCamera = self;
		ldyom.set_off_gui(true);
		addThread(staticCam);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+10);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local x = self:getPinValue(self.id+2,data,mission)[0];
	local y = self:getPinValue(self.id+3,data,mission)[0];
	local z = self:getPinValue(self.id+4,data,mission)[0];
	local rot_x = self:getPinValue(self.id+5,data,mission)[0];
	local rot_y = self:getPinValue(self.id+6,data,mission)[0];
	local rot_z = self:getPinValue(self.id+7,data,mission)[0];
	local movecam = self:getPinValue(self.id+8,data,mission)[0];
	ldyom.setLastNode(self.id);
	
	local xx = x
	local xy = y
	local xz = z;
	local rxx = rot_x
	local rxy = rot_y
	local rxz = rot_z;
	local x1 = xx
	local y1 = xy
	local z1 = xz;
	x1 = x1 + 2 * math.sin(math.rad(rxy)) * math.sin(math.rad(rxx));
	y1 = y1 + 2 * math.cos(math.rad(rxy)) * math.sin(math.rad(rxx));
	z1 = z1 + 2 * math.cos(math.rad(rxx));

	callOpcode(0x015F, {{xx,"float"}, {xy,"float"}, {xz,"float"}, {0,"float"}, {math.rad(rxz),"float"}, {0,"float"}});
	if (self.pointPos[0]) then
		callOpcode(0x0160, {{x1,"float"}, {y1,"float"}, {z1,"float"}, {fif(movecam,1,2),"int"}});
	end
	
	self:callOutputLinks(data, mission, self.id+10);
end

ldyom.nodeEditor.addNodeClass("Camera",Node);