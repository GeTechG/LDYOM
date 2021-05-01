#include "Manager.h"

#include <CHud.h>


#include "NodeGraph.h"
#include "boost/filesystem.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/utility.hpp>

#include "ScriptManager.h"
#include "lzma/Lzma2Dec.h"
#include "lzma/Lzma2Enc.h"
#include "gzip/compress.hpp"
#include "gzip/decompress.hpp"

const std::string path = "LDYOM//Missions_packs//";
const std::string path_s = "LDYOM//Storylines//";
const std::string path_backup = "LDYOM//Backup//";

extern Mission* currentMissionPtr;
extern std::string* nameCurrPack;
extern vector<std::string> namesMissionPacks;
extern vector<vector<std::string>> namesMissions;
extern const unsigned int VERSION;
extern std::string UTF8_to_CP1251(std::string const& utf8);
extern std::string replace_symb(std::string& str);
extern vector<std::string> namesStorylines;
extern Storyline* currentStorylinePtr;
extern NodeGraph* currentNodeGraphPtr;
extern std::map<int, const char*> namesVars;

#define LDYOM_VER 71;

void Manager::SaveMission(int curr_pack,int curr_miss)
{
	if (!boost::filesystem::exists(path_backup))
		assert(boost::filesystem::create_directory(path_backup));
	std::string path_pack = replace_symb(UTF8_to_CP1251(namesMissionPacks[curr_pack])) + "//";
	std::string name_miss = replace_symb(UTF8_to_CP1251(namesMissions[curr_pack][curr_miss]));
	std::stringstream old_save;
	std::string full_path = path + path_pack + name_miss + ".bin";
	if (boost::filesystem::exists(full_path))
	{
		std::string full_path_buckup = path_backup + name_miss + "_" + std::to_string(std::time(nullptr)) + ".bin";
		boost::filesystem::copy_file(full_path, full_path_buckup);
	}
	std::stringstream save;
	boost::archive::binary_oarchive oa(save);
	oa << currentMissionPtr;

	bool scripts = true;

	for (auto node : currentNodeGraphPtr->nodes) {
		ImVec2 pos = ImNodes::GetNodeEditorSpacePos(node.first);
		node.second["pos"]["x"] = pos.x;
		node.second["pos"]["y"] = pos.y;
	}
	
	for (auto pair : ScriptManager::lua_scripts)
	{
		if (pair.first)
		{
			scripts = false;
			sol::protected_function func = pair.second["serilize"];
			if (func.valid())
			{
				
				auto result = func(sol::as_table(currentNodeGraphPtr->nodes));
				if (ScriptManager::checkProtected(result))
				{
					std::string nodes_bytes = result;
					
					result = func(sol::as_table(currentNodeGraphPtr->links));
					if (ScriptManager::checkProtected(result)) {
						std::string links_bytes = result;

						oa << nodes_bytes;
						oa << links_bytes;
						scripts = true;
					}
				}
				
			}
		}
	}

	if (scripts) {
		ofstream save_file(full_path, ios::binary);
		/*uint32_t compressedSize;
		auto compressedBlob = lzmaCompress(reinterpret_cast<const uint8_t*>(save.str().c_str()), save.str().capacity() + 1, &compressedSize);
		std::string str(reinterpret_cast<const char*>(compressedBlob.get()),compressedSize);*/
		save_file << gzip::compress(save.str().data(), save.str().size(), Z_BEST_COMPRESSION);
		save_file.close();

		SaveVars(namesMissionPacks[curr_pack]);
	} else
	{
		CHud::SetHelpMessage("Error, see log.", false, false, false);
		CHud::DrawHelpText();
	}
}

void Manager::SaveListMission(int curr_pack)
{
	const std::string full_path = path + UTF8_to_CP1251(replace_symb(namesMissionPacks[curr_pack])) + "//list.bin";
	ofstream save_file(full_path, ios::binary);
	boost::archive::binary_oarchive oa(save_file);
	oa << namesMissions[curr_pack];
	save_file.close();
}

