#pragma once
#include <string>
#include <sol/sol.hpp>
#include <map>


#include "imgui.h"
#include "imnodes.h"
#include "Init.h"
#include <boost/serialization/split_member.hpp>

class NodeGraph {
public:
	std::map<unsigned, sol::table> nodes;
	std::map<int, sol::table> links;
	std::map<int, sol::table> vars;

	static sol::state baseNode;

	sol::state luaGraph;


	int lastNode = -1;
	int curr_var = 0;

	NodeGraph();
	
	enum PinType
	{
		number = 0,
		string,
		boolean,
		void_
	};
	
	static void addNodeClass(const std::string& category, sol::table claz);
	static unsigned int getID();

	static void BeginInputAttribute(int id, sol::object shape);
	static void EndInputAttribute();

	static void BeginOutputAttribute(int id, sol::object shape);
	static void EndOutputAttribute();

	static void BeginNode(int id, unsigned short type);
	static void EndNode();

	static std::string getNodeIcon(std::string icon);

	static void callAllNodeName(std::string& name, NodeGraph* data, Mission* mission);
	static void callAllNodeNameS(std::string& name, NodeGraph* data, Storyline* storyline);
	
	static void render();

	static void callNode(sol::table& node, NodeGraph* data, Mission* mission);
	static void callNodeS(sol::table& node, NodeGraph* data, Storyline* storyline);
};

