ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeDistanceBetweenPoints", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeDistanceBetweenPoints");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.three = ffi.new("bool[1]");
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.number, 1, ffi.new("float[1]")),
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
	
	imgui.imnodes.BeginStaticAttribute(self.id+8);
	imgui.ToggleButton("3D", self.three)
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	imgui.Text("x1");
	if not self.Pins[self.id+1].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+1].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text("y1");
	if not self.Pins[self.id+2].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+2].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	if (self.three[0]) then
		imgui.imnodes.BeginInputAttribute(self.id+3);
		imgui.Text("z1");
		if not self.Pins[self.id+3].link then
			imgui.SetNextItemWidth(200);
			imgui.InputFloat("", self.Pins[self.id+3].value);
		end
		imgui.imnodes.EndInputAttribute();
	end
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text("x2");
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text("y2");
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+5].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	if (self.three[0]) then
		imgui.imnodes.BeginInputAttribute(self.id+6);
		imgui.Text("z2");
		if not self.Pins[self.id+6].link then
			imgui.SetNextItemWidth(200);
			imgui.InputFloat("", self.Pins[self.id+6].value);
		end
		imgui.imnodes.EndInputAttribute();
	end
	
	imgui.imnodes.BeginOutputAttribute(self.id+7);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local x1 = self:getPinValue(self.id+1,data,mission)[0];
	local y1 = self:getPinValue(self.id+2,data,mission)[0];
	local z1 = self:getPinValue(self.id+3,data,mission)[0];
	local x2 = self:getPinValue(self.id+4,data,mission)[0];
	local y2 = self:getPinValue(self.id+5,data,mission)[0];
	local z2 = self:getPinValue(self.id+6,data,mission)[0];
	ldyom.setLastNode(self.id);
	
	if (self.three[0]) then
		self.Pins[self.id+7].value[0] = calcDistance(x1, y1, z1, x2, y2, z2);
	else
		self.Pins[self.id+7].value[0] = calcDistance(x1, y1, 0, x2, y2, 0);
	end
end

ldyom.nodeEditor.addNodeClass("World",Node);