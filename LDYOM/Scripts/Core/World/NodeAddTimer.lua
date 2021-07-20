ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeAddTimer", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langMenu["target_type"][7];

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.string, 0, ffi.new("char[129]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.void, 1),
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
	if (self.Pins[self.id+2].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.ToggleButton(ldyom.langt("backwards"),self.Pins[self.id+2].value);
	else
		imgui.Text("backwards");
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text(ldyom.langt("start"));
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(200);
		imgui.InputInt("", self.Pins[self.id+3].value);
		imgui.SameLine(0,0);
	end
	imgui.Text(ldyom.langt("time"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("text"));
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputText("", self.Pins[self.id+4].value, ffi.sizeof(self.Pins[self.id+4].value));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+5);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)

	local direction = self:getPinValue(self.id+2,data,mission)[0];
	local start_time = self:getPinValue(self.id+3,data,mission)[0];
	local text = ffi.string(self:getPinValue(self.id+4,data,mission));
	ldyom.setLastNode(self.id);
	
	callOpcode(0x0ADF,{{"LDTIMER0","string"}, {GXTEncode(UTF8_to_CP1251(text)),"string"}});
	
	ldyom.addTimer("LDTIMER0", direction, start_time * 1000);
end

ldyom.nodeEditor.addNodeClass("World",Node);