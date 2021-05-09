ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePedChangeSkin", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodePedChangeSkin");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("int[1]",1)),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.void, 1),
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
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+3);
	imgui.Text(ldyom.langt("type"));
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(150);
		if imgui.SliderInt("", self.Pins[self.id+3].value, 0, 1, fif(self.Pins[self.id+3].value[0] == 0,ldyom.langt("ped"),ldyom.langt("specActor"))) then
			self.Pins[self.id+4].value[0] = 0;
		end
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+4);
	if self.Pins[self.id+3].value[0] == 0 then
		if imgui.Button('\xef\x95\x93',imgui.ImVec2:new(20,20)) then
			imgui.OpenPopup("skins", 0);
		end
		imgui.SameLine(0,-1);
		imgui.SetNextItemWidth(150);
		imgui.InputInt(ldyom.langt("model"), self.Pins[self.id+4].value);
		ldyom.SkinSelector(self.Pins[self.id+4].value);
	else
		imgui.SetNextItemWidth(150);
		imgui.SliderInt(ldyom.langt("slot"), self.Pins[self.id+5].value, 0, 9, "%d");
		imgui.SetNextItemWidth(150);
		imgui.ComboVecStr(ldyom.langt("model"), self.Pins[self.id+4].value, ldyom.ID_Spec_Actors);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+6);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	local ped = self:getPinValue(self.id+2,data,mission)[0];
	local type_ = self:getPinValue(self.id+3,data,mission)[0];
	local model = self:getPinValue(self.id+4,data,mission)[0];
	local slot = self:getPinValue(self.id+5,data,mission)[0];
	assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");	
	ldyom.setLastNode(self.id);
	
	local modl;
	if type_ == 0 then
		modl = model;
		callOpcode(0x0247, {{model,"int"}});
		callOpcode(0x038B,{});
		while not callOpcode(0x0248, {{model,"int"}}) do
			wait(0);
		end
	else
		local model_n = ldyom.ID_Spec_Actors[model+1];
		print(model_n);
		ldyom.LoadSpecialChar(slot, model_n);
		modl = 290 + slot;
	end
	assert(callOpcode(0x056D, {{ped,"int"}}),"not found ped");
	ldyom.SetPedIndexModel(ped,modl);
	
	self:callOutputLinks(data, mission, self.id+6);
end

ldyom.nodeEditor.addNodeClass("Ped",Node);