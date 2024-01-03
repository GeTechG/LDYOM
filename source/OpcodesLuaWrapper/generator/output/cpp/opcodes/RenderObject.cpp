// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindRenderObject(sol::state &state) {
  auto table = state.create_table("RenderObjectOp");
  // CLEO+
  table.set_function("createToCharBone",
                     [](const int &_char, const int &_modelId, const int &_pedBone, const float &_x, const float &_y,
                        const float &_z, const float &_rx, const float &_ry, const float &_rz) {
                       int handle_;
                       Command<0x0E2E>(_char, _modelId, _pedBone, _x, _y, _z, _rx, _ry, _rz, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("delete", [](const int &_self) { Command<0x0E2F>(_self); });
  table.set_function("setVisible", [](const int &_self, const int &_visible) { Command<0x0E31>(_self, _visible); });
  table.set_function("setPosition", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0E35>(_self, _x, _y, _z);
  });
  table.set_function("setRotation", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0E36>(_self, _x, _y, _z);
  });
  table.set_function("setScale", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0E37>(_self, _x, _y, _z);
  });
  table.set_function("setDistortion", [](const int &_self, const float &_x, const float &_y, const float &_z,
                                         const float &_w) { Command<0x0E3A>(_self, _x, _y, _z, _w); });
}