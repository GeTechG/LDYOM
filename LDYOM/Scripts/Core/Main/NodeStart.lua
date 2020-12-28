ffi = require "ffi"
require "LDYOM.Scripts.baseNode"
class = require "LDYOM.Scripts.middleclass"
require "LDYOM.Scripts.utf8"

Node = class("NodeStart", BaseNode);

Node.static.name = imgui.imnodes.getNodeIcon("event")..' '..ldyom.langt("CoreNodeStart");

function Node:initialize(id)
	BaseNode.initialize(self,id);
	self.type = 0;
	self.Pins = {
		[self.id+1] = BasePin:new(self.id+1,imgui.imnodes.PinType.void, 1)
	};
end

function Node:draw()
	imgui.imnodes.BeginNode(self.id,self.type)
	
	imgui.imnodes.BeginNodeTitleBar();
	imgui.Text(self.class.static.name);
	imgui.imnodes.EndNodeTitleBar();
	
	imgui.imnodes.BeginOutputAttribute(self.id+1);
	imgui.Dummy(imgui.ImVec2:new(0,20));
	imgui.imnodes.EndOutputAttribute();
	
	imgui.imnodes.EndNode();
	
end

function Node:play(data, mission)
	if self.Pins[self.id+1].links ~= nil then
		for k,v in pairs(self.Pins[self.id+1].links) do
			print(tostring(data.links));
			print(tostring(data.links[v]));
			local link = data.links[v];
			for k2,v2 in pairs(link) do
				local strr = Utf8ToAnsi(k2);
				if strr == "id_in" then
					local node_num = math.floor(v2 / 100)*100;
					data.nodes[node_num]:play(data);
					break;
				end
			end
		end
	end
end

ldyom.nodeEditor.addNodeClass("Main",Node);