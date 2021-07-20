ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeIsPedInZone", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("fork")..' '..ldyom.langt("CoreNodeIsPedInZone");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 1;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.string, 0, ffi.new("char[129]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.boolean, 1, ffi.new("bool[1]")),
	};
	self:updateZone();
end

function Node:updateZone()
	local x = ffi.new("float[1]");
	local y = ffi.new("float[1]");
	local z = ffi.new("float[1]");
	callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
	local pointerZone = ffi.new("int[1]");
	callOpcode(0x0843, {{x[0], "float"}, {y[0], "float"}, {z[0], "float"}, {pointerZone, "int*"}});
	ffi.copy(self.Pins[self.id+3].value, getPointer(pointerZone));
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
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.SetNextItemWidth(200);
	imgui.InputText(ldyom.langt("zone"), self.Pins[self.id+3].value, 129);
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+5);
	if imgui.Button(ldyom.langt("getZone"), imgui.ImVec2:new(200,20)) then
		self:updateZone();
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+4);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("result"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
		
	local ped = self:getPinValue(self.id+2,data,mission)[0];
	local zone = ffi.string(self:getPinValue(self.id+3,data,mission));
	assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");
	ldyom.setLastNode(self.id);
	
	local result = self.Pins[self.id+4].value;
	
	result[0] = callOpcode(0x0154, {{ped,"int"}, {zone,"string"}});
end

ldyom.nodeEditor.addNodeClass("Ped",Node);