#pragma once
#include <list>
#include <sol.hpp>

enum NodeType {
	Function,
	Loops,
	Event,
	Condition,
	Variable,
	Constant
};

enum ValueType {
	Void = 0,
	Number,
	Boolean,
	String
};

inline const char* toString(const ValueType e) {
	switch (e) {
	case Void:
		return "Void";
	case Number:
		return "Number";
	case Boolean:
		return "Boolean";
	case String:
		return "String";
	default:
		return "unknown";
	}
}

enum PinType {
    In = 0,
    Out = 1
};

class NodeEditorUtils {
public:
	static void beginNode(const int id, const char* name, const NodeType& nodeType, std::optional<std::string> description);
    static void beginInputAttribute(sol::table pin);
    static void beginOutputAttribute(sol::table pin);
	static void endInputAttribute();
	static void endOutputAttribute();
	static void endNode();
	static unsigned getColorByType(const ValueType& valueType);
	static const char* getIcon(const NodeType& nodeType);
};
