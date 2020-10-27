/*
 * imguistyleloader.h by lenormf
 */

/*
 * Example of style-file supported:
 *

[text]
    value = [1.00, 1.00, 1.00, 1.00]

    [text.disabled]
        value = [0.50, 0.50, 0.50, 1.00]

[window.bg]
    value = [0.06, 0.06, 0.06, 0.94]

[alpha]
    value = 1

[window.padding]
    value = [8, 8]

 */

#ifndef LDYOM_IMGUILOADER_H
#define LDYOM_IMGUILOADER_H

#include <string>
#include <unordered_map>

#include "cpptoml.h"

#ifdef IMGUI_SL_DEBUG
#include <cstdio>
#define DEBUG_LOGTEXT(fmt, ...) std::printf("[SL] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_LOGTEXT(fmt, ...)
#endif

#define LOGTEXT(fmt, ...) ImGui::LogText("[SL] " fmt "\n", ##__VA_ARGS__)

#define VAR_VALUE_NAME "value"

#define IMGUI_SL_VERSION_NUM 17101

namespace ImGui { namespace StyleLoader {

// Type of variables instantiated by ImGui supported by this header
enum NodeType {
    Color,
    Variable,
};

// Data-types of the options initialized in the style file
enum NodeValueType {
    Float,
    Vector2Float,
    Vector4Float,
};

struct NodeMeta {
    int id;                   // Identifier of the option as declared by ImGui (ImGuiCol_, ImGuiStyleVar_)
    NodeType type;       // Type of option
    NodeValueType value_type; // Type of the value of the option
    size_t target_offset;     // Offset of the instance of the option (instantiated by ImGui) within the ImGuiStyle class
};

// Global index of fully qualified option names supported,
// with their corresponding meta-data as described above
const static std::unordered_map<std::string, NodeMeta> g_nodes_meta = {
    {"text",                    {ImGuiCol_Text,                     Color,    Vector4Float, 0}},
    {"text.disabled",           {ImGuiCol_TextDisabled,             Color,    Vector4Float, 0}},
    {"window.bg",               {ImGuiCol_WindowBg,                 Color,    Vector4Float, 0}},
    {"child.bg",                {ImGuiCol_ChildBg,                  Color,    Vector4Float, 0}},
    {"popup.bg",                {ImGuiCol_PopupBg,                  Color,    Vector4Float, 0}},
    {"border",                  {ImGuiCol_Border,                   Color,    Vector4Float, 0}},
    {"border.shadow",           {ImGuiCol_BorderShadow,             Color,    Vector4Float, 0}},
    {"frame.bg",                {ImGuiCol_FrameBg,                  Color,    Vector4Float, 0}},
    {"frame.bg.hovered",        {ImGuiCol_FrameBgHovered,           Color,    Vector4Float, 0}},
    {"frame.bg.active",         {ImGuiCol_FrameBgActive,            Color,    Vector4Float, 0}},
    {"title.bg",                {ImGuiCol_TitleBg,                  Color,    Vector4Float, 0}},
    {"title.bg.active",         {ImGuiCol_TitleBgActive,            Color,    Vector4Float, 0}},
    {"title.bg.collapsed",      {ImGuiCol_TitleBgCollapsed,         Color,    Vector4Float, 0}},
    {"menu.bar.bg",             {ImGuiCol_MenuBarBg,                Color,    Vector4Float, 0}},
    {"scrollbar.bg",            {ImGuiCol_ScrollbarBg,              Color,    Vector4Float, 0}},
    {"scrollbar.grab",          {ImGuiCol_ScrollbarGrab,            Color,    Vector4Float, 0}},
    {"scrollbar.grab.hovered",  {ImGuiCol_ScrollbarGrabHovered,     Color,    Vector4Float, 0}},
    {"scrollbar.grab.active",   {ImGuiCol_ScrollbarGrabActive,      Color,    Vector4Float, 0}},
    {"check.mark",              {ImGuiCol_CheckMark,                Color,    Vector4Float, 0}},
    {"slider.grab",             {ImGuiCol_SliderGrab,               Color,    Vector4Float, 0}},
    {"slider.grab.active",      {ImGuiCol_SliderGrabActive,         Color,    Vector4Float, 0}},
    {"button",                  {ImGuiCol_Button,                   Color,    Vector4Float, 0}},
    {"button.hovered",          {ImGuiCol_ButtonHovered,            Color,    Vector4Float, 0}},
    {"button.active",           {ImGuiCol_ButtonActive,             Color,    Vector4Float, 0}},
    {"header",                  {ImGuiCol_Header,                   Color,    Vector4Float, 0}},
    {"header.hovered",          {ImGuiCol_HeaderHovered,            Color,    Vector4Float, 0}},
    {"header.active",           {ImGuiCol_HeaderActive,             Color,    Vector4Float, 0}},
    {"separator",               {ImGuiCol_Separator,                Color,    Vector4Float, 0}},
    {"separator.hovered",       {ImGuiCol_SeparatorHovered,         Color,    Vector4Float, 0}},
    {"separator.active",        {ImGuiCol_SeparatorActive,          Color,    Vector4Float, 0}},
    {"resize.grip",             {ImGuiCol_ResizeGrip,               Color,    Vector4Float, 0}},
    {"resize.grip.hovered",     {ImGuiCol_ResizeGripHovered,        Color,    Vector4Float, 0}},
    {"resize.grip.active",      {ImGuiCol_ResizeGripActive,         Color,    Vector4Float, 0}},
    {"tab",                     {ImGuiCol_Tab,                      Color,    Vector4Float, 0}},
    {"tab.hovered",             {ImGuiCol_TabHovered,               Color,    Vector4Float, 0}},
    {"tab.active",              {ImGuiCol_TabActive,                Color,    Vector4Float, 0}},
    {"tab.unfocused",           {ImGuiCol_TabUnfocused,             Color,    Vector4Float, 0}},
    {"tab.unfocused.active",    {ImGuiCol_TabUnfocusedActive,       Color,    Vector4Float, 0}},
    {"plot.lines",              {ImGuiCol_PlotLines,                Color,    Vector4Float, 0}},
    {"plot.lines.hovered",      {ImGuiCol_PlotLinesHovered,         Color,    Vector4Float, 0}},
    {"plot.histogram",          {ImGuiCol_PlotHistogram,            Color,    Vector4Float, 0}},
    {"plot.histogram.hovered",  {ImGuiCol_PlotHistogramHovered,     Color,    Vector4Float, 0}},
    {"text.selected.bg",        {ImGuiCol_TextSelectedBg,           Color,    Vector4Float, 0}},
    {"drag.drop.target",        {ImGuiCol_DragDropTarget,           Color,    Vector4Float, 0}},
    {"nav.highlight",           {ImGuiCol_NavHighlight,             Color,    Vector4Float, 0}},
    {"nav.windowing.highlight", {ImGuiCol_NavWindowingHighlight,    Color,    Vector4Float, 0}},
    {"nav.windowing.dim.bg",    {ImGuiCol_NavWindowingDimBg,        Color,    Vector4Float, 0}},
    {"modal.window.dim.bg",     {ImGuiCol_ModalWindowDimBg,         Color,    Vector4Float, 0}},
    {"alpha",                   {ImGuiStyleVar_Alpha,               Variable, Float,        IM_OFFSETOF(ImGuiStyle, Alpha)}},
    {"window.padding",          {ImGuiStyleVar_WindowPadding,       Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, WindowPadding)}},
    {"window.rounding",         {ImGuiStyleVar_WindowRounding,      Variable, Float,        IM_OFFSETOF(ImGuiStyle, WindowRounding)}},
    {"window.border.size",      {ImGuiStyleVar_WindowBorderSize,    Variable, Float,        IM_OFFSETOF(ImGuiStyle, WindowBorderSize)}},
    {"window.min.size",         {ImGuiStyleVar_WindowMinSize,       Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, WindowMinSize)}},
    {"window.title.align",      {ImGuiStyleVar_WindowTitleAlign,    Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, WindowTitleAlign)}},
    {"child.rounding",          {ImGuiStyleVar_ChildRounding,       Variable, Float,        IM_OFFSETOF(ImGuiStyle, ChildRounding)}},
    {"child.border.size",       {ImGuiStyleVar_ChildBorderSize,     Variable, Float,        IM_OFFSETOF(ImGuiStyle, ChildBorderSize)}},
    {"popup.rounding",          {ImGuiStyleVar_PopupRounding,       Variable, Float,        IM_OFFSETOF(ImGuiStyle, PopupRounding)}},
    {"popup.border.size",       {ImGuiStyleVar_PopupBorderSize,     Variable, Float,        IM_OFFSETOF(ImGuiStyle, PopupBorderSize)}},
    {"frame.padding",           {ImGuiStyleVar_FramePadding,        Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, FramePadding)}},
    {"frame.rounding",          {ImGuiStyleVar_FrameRounding,       Variable, Float,        IM_OFFSETOF(ImGuiStyle, FrameRounding)}},
    {"frame.border.size",       {ImGuiStyleVar_FrameBorderSize,     Variable, Float,        IM_OFFSETOF(ImGuiStyle, FrameBorderSize)}},
    {"item.spacing",            {ImGuiStyleVar_ItemSpacing,         Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, ItemSpacing)}},
    {"item.inner.spacing",      {ImGuiStyleVar_ItemInnerSpacing,    Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, ItemInnerSpacing)}},
    {"indent.spacing",          {ImGuiStyleVar_IndentSpacing,       Variable, Float,        IM_OFFSETOF(ImGuiStyle, IndentSpacing)}},
    {"scrollbar.size",          {ImGuiStyleVar_ScrollbarSize,       Variable, Float,        IM_OFFSETOF(ImGuiStyle, ScrollbarSize)}},
    {"scrollbar.rounding",      {ImGuiStyleVar_ScrollbarRounding,   Variable, Float,        IM_OFFSETOF(ImGuiStyle, ScrollbarRounding)}},
    {"grab.min.size",           {ImGuiStyleVar_GrabMinSize,         Variable, Float,        IM_OFFSETOF(ImGuiStyle, GrabMinSize)}},
    {"grab.rounding",           {ImGuiStyleVar_GrabRounding,        Variable, Float,        IM_OFFSETOF(ImGuiStyle, GrabRounding)}},
    {"tab.rounding",            {ImGuiStyleVar_TabRounding,         Variable, Float,        IM_OFFSETOF(ImGuiStyle, TabRounding)}},
    {"button.text.align",       {ImGuiStyleVar_ButtonTextAlign,     Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, ButtonTextAlign)}},
    {"selectable.text.align",   {ImGuiStyleVar_SelectableTextAlign, Variable, Vector2Float, IM_OFFSETOF(ImGuiStyle, SelectableTextAlign)}},
};

