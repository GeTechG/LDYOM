// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindPickup(sol::state &state) {
  auto table = state.create_table("PickupOp");
  // default
  table.set_function(
      "create", [](const int &_modelId, const int &_pickupType, const float &_x, const float &_y, const float &_z) {
        int handle_;
        Command<0x0213>(_modelId, _pickupType, _x, _y, _z, &handle_);
        return std::make_tuple(handle_);
      });
  table.set_function("hasBeenCollected", [](const int &_self) {
    auto result = Command<0x0214>(_self);
    return std::make_tuple(result);
  });
  table.set_function("remove", [](const int &_self) { Command<0x0215>(_self); });
  table.set_function("createMoney", [](const float &_x, const float &_y, const float &_z, const int &_cashAmount,
                                       const bool &_permanent) {
    int handle_;
    Command<0x02E1>(_x, _y, _z, _cashAmount, _permanent, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("createWithAmmo", [](const int &_modelId, const int &_pickupType, const int &_ammo,
                                          const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x032B>(_modelId, _pickupType, _ammo, _x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("createProtection", [](const float &_x, const float &_y, const float &_z, const int &_revenueLimit,
                                            const int &_revenueRate) {
    int handle_;
    Command<0x04A6>(_x, _y, _z, _revenueLimit, _revenueRate, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("createLockedProperty",
                     [](const float &_x, const float &_y, const float &_z, const char *_message) {
                       int handle_;
                       Command<0x0517>(_x, _y, _z, _message, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("createForSaleProperty",
                     [](const float &_x, const float &_y, const float &_z, const int &_price, const char *_message) {
                       int handle_;
                       Command<0x0518>(_x, _y, _z, _price, _message, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("getCoordinates", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x065B>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("updateMoneyPerDay", [](const int &_self, const int &_value) { Command<0x094A>(_self, _value); });
  table.set_function("createSnapshot", [](const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x0958>(_x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("createHorseshoe", [](const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x0959>(_x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("createOyster", [](const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x095A>(_x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("doesExist", [](const int &_handle) {
    auto result = Command<0x09D1>(_handle);
    return std::make_tuple(result);
  });

  // CLEO+
  table.set_function("getModel", [](const int &_self) {
    int modelId_;
    Command<0x0E34>(_self, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("getPointer", [](const int &_self) {
    int pointer_;
    Command<0x0E38>(_self, &pointer_);
    return std::make_tuple(pointer_);
  });
  table.set_function("getType", [](const int &_self) {
    int type_;
    Command<0x0E39>(_self, &type_);
    return std::make_tuple(type_);
  });
}