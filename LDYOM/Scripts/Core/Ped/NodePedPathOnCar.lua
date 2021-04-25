ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePedPathOnCar", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodePedPathOnCar");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("float[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.boolean, 0, ffi.new("bool[1]")),
		[self.id+7] = BasePin:new(self.id+7,imgui.imnodes.PinType.void, 1),
		[self.id+8] = BasePin:new(self.id+8,imgui.imnodes.PinType.void, 1),
	};
end

function editPathCar()
	callOpcode(0x01B4, {{0,"int"}, {1,"int"}});
	callOpcode(0x0512, {{"HAPA","string"}});
	callOpcode(0x07A7, {{PLAYER_PED,"ped"}});
	currNodePedPath.points = {};
	while not isKeyJustPressed(0x4E) do
        wait(0);
		
		if isKeyJustPressed(0x59) then
			local x = ffi.new("float[1]");
			local y = ffi.new("float[1]");
			local z = ffi.new("float[1]");
			callOpcode(0x00A0, {{PLAYER_PED,"ped"}, {x,"float*"}, {y,"float*"}, {z,"float*"}});
			currNodePedPath.points[#currNodePedPath.points+1] = ffi.new("float[3]",x[0],y[0],z[0]);
			callOpcode(0x0ACC, {{GXTEncode(UTF8_to_CP1251(ldyom.langt("added_point")..tostring(#currNodePedPath.points))),"string"}, {1000,"int"}});
		end
    end
	callOpcode(0x03E6,{});
	callOpcode(0x048F, {{PLAYER_PED,"ped"}});
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
	
	imgui.imnodes.BeginInputAttribute(self.id+1);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+2);
	imgui.Text(ldyom.langt("ped"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+3);
	imgui.Text(ldyom.langt("CoreHandleCar"));
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("speed"));
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.InputFloat("", self.Pins[self.id+4].value);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+5);
	imgui.Text(ldyom.langt("type_route_ped"));
	if not self.Pins[self.id+5].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+5].value, ldyom.langMenu["move_route_ped"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+6);
	if not self.Pins[self.id+6].link then
		imgui.ToggleButton(ldyom.langt("CoreAgressive"), self.Pins[self.id+6].value);
	else
		imgui.Text(ldyom.langt("CoreAgressive"));
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+9);
	if imgui.Button(ldyom.langt("add_enter"), imgui.ImVec2:new(200,20)) then
		currNodePedPath = self;
		ldyom.set_off_gui(true);
		addThread(editPathCar);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+7);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("whenItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+8);
	imgui.Dummy(imgui.ImVec2:new(0,10));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	addThreadObj(function(data_pack)
		local data = data_pack[1];
		local mission = data_pack[2];
		local node = data_pack[3];
		
		local ped = node:getPinValue(node.id+2,data,mission)[0];
		local car = node:getPinValue(node.id+3,data,mission)[0];
        local speed = node:getPinValue(node.id+4,data,mission)[0];
        local type_route_ped = node:getPinValue(node.id+5,data,mission)[0];
        local agressive = node:getPinValue(node.id+6,data,mission)[0];
		assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");	
		ldyom.setLastNode(self.id);
		
		local px = ffi.new("float[1]");
        local py = ffi.new("float[1]");
        local pz = ffi.new("float[1]");
		callOpcode(0x00AA, {{car,"int"}, {px,"float*"}, {py,"float*"}, {pz,"float*"}});
		callOpcode(0x03AB, {{car,"int"}, {true,"bool"}});
		
		repeat
            for i = 1,#self.points do
                if not  callOpcode(0x056D, {{ped,"int"}}) then
                    return
                end
                local x1,y1,z1 = self.points[i][0],self.points[i][1],self.points[i][2]
				callOpcode(0x05D1, {{ped,"int"}, {car,"int"}, {x1,"float"}, {y1,"float"}, {z1,"float"}, {speed,"float"}, {0,"int"}, {0,"int"}, {fif(agressive,2,0),"int"}});
				--callOpcode(0x02C2, {{car,"int"}, {x1,"float"}, {y1,"float"}, {z1,"float"}});
				callOpcode(0x00AA, {{car,"int"}, {px,"float*"}, {py,"float*"}, {pz,"float*"}});
				local distance = ffi.new("float[1]",9999);
                while distance[0] > 5 do
					callOpcode(0x050A, {{x1,"float"}, {y1,"float"}, {z1,"float"}, {px[0],"float"}, {py[0],"float"}, {pz[0],"float"}, {distance,"float*"}});
                    wait(0);
                    if callOpcode(0x056E, {{car,"int"}}) then
                        callOpcode(0x00AA, {{car,"int"}, {px,"float*"}, {py,"float*"}, {pz,"float*"}});
                    else
                        return
                    end
                end
            end
            wait(0)
        until type_route_ped == 0
		
		local distance = ffi.new("float[1]",9999);
		while distance[0] > 5 do
			wait(0);
			callOpcode(0x050A, {{self.points[#self.points][0],"float"}, {self.points[#self.points][1],"float"}, {self.points[#self.points][2],"float"}, {px[0],"float"}, {py[0],"float"}, {pz[0],"float"}, {distance,"float*"}});
			if callOpcode(0x056E, {{car,"int"}}) then
                callOpcode(0x00AA, {{car,"int"}, {px,"float*"}, {py,"float*"}, {pz,"float*"}});
            else
                return;
            end
		end
		callOpcode(0x03AB, {{car,"int"}, {false,"bool"}});
		
		node:callOutputLinks(data, mission, node.id+7);
		
	end, {data,mission,self});
	
	self:callOutputLinks(data, mission, self.id+8);
end

ldyom.nodeEditor.addNodeClass("Ped",Node);