// Recursive function that takes a fully qualified node name along
// with the corresponding node instance, parses the "value" option instantiated within said node (table),
// and recursively handles sub-nodes
static inline bool _handle_node(std::string const &fqn, const std::shared_ptr<cpptoml::base> &current_node) {
	ImGuiStyle * im_style = &ImGui::GetStyle();
	ImVec4 * im_colors = im_style->Colors;

	DEBUG_LOGTEXT("Current node: %s", fqn.c_str());

	auto table = current_node->as_table();

    // The only variable name allowed is "value", and it should be set within a table
    if (!current_node->is_table()) {
        LOGTEXT("Node is not a table: %s", fqn.c_str());
        return false;
    }

    // The fully qualified option names that correspond to ImGui options are stored in the map above
    auto meta_it = g_nodes_meta.find(fqn);
    if (meta_it != g_nodes_meta.end()) {
        auto const &node_meta = meta_it->second;

        if (!table->contains(VAR_VALUE_NAME)) {
            DEBUG_LOGTEXT("Node doesn't contain a \"%s\" variable, ignoring", VAR_VALUE_NAME);
            return true;
        }

        if (node_meta.type == NodeType::Color && node_meta.value_type != NodeValueType::Vector4Float)
            IM_ASSERT(false); // Colors only take a vector of 4 floating integers for argument

        // Depending on the expected type of the option value in the configuration file,
        // fetch the value,
        // then create a target pointer pointing at an ImGui variable that will hold the value
        switch (node_meta.value_type) {
            case NodeValueType::Float: {
                if (auto value = table->get_as<double>(VAR_VALUE_NAME)) {
                    float *target = (float*)((unsigned char*)im_style + node_meta.target_offset);

                    DEBUG_LOGTEXT("Setting floating point value: %.3f", *value);
                    *target = (float)*value;
                } else {
                    LOGTEXT("Node value is not a floating integer: %s", fqn.c_str());
                    return false;
                }
                break;
            }

            case NodeValueType::Vector2Float: {
                if (auto value = table->get_array_of<double>(VAR_VALUE_NAME)) {
                    auto const &values = *value;

                    if (values.size() != 2) {
                        LOGTEXT("Node value is not a double vector of floating integers: %s", fqn.c_str());
                        return false;
                    }

                    ImVec2 const new_value = {(float)values[0], (float)values[1]};
                    if (node_meta.type == NodeType::Variable) {
                        ImVec2 *target = (ImVec2*)((unsigned char*)im_style + node_meta.target_offset);

                        DEBUG_LOGTEXT("Setting floating point value: [%.3f, %.3f]", new_value.x, new_value.y);
                        *target = new_value;
                    }
                } else {
                    LOGTEXT("Node value is not a double vector of floating integers: %s", fqn.c_str());
                    return false;
                }
                break;
            }

            case NodeValueType::Vector4Float: {
                if (auto value = table->get_array_of<double>(VAR_VALUE_NAME)) {
                    auto const &values = *value;

                    if (values.size() != 4) {
                        LOGTEXT("Node value is not a quadruple vector of floating integers: %s", fqn.c_str());
                        return false;
                    }

                    ImVec4 const new_value = {(float)values[0], (float)values[1], (float)values[2], (float)values[3]};
                    if (node_meta.type == NodeType::Variable) {
                        ImVec4 *target = (ImVec4*)((unsigned char*)im_style + node_meta.target_offset);

                        DEBUG_LOGTEXT("Setting floating point value: [%.3f, %.3f, %.3f, %.3f]", new_value.x, new_value.y, new_value.z, new_value.w);
                        *target = new_value;
                    } else if (node_meta.type == NodeType::Color) {
                        DEBUG_LOGTEXT("Setting floating point value: [%.3f, %.3f, %.3f, %.3f]", new_value.x, new_value.y, new_value.z, new_value.w);
                        im_colors[node_meta.id] = new_value;
                    }
                } else {
                    LOGTEXT("Node value is not a quadruple vector of floating integers: %s", fqn.c_str());
                    return false;
                }
                break;
            }

            default: IM_ASSERT(false); // The nodes meta table holds a type of node that is not handled at runtime
        }
    } else {
        DEBUG_LOGTEXT("Node not configurable, ignoring");
    }

    // Iterator over the descendant of the current table,
    // which also have to be tables
    for (auto const &node : *table) {
        if (!node.second->is_table())
            continue;

        // Append to the name of the current node to complete the fully qualified name
        std::string const node_name = fqn + "." + node.first;
        if (!_handle_node(node_name, node.second))
            return false;
    }

    return true;
}

// Load the file pointed to by the path passed as first argument,
// and return false if any errors have occurred (parsing, logic)
static bool LoadFile(std::string const &path) {
    try {
        auto const config = cpptoml::parse_file(path);

        // Iterate over all root-level nodes,
        // and pass them to a recursive function that will traverse their descendants
        // and handle their value directly
        for (auto const &node : *config)
            if (!_handle_node(node.first, node.second))
                return false;
    } catch (cpptoml::parse_exception const &e) {
        LOGTEXT("%s", e.what());
        return false;
    }

    return true;
}

} }

#endif //LDYOM_IMGUILOADER_H
