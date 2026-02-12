#pragma once

// Re-registers the built-in math/utility nodes (Number, Add, Multiply, Output, Text)
// through the NodeRegistry so they appear in the context-menu alongside Lua nodes.
void registerBuiltinNodes();
