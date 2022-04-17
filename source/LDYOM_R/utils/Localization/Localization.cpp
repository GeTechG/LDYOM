// ReSharper disable CppMemberFunctionMayBeStatic
#include "Localization.h"
#include <filesystem>

#include "Logger.h"
#include "Settings.h"
#include "fmt/core.h"

std::string language_path("LDYOM/Languages/");

void Localization::Init()
{
	const std::optional<std::string> language = Settings::getInstance().get<std::string>("main.language");
	loadLocalization("English");
	if (language.has_value())
		loadLocalization(language.value());
}

void Localization::unpackLocalization(std::string path, toml::table& table)
{
	for (const auto& table_value : table)
	{
		std::string name_loc = path + table_value.first;
		switch (table_value.second.type())
		{
		case toml::node_type::table:
			unpackLocalization(name_loc + ".", *table_value.second.as_table());
			break;
		case toml::node_type::array: 
			{
				std::vector<std::string> arrayLocalization;
				for (const auto& array_value : *table_value.second.as_array())
				{
					if (array_value.is_string())
					{
						arrayLocalization.push_back(array_value.as_string()->ref<std::string>());
					}
				}
				this->arrayLocalization_[name_loc] = arrayLocalization;
			}
			break;
		case toml::node_type::string:
			this->textLocalizations_[name_loc] = table_value.second.as_string()->ref<std::string>();
			break;
		default:
			Logger::getInstance().log(fmt::format("invalid read \"{}\"", name_loc));
			break;
		}
	}
}

void Localization::loadLocalization(const std::string& localizationName)
{
	toml::parse_result file;
	try {
		file = toml::parse_file(fmt::format("{}{}.toml", language_path, localizationName));
	}
	catch (toml::parse_error& err) {
		Logger::getInstance().log(err.what());
		return;
	}
	unpackLocalization("", file);
}

const std::string& Localization::get(const std::string& path)
{
	const auto loc = this->textLocalizations_.find(path);
	if (loc == this->textLocalizations_.end()) {
		return path;
	}
	return loc->second;
}

std::vector<std::string>& Localization::getArray(const std::string& path)
{
	const auto loc = this->arrayLocalization_.find(path);
	if (loc == this->arrayLocalization_.end()) {
		static std::vector<std::string> empty_array;
		return empty_array;
	}
	return loc->second;
}
