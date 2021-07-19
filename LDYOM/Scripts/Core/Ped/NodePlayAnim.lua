ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePlayAnim", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CorePlayAnim");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+11] = BasePin:new(self.id+11,imgui.imnodes.PinType.void, 0),
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",4)),
		[self.id+14] = BasePin:new(self.id+14,imgui.imnodes.PinType.void, 1),
		[self.id+15] = BasePin:new(self.id+15,imgui.imnodes.PinType.void, 1),
	};
end

function playNodePreviewAnimPed()
	if not callOpcode(0x04EE, {{ldyom.Anim_name[currNodePlayAnim.Pins[currNodePlayAnim.id+2].value[0]+1],"string"}}) then
		callOpcode(0x04ED, {{ldyom.Anim_name[currNodePlayAnim.Pins[currNodePlayAnim.id+2].value[0]+1],"string"}});
    end
    callOpcode(0x01B4, {{0,"int"}, {0,"int"}});
    local anims = ldyom.Anim_list[currNodePlayAnim.Pins[currNodePlayAnim.id+2].value[0]+1];
	print(ffi.string(anims[currNodePlayAnim.Pins[currNodePlayAnim.id+3].value[0]+1]));
	print(ffi.string(ldyom.Anim_name[currNodePlayAnim.Pins[currNodePlayAnim.id+2].value[0]+1]));
	callOpcode(0x0605,{{PLAYER_PED,"ped"}, {anims[currNodePlayAnim.Pins[currNodePlayAnim.id+3].value[0]+1],"string"}, {ldyom.Anim_name[currNodePlayAnim.Pins[currNodePlayAnim.id+2].value[0]+1],"string"}, {currNodePlayAnim.Pins[currNodePlayAnim.id+9].value[0],"float"}, {currNodePlayAnim.Pins[currNodePlayAnim.id+4].value[0],"bool"}, {false,"bool"}, {false,"bool"}, {false,"bool"}, {-1,"int"}});
end

