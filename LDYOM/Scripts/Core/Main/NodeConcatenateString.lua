ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeConcatenateString", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeConcatenateString");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.string, 0, ffi.new("char[128]")),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.string, 0, ffi.new("char[128]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.string, 1, ffi.new("char[128]")),
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
	if (self.Pins[self.id+1].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.InputText("", self.Pins[self.id+1].value, 128);
	else
		imgui.Text(ldyom.langt("value").." 1");
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	if (self.Pins[self.id+2].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.InputText("", self.Pins[self.id+2].value, 128);
	else
		imgui.Text(ldyom.langt("value").." 2");
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+3);
	imgui.Indent(120);
	imgui.Text(ldyom.langt("result"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local value1 = ffi.string(self:getPinValue(self.id+1,data,mission));
	local value2 = ffi.string(self:getPinValue(self.id+2,data,mission));
	ldyom.setLastNode(self.id);
	
	ffi.copy(self.Pins[self.id+3].value, value1 .. value2);
end

ldyom.nodeEditor.addNodeClass("Main",Node);