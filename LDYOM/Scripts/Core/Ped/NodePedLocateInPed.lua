ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePedLocateInPed", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("fork")..' '..ldyom.langt("CoreNodePedLocateInPed");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 1;
	self.three = ffi.new("bool[1]",false);
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
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
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text(ldyom.langt("type"));
	if not self.Pins[self.id+2].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+2].value, ldyom.langMenu["typeLocate"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+12);
	imgui.ToggleButton("3D", self.three);
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
		
	local ped = self:getPinValue(self.id+1,data,mission)[0];
	local _type = self:getPinValue(self.id+2,data,mission)[0];
	local ped2 = self:getPinValue(self.id+4,data,mission)[0];
	local radius = self:getPinValue(self.id+7,data,mission)[0];
	local sphere = self:getPinValue(self.id+8,data,mission)[0];
	assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");
	assert(callOpcode(0x056D, {{ped2,"int"}}), "Not found ped");
	ldyom.setLastNode(self.id);
	
	local result = self.Pins[self.id+9].value;
	
	local sphere_id;
	if sphere then
		local x = ffi.new("float[1]");
		local y = ffi.new("float[1]");
		local z = ffi.new("float[1]");
		callOpcode(0x00A0, {{ped2,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
		sphere_id = ldyom.createScriptSphere(self.id + 700, x, y, z, radius);
	end
	
	if self.three[0] then
		if _type == 0 then
			result[0] = callOpcode(0x0104, {{ped,"int"}, {ped2,"int"}, {radius,"float"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
		elseif _type == 1 then
			result[0] = callOpcode(0x0105, {{ped,"int"}, {ped2,"int"}, {radius,"float"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
		else
			result[0] = callOpcode(0x0106, {{ped,"int"}, {ped2,"int"}, {radius,"float"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
		end
	else
		if _type == 0 then
			result[0] = callOpcode(0x00F2, {{ped,"int"}, {ped2,"int"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
		elseif _type == 1 then
			result[0] = callOpcode(0x00F3, {{ped,"int"}, {ped2,"int"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
		else
			result[0] = callOpcode(0x00F4, {{ped,"int"}, {ped2,"int"}, {radius,"float"}, {radius,"float"}, {false,"bool"}});
		end
	end
	
	if sphere then
		ldyom.drawScriptSpheres();
		
		ldyom.removeScriptSphere(sphere_id);
	end
	
end

ldyom.nodeEditor.addNodeClass("Ped",Node);