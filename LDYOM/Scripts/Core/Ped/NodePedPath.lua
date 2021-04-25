ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"

Node = bitser.registerClass(class("NodePedPath", BaseNode));
Node.static.mission = true;

Node.static.name = imgui.imnodes.getNodeIcon("func")..' '..ldyom.langt("CoreNodePedPath");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 4;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 0),
		[self.id+2] = BasePin:new(self.id+2,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+3] = BasePin:new(self.id+3,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+4] = BasePin:new(self.id+4,imgui.imnodes.PinType.number, 0, ffi.new("int[1]")),
		[self.id+5] = BasePin:new(self.id+5,imgui.imnodes.PinType.void, 1),
		[self.id+6] = BasePin:new(self.id+6,imgui.imnodes.PinType.void, 1),
	};
end

function editPath()
	callOpcode(0x01B4, {{0,"int"}, {1,"int"}});
	callOpcode(0x0512, {{"HAPA","string"}});
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
	imgui.Text(ldyom.langt("type_move_ped"));
	if not self.Pins[self.id+3].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+3].value, ldyom.langMenu["move_type_ped"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginInputAttribute(self.id+4);
	imgui.Text(ldyom.langt("type_route_ped"));
	if not self.Pins[self.id+4].link then
		imgui.SetNextItemWidth(200);
		imgui.ComboVecStr("", self.Pins[self.id+4].value, ldyom.langMenu["move_route_ped"]);
	end
	imgui.imnodes.EndInputAttribute();
	
	imgui.imnodes.BeginStaticAttribute(self.id+7);
	if imgui.Button(ldyom.langt("add_enter"), imgui.ImVec2:new(200,20)) then
		currNodePedPath = self;
		ldyom.set_off_gui(true);
		addThread(editPath);
	end
	imgui.imnodes.EndStaticAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+5);
	imgui.Indent(130);
	imgui.Text(ldyom.langt("whenItsDone"));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.BeginOutputAttribute(self.id+6);
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
		local type_move_ped = node:getPinValue(node.id+3,data,mission)[0];
        local type_route_ped = node:getPinValue(node.id+4,data,mission)[0];
		assert(callOpcode(0x056D, {{ped,"int"}}), "Not found ped");	
		ldyom.setLastNode(self.id);
		
		local type_walk = 4
        if type_move_ped == 2 then type_walk = 6
        elseif type_move_ped == 3 then type_walk = 7 end
		
		callOpcode(0x04EB, {{ped,"int"}, {fif(type_move_ped == 0,true,false),"bool"}});
		
		repeat
            for i = 1,#self.points do
                if not  callOpcode(0x056D, {{ped,"int"}}) then
                    return
                end
                local x1,y1,z1 = self.points[i][0],self.points[i][1],self.points[i][2]
				callOpcode(0x05D3, {{ped,"int"}, {x1,"float"}, {y1,"float"}, {z1,"float"}, {type_walk,"int"}, {-1,"int"}});
                local px = ffi.new("float[1]");
                local py = ffi.new("float[1]");
                local pz = ffi.new("float[1]");
				callOpcode(0x00A0, {{ped,"int"}, {px,"float*"}, {py,"float*"}, {pz,"float*"}});
				local distance = ffi.new("float[1]",9999);
                while distance[0] > 1 do
					callOpcode(0x050A, {{x1,"float"}, {y1,"float"}, {z1,"float"}, {px[0],"float"}, {py[0],"float"}, {pz[0],"float"}, {distance,"float*"}});
                    wait(0);
                    if callOpcode(0x056D, {{ped,"int"}}) then
                        callOpcode(0x00A0, {{ped,"int"}, {px,"float*"}, {py,"float*"}, {pz,"float*"}});
                    else
                        return
                    end
                end
            end
            wait(0)
        until type_route_ped == 0
		
		callOpcode(0x04EB, {{ped,"int"}, {false,"bool"}});
		
		local px = ffi.new("float[1]");
        local py = ffi.new("float[1]");
        local pz = ffi.new("float[1]");
		local distance = ffi.new("float[1]",9999);
		while distance[0] > 1 do
			wait(0);
			callOpcode(0x050A, {{self.points[#self.points][0],"float"}, {self.points[#self.points][1],"float"}, {self.points[#self.points][2],"float"}, {px[0],"float"}, {py[0],"float"}, {pz[0],"float"}, {distance,"float*"}});
			if callOpcode(0x056D, {{ped,"int"}}) then
                callOpcode(0x00A0, {{ped,"int"}, {px,"float*"}, {py,"float*"}, {pz,"float*"}});
            else
                return;
            end
		end
		
		
		node:callOutputLinks(data, mission, node.id+5);
		
	end, {data,mission,self});
	
	self:callOutputLinks(data, mission, self.id+6);
end

ldyom.nodeEditor.addNodeClass("Ped",Node);