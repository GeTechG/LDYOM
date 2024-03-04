// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindTxd(sol::state &state) {
  auto table = state.create_table("TxdOp");
  // default
  table.set_function(
      "loadSprite", [](const int &_memorySlot, const char *_spriteName) { Command<0x038F>(_memorySlot, _spriteName); });
  table.set_function("loadDictionary", [](const char *_name) { Command<0x0390>(_name); });
  table.set_function("remove", []() { Command<0x0391>(); });

  // CLEO+
  table.set_function("drawTexturePlus",
                     [](const int &_rwTextureOrSprite, const int &_drawEvent, const float &_posX, const float &_posY,
                        const float &_sizeX, const float &_sizeY, const float &_angle, const float &_depth,
                        const bool &_fixAr, const int &_maskVertCount, const int &_maskVertArray, const int &_red,
                        const int &_green, const int &_blue, const int &_alpha) {
                       Command<0x0E1E>(_rwTextureOrSprite, _drawEvent, _posX, _posY, _sizeX, _sizeY, _angle, _depth,
                                       _fixAr, _maskVertCount, _maskVertArray, _red, _green, _blue, _alpha);
                     });
  table.set_function("getTextureFromSprite", [](const int &_sprite) {
    int rwTexture_;
    Command<0x0E3C>(_sprite, &rwTexture_);
    return std::make_tuple(rwTexture_);
  });
}