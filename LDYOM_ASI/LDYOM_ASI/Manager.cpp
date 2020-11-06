#include "Manager.h"

#include "Init.h"
#include "boost/filesystem.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/utility.hpp>
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
	ofstream save_file(full_path, ios::binary);
	/*uint32_t compressedSize;
	auto compressedBlob = lzmaCompress(reinterpret_cast<const uint8_t*>(save.str().c_str()), save.str().capacity() + 1, &compressedSize);
	std::string str(reinterpret_cast<const char*>(compressedBlob.get()),compressedSize);*/
	save_file << gzip::compress(save.str().data(), save.str().size(), Z_BEST_COMPRESSION);
	save_file.close();
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
}

Mission* Manager::LoadMission(std::string& path)
{
	Mission* return_miss;

	ifstream save_file(path, ios::binary);
	std::stringstream encoded_save;
	encoded_save << save_file.rdbuf();
	save_file.close();
	std::stringstream save;
	save << gzip::decompress(encoded_save.str().data(), encoded_save.str().size());
	boost::archive::binary_iarchive ia(save);
	ia >> return_miss;
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
	ofstream save_file(full_path, ios::binary);
	save_file << gzip::compress(save.str().data(), save.str().size(), Z_BEST_COMPRESSION);
	save_file.close();
}

void Manager::LoadStoryline(int curr_storyline)
{
	std::string name_storyline = replace_symb(UTF8_to_CP1251(namesStorylines[curr_storyline]));
	std::string full_path = path_s + name_storyline + ".bin";

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