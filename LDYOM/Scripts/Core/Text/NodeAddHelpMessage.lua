ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeAddHelpMessage", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeAddHelpMessage");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.gxt = ffi.new("bool[1]", false);
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.string, 0, ffi.new("char[128]","")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.void, 1)
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
	imgui.Text(ldyom.langt("text"));
	if (self.Pins[self.id+2].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.InputText("",self.Pins[self.id+2].value,ffi.sizeof(self.Pins[self.id+2].value));
		imgui.SameLine(0,0);
		imgui.ToggleButton(ldyom.langt("gxt"), self.gxt);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	if (self.Pins[self.id+3].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.ToggleButton(ldyom.langt("CoreQuickMessage"),self.Pins[self.id+3].value);
	else
		imgui.Text(ldyom.langt("CoreQuickMessage"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	if (self.Pins[self.id+4].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.ToggleButton(ldyom.langt("CoreInstantShow"),self.Pins[self.id+4].value);
	else
		imgui.Text(ldyom.langt("CoreInstantShow"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+5);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local text = ffi.string(self:getPinValue(self.id+2,data,mission));
	local quickMessage = self:getPinValue(self.id+3,data,mission)[0];
	local instant = self:getPinValue(self.id+4,data,mission)[0];
	ldyom.setLastNode(self.id);
	
	AddHelpMessage(text, quickMessage, instant, self.gxt[0]);
	
	self:callOutputLinks(data,mission,self.id+5);
end

ldyom.nodeEditor.addNodeClass("Text",Node);