void Manager::LoadMission(int curr_pack, int curr_miss)
{
	std::string path_pack = replace_symb(UTF8_to_CP1251(namesMissionPacks[curr_pack])) + "//";
	std::string name_miss = replace_symb(UTF8_to_CP1251(namesMissions[curr_pack][curr_miss]));
	std::string full_path = path + path_pack + name_miss + ".bin";

	ifstream save_file(full_path, ios::binary);
	std::stringstream encoded_save;
	encoded_save << save_file.rdbuf();
	save_file.close();
	std::stringstream save;
	save << gzip::decompress(encoded_save.str().data(), encoded_save.str().size());
	boost::archive::binary_iarchive ia(save);
	delete currentMissionPtr;
	ia >> currentMissionPtr;

	currentNodeGraphPtr->nodes.clear();
	currentNodeGraphPtr->links.clear();
	
	for (auto pair : ScriptManager::lua_scripts)
	{
		if (pair.first)
		{
			std::string nodes_bytes;
			std::string links_bytes;
			ia >> nodes_bytes;
			ia >> links_bytes;
			
			sol::protected_function func = pair.second["deserilize"];
			if (func.valid())
			{
				auto result = func(nodes_bytes);				
				if (ScriptManager::checkProtected(result))
				{
					sol::table nodes_result = result;
					for (auto pair : nodes_result)
					{
						unsigned idx = pair.first.as<unsigned>();
						currentNodeGraphPtr->nodes[idx] = pair.second;
					}

					result = func(links_bytes);
					if (ScriptManager::checkProtected(result)) {
						sol::table links_result = result;

						for (auto pair : links_result)
						{
							unsigned idx = pair.first.as<unsigned>();
							currentNodeGraphPtr->links[idx] = pair.second;
						}
					}
				}

			}
		}
	}

	for (auto node : currentNodeGraphPtr->nodes) {
		float x = node.second["pos"]["x"];
		float y = node.second["pos"]["y"];
		ImNodes::SetNodeEditorSpacePos(node.first,ImVec2(x,y));
	}
	LoadVars(namesMissionPacks[curr_pack]);
}

std::pair<Mission*, NodeGraph*> Manager::LoadMission(std::string& path)
{
	std::pair<Mission*, NodeGraph*> return_miss;
	return_miss.second = new NodeGraph();

	ifstream save_file(path, ios::binary);
	std::stringstream encoded_save;
	encoded_save << save_file.rdbuf();
	save_file.close();
	std::stringstream save;
	save << gzip::decompress(encoded_save.str().data(), encoded_save.str().size());
	boost::archive::binary_iarchive ia(save);
	ia >> return_miss.first;

	for (auto pair : ScriptManager::lua_scripts)
	{
		if (pair.first)
		{
			std::string nodes_bytes;
			std::string links_bytes;
			ia >> nodes_bytes;
			ia >> links_bytes;

			sol::protected_function func = pair.second["deserilize"];
			if (func.valid())
			{
				auto result = func(nodes_bytes);
				if (ScriptManager::checkProtected(result))
				{
					sol::table nodes_result = result;
					for (auto pair : nodes_result)
					{
						unsigned idx = pair.first.as<unsigned>();
						return_miss.second->nodes[idx] = pair.second;
					}

					result = func(links_bytes);
					if (ScriptManager::checkProtected(result)) {
						sol::table links_result = result;

						for (auto pair : links_result)
						{
							unsigned idx = pair.first.as<unsigned>();
							return_miss.second->links[idx] = pair.second;
						}
					}
				}

			}
		}
	}
	
	return return_miss;
}

void Manager::LoadListMission(int curr_pack)
{
	const std::string full_path = path + UTF8_to_CP1251(replace_symb(namesMissionPacks[curr_pack])) + "//list.bin";
	ifstream save_file(full_path, ios::binary);
	boost::archive::binary_iarchive ia(save_file);
	ia >> namesMissions[curr_pack];
	save_file.close();
}

void Manager::SaveStoryline(int curr_storyline)
{
	std::string name_storyline = replace_symb(UTF8_to_CP1251(namesStorylines[curr_storyline]));
	std::stringstream old_save;
	std::string full_path = path_s + name_storyline + ".bin";
	if (boost::filesystem::exists(full_path))
	{
		std::string full_path_buckup = path_backup + name_storyline + "_s" + std::to_string(std::time(nullptr)) + ".bin";
		boost::filesystem::copy_file(full_path, full_path_buckup);
	}
	std::stringstream save;
	boost::archive::binary_oarchive oa(save);
	oa << currentStorylinePtr;

	bool scripts = true;

	for (auto node : currentNodeGraphPtr->nodes) {
		ImVec2 pos = ImNodes::GetNodeEditorSpacePos(node.first);
		node.second["pos"]["x"] = pos.x;
		node.second["pos"]["y"] = pos.y;
	}

	for (auto pair : ScriptManager::lua_scripts)
	{
		if (pair.first)
		{
			scripts = false;
			sol::protected_function func = pair.second["serilize"];
			if (func.valid())
			{

				auto result = func(sol::as_table(currentNodeGraphPtr->nodes));
				if (ScriptManager::checkProtected(result))
				{
					std::string nodes_bytes = result;

					result = func(sol::as_table(currentNodeGraphPtr->links));
					if (ScriptManager::checkProtected(result)) {
						std::string links_bytes = result;

						oa << nodes_bytes;
						oa << links_bytes;
						scripts = true;
					}
				}

			}
		}
	}
	
	if (scripts) {
		ofstream save_file(full_path, ios::binary);
		/*uint32_t compressedSize;
		auto compressedBlob = lzmaCompress(reinterpret_cast<const uint8_t*>(save.str().c_str()), save.str().capacity() + 1, &compressedSize);
		std::string str(reinterpret_cast<const char*>(compressedBlob.get()),compressedSize);*/
		save_file << gzip::compress(save.str().data(), save.str().size(), Z_BEST_COMPRESSION);
		save_file.close();

		SaveVars(currentStorylinePtr->missPack);
	}
	else
	{
		CHud::SetHelpMessage("Error, see log.", false, false, false);
		CHud::DrawHelpText();
	}
}

