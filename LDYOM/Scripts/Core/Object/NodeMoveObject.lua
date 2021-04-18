ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodeMoveObject", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodeMoveObject");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	local x = ffi.new("float[1]");
	local y = ffi.new("float[1]");
	local z = ffi.new("float[1]");
	callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("float[3]",{x[0],y[0],z[0]})),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[3]",{0,0,0})),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("float[1]",1)),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.void, 1),
		[self.id+9] = BasePin:new(self.id+9,imgui.imnodes.PinType.void, 1),
	};
end

function moveObject()
	ldyom.moveObject(currNodeMoveObject.Pins[currNodeMoveObject.id+2].value[0], currNodeMoveObject.Pins[currNodeMoveObject.id+3].value, currNodeMoveObject.Pins[currNodeMoveObject.id+4].value);
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
	
	imgui.imnodes.BeginStaticAttribute(self.id+2);
	local names = ldyom.namesObjects;
	imgui.Text(ldyom.langt("object"));
	imgui.SetNextItemWidth(150);
	imgui.ComboVecChars("",self.Pins[self.id+2].value,names);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+3);
	imgui.Text(ldyom.langt("position"));
	imgui.SetNextItemWidth(200);
	imgui.InputFloat3("", self.Pins[self.id+3].value);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+4);
	imgui.Text(ldyom.langt("rotate"));
	imgui.SetNextItemWidth(200);
	imgui.InputFloat3("", self.Pins[self.id+4].value);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text(ldyom.langt("time"));
	imgui.SetNextItemWidth(200);
	imgui.InputFloat("", self.Pins[self.id+5].value);
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+6);
	local names = ldyom.langMenu["CoreTypeMovingObject"];
	imgui.Text(ldyom.langt("type"));
	imgui.SetNextItemWidth(200);
	imgui.ComboVecStr("",self.Pins[self.id+6].value,names);
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+7);
	if imgui.Button(ldyom.langt("edithand"), imgui.ImVec2:new(200,20)) then
		callOpcode(0x01B4, {{0,"int"}, {0,"int"}});
		currNodeMoveObject = self;
		ldyom.set_off_gui(true);
		addThread(moveObject);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+8);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("whenItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+9);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	addThreadObj(function(data_pack)
		local data = data_pack[1];
		local mission = data_pack[2];
		local node = data_pack[3];
	
		local object = node:getPinValue(node.id+2,data,mission)[0];
		local pos = node:getPinValue(node.id+3,data,mission);
		local rot = node:getPinValue(node.id+4,data,mission);
		ldyom.setLastNode(node.id);
		assert(mission.list_objects[object+1].missionObject,"The object is not yet established or has already disappeared.");
		
		local last_time = time_ms();
		local duration = node:getPinValue(node.id+5,data,mission)[0] * 1000;
		local _type = node:getPinValue(node.id+6,data,mission)[0];
		local curr_time = 0;
		local obj = getObjectRef(mission.list_objects[object+1].missionObject);
		
		local start_x = ffi.new("float[1]");
		local start_y = ffi.new("float[1]");
		local start_z = ffi.new("float[1]");
		callOpcode(0x01BB, {{obj, "int"}, {start_x,"float*"}, {start_y,"float*"}, {start_z,"float*"}});
		
		local start_rot_x = ffi.new("float[1]");
		local start_rot_y = ffi.new("float[1]");
		local start_rot_z = ffi.new("float[1]");
		getOrientationObject(mission.list_objects[object+1].missionObject, start_rot_x, start_rot_y, start_rot_z);
		print(tostring(start_rot_z[0]));
		
		local _reverse = 1;
		while (ldyom.getMissionStarted()) do
			if (not mission.list_objects[object+1].missionObject) then
				break;
			end
			
			if (_type == 0 and curr_time >= duration) then
				break;
			elseif (_type == 1 and curr_time >= duration) then
				curr_time = 0;
			elseif (_type == 2 and (curr_time >= duration or curr_time <= 0)) then
				_reverse = _reverse * -1;
			end
			
			local new_time = time_ms();
			curr_time = curr_time + (new_time - last_time) * _reverse;
			last_time = new_time;
			
			local delta = curr_time / duration;
			
			local delta_x = start_x[0]*(1-delta) + pos[0]*delta;
			local delta_y = start_y[0]*(1-delta) + pos[1]*delta;
			local delta_z = start_z[0]*(1-delta) + pos[2]*delta;
			callOpcode(0x01BC,{{obj,"int"}, {delta_x,"float"}, {delta_y,"float"}, {delta_z,"float"}});
			
			local delta_rot_x = start_rot_x[0]*(1-delta) + rot[0]*delta;
			local delta_rot_y = start_rot_y[0]*(1-delta) + rot[1]*delta;
			local delta_rot_z = start_rot_z[0]*(1-delta) + rot[2]*delta;
			callOpcode(0x0453, {{obj,"int"}, {delta_rot_x,"float"}, {delta_rot_y,"float"}, {delta_rot_z,"float"}})
		
			wait(0);
		end
		
		node:callOutputLinks(data, mission, node.id+8);
		
	end, {data,mission,self});
	
	self:callOutputLinks(data, mission, self.id+9);
end

ldyom.nodeEditor.addNodeClass("Object",Node);