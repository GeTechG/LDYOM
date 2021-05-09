ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeWaitUntil", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("loop")..' '..ldyom.langt("CoreNodeWaitUntil");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 2;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]",false)),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.void, 1),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.void, 1),
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
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	if not self.Pins[self.id+2].link then
		imgui.ToggleButton(ldyom.langt("condition"), self.Pins[self.id+2].value)
	else
		imgui.Text(ldyom.langt("condition"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+3);
	imgui.Indent(50);
	imgui.Text(ldyom.langt("whenItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+4);
	imgui.Indent(50);
	imgui.Text(ldyom.langt("untilItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	ldyom.waitUtilNode(self, self.id+2,self.id+3,self.id+4, data, mission);
end

ldyom.nodeEditor.addNodeClass("Main",Node);