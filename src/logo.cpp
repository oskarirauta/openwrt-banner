#include <fstream>
#include <sstream>
#include <streambuf>
#include <filesystem>
#include <algorithm>
#include <vector>

#include "logo.hpp"

bool banner::logo_error = false;

static std::string classic_logo(void) {

	return
		" _______                     ________        __   \n"
		"|       |.-----.-----.-----.|  |  |  |.----.|  |_ \n"
		"|   -   ||  _  |  -__|     ||  |  |  ||   _||   _|\n"
		"|_______||   __|_____|__|__||________||__|  |____|\n"
		"         |__| W I R E L E S S   F R E E D O M     \n";
}

static std::string default_logo(void) {

	return
		"\n"
		"  ░░░░░░  ░░░░░░░ ░░░░░░░ ░░░   ░░ ░░░  ░░░░░░░░░░  ░░░░░░░░░\n"
		" ░▒█████░ ░██▓███░░▓█████░███▄  ██░██░ ░░░█▓▒█████▄░▒█████▓▒░\n"
		"░▒██▒  ██▒▒██▒ ▀██▒▓█▒░░▀ ██▀█░ ██▒▓█░░█▒░█▓▒▓█▒ ▀██░ ▒██▒░░░\n"
		"░▒██░ ░██▒▒██░  █▓▒████░  ██░▀█ ██▒░█▒░█▒░█▒░▓█░ ▄█▀  ▒██▒░  \n"
		"░░██░ ░██░▒██▄▄█▓░▒▓█▒░░▄ ██▒ █▌██▒░█▒▒██░█▒▒██▀██▄   ░█▓▒░  \n"
		" ░▒████▓▒░▒██▒░░░░▒██████▒██░ ▀███░ ░██▒██░░░██ ░▒██░ ░██▒░  \n"
		"  ░░░▒▒░░ ░▓█▒░   ░░░░░▒░░▒░░  ░░▒░ ░░░░▒░  ░░▒  ░▒░░ ░░▒░░  \n"
		"                           W I R E L E S S    F R E E D O M  \n";
}

const std::string banner::logo(const std::string& filename, const bool& use_classic_logo) {

	banner::logo_error = false;
	std::string s(use_classic_logo ? classic_logo() : default_logo());

	if ( !filename.empty() && std::filesystem::exists(filename)) {

		if ( std::ifstream ifs(filename); ifs && ifs.is_open() && !ifs.fail() && ifs.good() && !ifs.eof()) {

			s = std::string((std::istreambuf_iterator<char>(ifs)),
					(std::istreambuf_iterator<char>()));

			if ( !s.empty() && s.back() == '\n' )
				s.erase(std::prev(s.end()));

			ifs.close();
		} else banner::logo_error = true;

	} else if ( !filename.empty()) banner::logo_error = true;

	if ( !s.empty()) {

		std::vector<std::string> lines;
		std::stringstream ss(s);
		std::string line;

		while ( std::getline(ss, line, '\n'))
			lines.push_back(line);

		if ( lines.back() == "" )
			lines.pop_back();

		s = "";
		for ( std::string l : lines )
			s += " " + l + "\n";

		s.pop_back();
	}

	return s;
}

size_t utf8len(std::string s) {

	return std::count_if(s.begin(), s.end(),
		[](char c) { return (static_cast<unsigned char>(c) & 0xC0) != 0x80; } );
}

const int banner::logo_width(const std::string& logo) {

	size_t len = 0;
	std::stringstream ss(logo);

	for ( std::string line; std::getline(ss, line, '\n');)
		if ( size_t usize = utf8len(line); usize > len ) len = usize;

	return int(len);
}

const std::string banner::separator(int width, const char& sep) {

	if ( width == 0 )
		return "";

	std::string res;
	while ( res.size() < size_t(width + 2))
		res += sep;
	return res;
}
