ffi = require "ffi"
class = require "LDYOM.Scripts.middleclass"

TestNode = bitser.registerClass(class("TestNode", BaseNode));

for k,v in pairs(TestNode) do
	print(tostring(k == "name"));
end

TestNode.static.name = imgui.imnodes.getNodeIcon("event")..' '..ldyom.langt("CoreNodeTest");

function TestNode:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 0;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.boolean, 0),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 1),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.boolean, 1),
	};
end

function TestNode:draw()
	imgui.imnodes.BeginNode(self.id,self.type)
	
	imgui.imnodes.BeginNodeTitleBar();
	imgui.Text(TestNode.static.name);
	imgui.imnodes.EndNodeTitleBar();
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	imgui.Text("input");
	imgui.imnodes.EndInputAttribute();
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text("input2");
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+3);
	imgui.Text("input");
	imgui.imnodes.EndOutputAttribute();
	imgui.imnodes.BeginOutputAttribute(self.id+4);
	imgui.Text("input2");
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

ldyom.nodeEditor.addNodeClass("first",TestNode);