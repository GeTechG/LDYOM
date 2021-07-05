ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeAddCounter", BaseNode));

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeAddCounter");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.string, 0, ffi.new("char[129]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.void, 1),
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
		imgui.SliderInt(ldyom.langt("slot"),self.Pins[self.id+2].value, 0, 3, tostring(self.Pins[self.id+2].value[0]));
	else
		imgui.Text("slot");
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	if (self.Pins[self.id+3].link == nil) then
		imgui.SetNextItemWidth(150);
		imgui.ToggleButton(ldyom.langt("progressBar"),self.Pins[self.id+3].value);
	else
		imgui.Text("progressBar");
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("value"));
	imgui.imnodes.EndInputAttribute();
	
	if (self.Pins[self.id+3].value[0]) then
		imgui.imnodes.BeginInputAttribute(self.id+5);
		if not self.Pins[self.id+5].link then
			imgui.SetNextItemWidth(200);
			imgui.InputFloat(" - 1%", self.Pins[self.id+5].value);
		end
		imgui.imnodes.EndInputAttribute();
	end
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	imgui.Text(ldyom.langt("text"));
	if not self.Pins[self.id+6].link then
		imgui.SetNextItemWidth(200);
		imgui.InputText("", self.Pins[self.id+6].value, ffi.sizeof(self.Pins[self.id+6].value));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+7);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:getValue(data, mission)
	if self:getPinValue(self.id+3,data,mission)[0] then
		return math.floor(self:getPinValue(self.id+4,data,mission)[0] / math.max(self:getPinValue(self.id+5,data,mission)[0],1));
	else
		return self:getPinValue(self.id+4,data,mission)[0];
	end
end

function Node:play(data, mission)

	local slot = self:getPinValue(self.id+2,data,mission)[0];
	local isProgressBar = self:getPinValue(self.id+3,data,mission)[0];
	local text = ffi.string(self:getPinValue(self.id+6,data,mission));
	ldyom.setLastNode(self.id);
	
	callOpcode(0x0ADF,{{"LDCNTR"..slot,"string"}, {GXTEncode(UTF8_to_CP1251(text)),"string"}});
	
	ldyom.addCounterByNode(slot, isProgressBar, "LDCNTR"..slot, self, data, mission);
end

ldyom.nodeEditor.addNodeClass("World",Node);