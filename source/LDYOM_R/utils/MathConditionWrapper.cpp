#include <sol.hpp>

#include "../Data/MathCondition.h"

void mathConditionWrapper(sol::state &state) {
	state.new_enum("MathCondition",
	               "Equal", Equal,
	               "NotEqual", NotEqual,
	               "More", More,
	               "MoreOrEqual", MoreOrEqual,
	               "Less", Less,
	               "LessOrEqual", LessOrEqual
	);
}
