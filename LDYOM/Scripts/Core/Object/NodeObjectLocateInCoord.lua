ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeObjectLocateInCoord", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("fork")..' '..ldyom.langt("CoreNodeObjectLocateInCoord");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 1;
	self.three = ffi.new("bool[1]",false);
	local x = ffi.new("float[1]");
	local y = ffi.new("float[1]");
	local z = ffi.new("float[1]");
	callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",x[0])),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",y[0])),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",z[0])),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",2.0)),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.boolean, 1, ffi.new("bool[1]")),
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
	imgui.Text(ldyom.langt("CoreHandleObject"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+12);
	imgui.ToggleButton("3D", self.three);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text("x");
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text("y");
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+5].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	if self.three[0] then
		imgui.imnodes.BeginInputAttribute(self.id+6);
		imgui.Text("z");
		if not self.Pins[self.id+6].link then
			imgui.SetNextItemWidth(200);
			imgui.InputFloat("", self.Pins[self.id+6].value);
		end
		imgui.imnodes.EndInputAttribute();
	end
	
	imgui.imnodes.BeginStaticAttribute(self.id+13);
	if imgui.Button(ldyom.langt("playerCoordinates"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {self.Pins[self.id+4].value,"float*"}, {self.Pins[self.id+5].value,"float*"}, {self.Pins[self.id+6].value,"float*"}});
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+7);
	imgui.Text(ldyom.langt("radius"));
	if not self.Pins[self.id+7].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+7].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+8);
	if not self.Pins[self.id+8].link then
		imgui.SetNextItemWidth(200);
		imgui.ToggleButton(ldyom.langt("sphere"), self.Pins[self.id+8].value);
	else
		imgui.Text(ldyom.langt("sphere"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+9);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("result"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
		
	local object = self:getPinValue(self.id+1,data,mission)[0];
	local x = self:getPinValue(self.id+4,data,mission)[0];
	local y = self:getPinValue(self.id+5,data,mission)[0];
	local z = self:getPinValue(self.id+6,data,mission)[0];
	local radius = self:getPinValue(self.id+7,data,mission)[0];
	local sphere = self:getPinValue(self.id+8,data,mission)[0];
	assert(callOpcode(0x03CA, {{object,"int"}}), "Not found object");
	ldyom.setLastNode(self.id);
	
	local result = self.Pins[self.id+9].value;
	
	local sphere_id;
	if sphere then
		sphere_id = ldyom.createScriptSphere(self.id + 700, x, y, z, radius);
	end
	
	if self.three[0] then
		result[0] = callOpcode(0x04E6, {{object,"int"}, {x,"float"}, {y,"float"}, {z,"float"}, {radius,"float"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
	else
		result[0] = callOpcode(0x04E5, {{object,"int"}, {x,"float"}, {y,"float"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
	end
	
	if sphere then
		ldyom.drawScriptSpheres();
		
		ldyom.removeScriptSphere(sphere_id);
	end
	
end

ldyom.nodeEditor.addNodeClass("Object",Node);