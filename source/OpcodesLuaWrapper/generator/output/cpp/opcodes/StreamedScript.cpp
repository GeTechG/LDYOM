// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindStreamedScript(sol::state &state) {
  auto table = state.create_table("StreamedScriptOp");
  // default
  table.set_function("registerScriptBrainForCodeUse",
                     [](const int &_id, const char *__p2) { Command<0x07D3>(_id, __p2); });
  table.set_function("registerAttractorScriptBrainForCodeUse",
                     [](const int &_id, const char *__p2) { Command<0x0884>(_id, __p2); });
  table.set_function("stream", [](const int &_id) { Command<0x08A9>(_id); });
  table.set_function("hasLoaded", [](const int &_id) {
    auto result = Command<0x08AB>(_id);
    return std::make_tuple(result);
  });
  table.set_function("markAsNoLongerNeeded", [](const int &_id) { Command<0x090F>(_id); });
  table.set_function("remove", [](const int &_id) { Command<0x0910>(_id); });
  table.set_function("startNew", [](const int &_id, const int &_args) { Command<0x0913>(_id, _args); });
  table.set_function("getNumberOfInstances", [](const int &_id) {
    int numScripts_;
    Command<0x0926>(_id, &numScripts_);
    return std::make_tuple(numScripts_);
  });
  table.set_function("allocateToRandomPed", [](const int &_id, const int &_modelId, const int &_priority) {
    Command<0x0928>(_id, _modelId, _priority);
  });
  table.set_function("allocateToObject",
                     [](const int &_id, const int &_modelId, const int &_priority, const float &_radius,
                        const int &_type) { Command<0x0929>(_id, _modelId, _priority, _radius, _type); });
}