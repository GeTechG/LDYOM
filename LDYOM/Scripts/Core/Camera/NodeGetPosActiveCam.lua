ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeGetPosActiveCam", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeGetPosActiveCam");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 1, ffi.new("float[1]")),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 1, ffi.new("float[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 1, ffi.new("float[1]")),
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
	
	imgui.imnodes.BeginOutputAttribute(self.id+1);
	imgui.Indent(200);
	imgui.Text("x");
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+2);
	imgui.Indent(200);
	imgui.Text("y");
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+3);
	imgui.Indent(200);
	imgui.Text("z");
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	ldyom.setLastNode(self.id);
	
	callOpcode(0x068D, {{self.Pins[self.id+1].value,"float*"}, {self.Pins[self.id+2].value,"float*"}, {self.Pins[self.id+3].value,"float*"}});
end

ldyom.nodeEditor.addNodeClass("Camera",Node);