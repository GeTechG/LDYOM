// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindConversation(sol::state &state) {
  auto table = state.create_table("ConversationOp");
  // default
  table.set_function("startSettingUp", [](const int &_handle) { Command<0x0717>(_handle); });
  table.set_function("finishSettingUp", []() { Command<0x0719>(); });
  table.set_function("isAtNode", [](const int &_handle, const int &_speech) {
    auto result = Command<0x071A>(_handle, _speech);
    return std::make_tuple(result);
  });
  table.set_function("isPlayerInPosition", [](const int &_handle) {
    auto result = Command<0x089B>(_handle);
    return std::make_tuple(result);
  });
  table.set_function("enable", [](const int &_handle, const int &_state) { Command<0x089C>(_handle, _state); });
  table.set_function("clearForChar", [](const int &_handle) { Command<0x08ED>(_handle); });
  table.set_function("setUpNodeWithSpeech", [](const int &_question, const int &_positiveAnswer,
                                               const int &_negativeAnswer, const int &_questionSoundId,
                                               const int &_positiveAnswerSoundId, const int &_negativeAnswerSoundId) {
    Command<0x09A4>(_question, _positiveAnswer, _negativeAnswer, _questionSoundId, _positiveAnswerSoundId,
                    _negativeAnswerSoundId);
  });
  table.set_function("setUpEndNodeWithSpeech",
                     [](const int &_speech, const int &_speechSoundId) { Command<0x09AA>(_speech, _speechSoundId); });
  table.set_function("setUpNodeWithScriptedSpeech",
                     [](const int &_question, const int &_positiveAnswer, const int &_negativeAnswer,
                        const int &_questionSoundId, const int &_positiveAnswerSoundId,
                        const int &_negativeAnswerSoundId) {
                       Command<0x0A18>(_question, _positiveAnswer, _negativeAnswer, _questionSoundId,
                                       _positiveAnswerSoundId, _negativeAnswerSoundId);
                     });
  table.set_function("setUpEndNodeWithScriptedSpeech",
                     [](const int &_speech, const int &_speechSoundId) { Command<0x0A3C>(_speech, _speechSoundId); });
  table.set_function("finishSettingUpNoSubtitles", []() { Command<0x0A47>(); });
}