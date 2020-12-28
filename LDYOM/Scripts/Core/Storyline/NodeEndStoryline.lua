ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = class("NodeEndStoryline", BaseNode);

Node.static.name = imgui.imnodes.getNodeIcon("exit")..' '..ldyom.langt("CoreNodeEndStoryline");
Node.static.storyline = true;

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 5;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0)
	};
end

function Node:draw()
	imgui.imnodes.BeginNode(self.id,self.type)
	
	imgui.imnodes.BeginNodeTitleBar();
	imgui.Text(self.class.static.name);
	imgui.imnodes.EndNodeTitleBar();
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	imgui.Dummy(imgui.ImVec2:new(0,20));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.EndNode();
	
end

ldyom.nodeEditor.addNodeClass("Storyline",Node);