void Manager::LoadStoryline(int curr_storyline)
{
	std::string name_storyline = replace_symb(UTF8_to_CP1251(namesStorylines[curr_storyline]));
	std::string full_path = path_s + name_storyline + ".bin";

	currentNodeGraphPtr->nodes.clear();
	currentNodeGraphPtr->links.clear();
	
	ifstream save_file(full_path, ios::binary);
	std::stringstream encoded_save;
	encoded_save << save_file.rdbuf();
	save_file.close();
	std::stringstream save;
	save << gzip::decompress(encoded_save.str().data(), encoded_save.str().size());
	boost::archive::binary_iarchive ia(save);
	if (currentStorylinePtr != nullptr)
		delete currentStorylinePtr;
	ia >> currentStorylinePtr;

	currentNodeGraphPtr->nodes.clear();
	currentNodeGraphPtr->links.clear();

	for (auto pair : ScriptManager::lua_scripts)
	{
		if (pair.first)
		{
			std::string nodes_bytes;
			std::string links_bytes;
			ia >> nodes_bytes;
			ia >> links_bytes;

			sol::protected_function func = pair.second["deserilize"];
			if (func.valid())
			{
				auto result = func(nodes_bytes);
				if (ScriptManager::checkProtected(result))
				{
					sol::table nodes_result = result;
					for (auto pair : nodes_result)
					{
						unsigned idx = pair.first.as<unsigned>();
						currentNodeGraphPtr->nodes[idx] = pair.second;
					}

					result = func(links_bytes);
					if (ScriptManager::checkProtected(result)) {
						sol::table links_result = result;

						for (auto pair : links_result)
						{
							unsigned idx = pair.first.as<unsigned>();
							currentNodeGraphPtr->links[idx] = pair.second;
						}
					}
				}

			}
		}
	}

	for (auto node : currentNodeGraphPtr->nodes) {
		float x = node.second["pos"]["x"];
		float y = node.second["pos"]["y"];
		ImNodes::SetNodeEditorSpacePos(node.first, ImVec2(x, y));
	}
	LoadVars(currentStorylinePtr->missPack);
}

void Manager::SaveVars(std::string missPack)
{
	std::string path_pack = replace_symb(UTF8_to_CP1251(missPack)) + "//";
	std::string full_path = path + path_pack + "vars.bin";

	ofstream save_file(full_path, ios::binary);

	bool scripts = true;
	std::string vars_bytes;
	
	for (auto pair : ScriptManager::lua_scripts)
	{
		if (pair.first)
		{
			scripts = false;
			sol::protected_function func = pair.second["serilize"];
			if (func.valid())
			{

				auto result = func(sol::as_table(currentNodeGraphPtr->vars));
				if (ScriptManager::checkProtected(result))
				{
					scripts = true;
					vars_bytes = result;
				}

			}
		}
	}

	if (scripts) {
		save_file << vars_bytes;
		save_file.close();
	} else
	{
		CHud::SetHelpMessage("Error, see log.", false, false, false);
		CHud::DrawHelpText();
	}
}

void Manager::LoadVars(std::string missPack)
{
	std::string path_pack = replace_symb(UTF8_to_CP1251(missPack)) + "//";
	std::string full_path = path + path_pack + "vars.bin";
	currentNodeGraphPtr->vars.clear();
	namesVars.clear();

	if (!boost::filesystem::exists(full_path))
		return;

	ifstream save_file(full_path, ios::binary);
	std::stringstream vars;
	vars << save_file.rdbuf();
	save_file.close();

	currentNodeGraphPtr->vars.clear();
	namesVars.clear();
	
	for (auto pair : ScriptManager::lua_scripts)
	{
		if (pair.first)
		{
			std::string vars_bytes = vars.str();

			sol::protected_function func = pair.second["deserilize"];
			if (func.valid())
			{
				auto result = func(vars_bytes);
				if (ScriptManager::checkProtected(result))
				{
					sol::table nodes_result = result;
					for (auto pair : nodes_result)
					{
						unsigned idx = pair.first.as<unsigned>();
						currentNodeGraphPtr->vars[idx] = pair.second;
						sol::table var = pair.second;
						sol::object name_var = var["var_name"];
						namesVars[idx] = (const char*)name_var.pointer();
					}
				}

			}
		}
	}
}

namespace boost {
	namespace serialization {

		template <>
		struct version<Mission>
		{
			BOOST_STATIC_CONSTANT(unsigned int, value = LDYOM_VER);
		};
	} // namespace serialization
} // namespace boost