#include <sol.hpp>

#include "GlobalVariablesService.h"

void globalVariableWrapper(sol::state &state) {
	state.new_enum("GlobalVariableType",
	               "Float", GlobalVariableType::Float,
	               "Int", GlobalVariableType::Int,
	               "String", GlobalVariableType::String,
	               "Bool", GlobalVariableType::Bool
	);
}
