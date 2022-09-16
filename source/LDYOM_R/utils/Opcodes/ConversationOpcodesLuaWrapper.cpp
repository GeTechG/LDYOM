//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ConversationOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ConversationOp");
    //default
	table.set_function("startSettingUp", [](const int& _handle) { auto result = Command<0x0717>(_handle); return std::make_tuple(result);});
	table.set_function("finishSettingUp", []() { auto result = Command<0x0719>(); return std::make_tuple(result);});
	table.set_function("isAtNode", [](const int& _handle, const char* _speech) { auto result = Command<0x071A>(_handle, _speech); return std::make_tuple(result);});
	table.set_function("isPlayerInPosition", [](const int& _handle) { auto result = Command<0x089B>(_handle); return std::make_tuple(result);});
	table.set_function("enable", [](const int& _handle, const bool& _state) { auto result = Command<0x089C>(_handle, _state); return std::make_tuple(result);});
	table.set_function("clearForChar", [](const int& _handle) { auto result = Command<0x08ED>(_handle); return std::make_tuple(result);});
	table.set_function("setUpNodeWithSpeech", [](const char* _question, const char* _positiveAnswer, const char* _negativeAnswer, const int& _questionSoundId, const int& _positiveAnswerSoundId, const int& _negativeAnswerSoundId) { auto result = Command<0x09A4>(_question, _positiveAnswer, _negativeAnswer, _questionSoundId, _positiveAnswerSoundId, _negativeAnswerSoundId); return std::make_tuple(result);});
	table.set_function("setUpEndNodeWithSpeech", [](const char* _speech, const int& _speechSoundId) { auto result = Command<0x09AA>(_speech, _speechSoundId); return std::make_tuple(result);});
	table.set_function("setUpNodeWithScriptedSpeech", [](const char* _question, const char* _positiveAnswer, const char* _negativeAnswer, const int& _questionSoundId, const int& _positiveAnswerSoundId, const int& _negativeAnswerSoundId) { auto result = Command<0x0A18>(_question, _positiveAnswer, _negativeAnswer, _questionSoundId, _positiveAnswerSoundId, _negativeAnswerSoundId); return std::make_tuple(result);});
	table.set_function("setUpEndNodeWithScriptedSpeech", [](const char* _speech, const int& _speechSoundId) { auto result = Command<0x0A3C>(_speech, _speechSoundId); return std::make_tuple(result);});
	table.set_function("finishSettingUpNoSubtitles", []() { auto result = Command<0x0A47>(); return std::make_tuple(result);});

}