function previewOnPos()
	callOpcode(0x01B4, {{0,"int"}, {1,"int"}});
    callOpcode(0x0512, {{"HENAA","string"}});
    while not isKeyJustPressed(0x59) do
        wait(0);
    end
    callOpcode(0x03E6,{});
    playNodePreviewAnimPed();
    callOpcode(0x0512, {{"HVIEW","string"}});
    while not isKeyJustPressed(0x46) do
        wait(0);
    end
	callOpcode(0x0605,{{PLAYER_PED,"ped"}, {"WALK_START","string"}, {"PED","string"}, {1.0,"float"}, {false,"bool"}, {false,"bool"}, {false,"bool"}, {false,"bool"}, {-1,"int"}});
    callOpcode(0x03E6,{});
    ldyom.set_off_gui(false);
    callOpcode(0x01B4, {{0,"int"}, {0,"int"}});
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
	
	imgui.imnodes.BeginInputAttribute(self.id+11);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text(ldyom.langt("pack"));
	if not self.Pins[self.id+2].link then
		imgui.SetNextItemWidth(150);
		if imgui.ComboVecStr("", self.Pins[self.id+2].value, ldyom.Anim_name) then
			self.Pins[self.id+3].value[0] = 0;
		end
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text(ldyom.langt("anim"));
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(150);
		if imgui.ComboVecStr("", self.Pins[self.id+3].value, ldyom.Anim_list[self.Pins[self.id+2].value[0]+1]) then
			currNodePlayAnim = self;
			playNodePreviewAnimPed();
		end
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	if not self.Pins[self.id+4].link then
		imgui.ToggleButton(ldyom.langt("looped"), self.Pins[self.id+4].value)
	else
		imgui.Text(ldyom.langt("looped"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	if not self.Pins[self.id+5].link then
		imgui.ToggleButton(ldyom.langt("unbreak"), self.Pins[self.id+5].value)
	else
		imgui.Text(ldyom.langt("unbreak"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	if not self.Pins[self.id+6].link then
		imgui.ToggleButton(ldyom.langt("stopEndFrameW"), self.Pins[self.id+6].value)
	else
		imgui.Text(ldyom.langt("stopEndFrameW"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+7);
	if not self.Pins[self.id+7].link then
		imgui.ToggleButton(ldyom.langt("useAnimMove"), self.Pins[self.id+7].value)
	else
		imgui.Text(ldyom.langt("useAnimMove"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+8);
	imgui.Text(ldyom.langt("time"));
	if not self.Pins[self.id+8].link then
		imgui.SetNextItemWidth(150);
		imgui.InputFloat("", self.Pins[self.id+8].value)
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+9);
	imgui.Text(ldyom.langt("smoothness"));
	if not self.Pins[self.id+9].link then
		imgui.SetNextItemWidth(150);
		imgui.InputFloat("", self.Pins[self.id+9].value)
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+10);
	if imgui.Button(ldyom.langt("preview"), imgui.ImVec2:new(200,20)) then
		currNodePlayAnim = self;
		playNodePreviewAnimPed();
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+12);
	if imgui.Button(ldyom.langt("previewPos"), imgui.ImVec2:new(200,20)) then
		currNodePlayAnim = self;
		ldyom.set_off_gui(true);
		addThread(previewOnPos);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+13);
	if imgui.Button(ldyom.langt("unPreview"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x0605,{{PLAYER_PED,"ped"}, {"WALK_START","string"}, {"PED","string"}, {1.0,"float"}, {false,"bool"}, {false,"bool"}, {false,"bool"}, {false,"bool"}, {-1,"int"}});
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+14);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("whenItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+15);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	addThreadObj(function(data_pack)
		local data = data_pack[1];
		local mission = data_pack[2];
		local node = data_pack[3];
		
		local ped = node:getPinValue(node.id+1,data,mission)[0];
		local pack = node:getPinValue(node.id+2,data,mission)[0];
		local anim = node:getPinValue(node.id+3,data,mission)[0];
		local looped = node:getPinValue(node.id+4,data,mission)[0];
		local unbreak = node:getPinValue(node.id+5,data,mission)[0];
		local stopEndFrame = node:getPinValue(node.id+6,data,mission)[0];
		local useAnimMove = node:getPinValue(node.id+7,data,mission)[0];
		local _time = node:getPinValue(node.id+8,data,mission)[0];
		local smoothness = node:getPinValue(node.id+9,data,mission)[0];
		assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");	
		ldyom.setLastNode(self.id);
		
		if not callOpcode(0x04EE, {{ldyom.Anim_name[pack+1],"string"}}) then
			callOpcode(0x04ED, {{ldyom.Anim_name[pack+1],"string"}});
			while not callOpcode(0x04EE, {{ldyom.Anim_name[pack+1],"string"}}) do
				wait(0);
			end
		end
		local anims = ldyom.Anim_list[pack+1];
		
		if not unbreak then
			callOpcode(0x0605,{{ped,"int"}, {anims[anim+1],"string"}, {ldyom.Anim_name[pack+1],"string"}, {smoothness,"float"}, {looped,"bool"}, {useAnimMove,"bool"}, {useAnimMove,"bool"}, {stopEndFrame,"bool"}, {fif(_time > 0,_time*1000,-1),"int"}});
        else
			callOpcode(0x088A,{{ped,"int"}, {anims[anim+1],"string"}, {ldyom.Anim_name[pack+1],"string"}, {smoothness,"float"}, {looped,"bool"}, {useAnimMove,"bool"}, {useAnimMove,"bool"}, {stopEndFrame,"bool"}, {fif(_time > 0,_time*1000,-1),"int"}, {false,"bool"}, {false,"bool"}});
        end
		if _time > 0 then
            wait(_time * 1000)
        else
            while ldyom.getMissionStarted() and not callOpcode(0x0611, {{ped,"int"}, {anims[anim+1], "string"}}) do
                wait(0);
            end
			local time_c = ffi.new("float[1]");
			callOpcode(0x061A, {{ped, "int"}, {anims[anim+1], "string"}, {time_c, "float*"}});
			print(tostring(time_c[0]));
            wait(math.floor(time_c[0]));
        end
		
		node:callOutputLinks(data, mission, node.id+14);
		
	end, {data,mission,self});
	
	self:callOutputLinks(data, mission, self.id+15);
end

ldyom.nodeEditor.addNodeClass("Ped",Node);