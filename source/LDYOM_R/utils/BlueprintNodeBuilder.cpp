﻿#include <sol.hpp>

#include "node_editor/blueprint/utilities/builders.h"

namespace ex = ax::NodeEditor;

void blueprintNodeBuilder(sol::state &state) {
	state.new_usertype<ex::Utilities::BlueprintNodeBuilder>("BlueprintNodeBuilder",
	                                                        sol::constructors<ex::Utilities::BlueprintNodeBuilder()>(),
	                                                        "Begin", &ex::Utilities::BlueprintNodeBuilder::Begin,
	                                                        "End", &ex::Utilities::BlueprintNodeBuilder::End,
	                                                        "Header", &ex::Utilities::BlueprintNodeBuilder::Header,
	                                                        "EndHeader",
	                                                        &ex::Utilities::BlueprintNodeBuilder::EndHeader,
	                                                        "Input", &ex::Utilities::BlueprintNodeBuilder::Input,
	                                                        "EndInput", &ex::Utilities::BlueprintNodeBuilder::EndInput,
	                                                        "Middle", &ex::Utilities::BlueprintNodeBuilder::Middle,
	                                                        "Output", &ex::Utilities::BlueprintNodeBuilder::Output,
	                                                        "EndOutput", &ex::Utilities::BlueprintNodeBuilder::EndOutput
	);
}
