#pragma once
#include <string>
#include <sol/sol.hpp>
#include <map>


#include "imgui.h"
#include "imnodes.h"
#include "Init.h"

class NodeGraph
{
public:
	std::map<unsigned, sol::table> nodes;
	std::map<int, sol::table> links;

	static sol::state baseNode;

	NodeGraph();
	
	enum PinType
	{
		number = 0,
		string,
		boolean,
		void_
	};
	
	static void addNodeClass(const std::string& category, sol::table& class_);
	static unsigned int getID();

	static void BeginInputAttribute(int id, sol::object shape);
	static void EndInputAttribute();

	static void BeginOutputAttribute(int id, sol::object shape);
	static void EndOutputAttribute();

	static void BeginNode(int id, unsigned short type);
	static void EndNode();

	static std::string getNodeIcon(std::string icon);

	static void callAllNodeName(std::string& name, NodeGraph* data, Mission* mission);
	
	static void render();

	static void callNode(sol::table& node, NodeGraph* data, Mission* mission);
};

