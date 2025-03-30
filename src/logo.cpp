#include <fstream>
#include <sstream>
#include <streambuf>
#include <filesystem>
#include <algorithm>
#include <exception>
#include <vector>
#include <map>

#include "common.hpp"
#include "logo.hpp"

#include <iostream>

bool banner::logo_error = false;

static std::map<std::string, LOGO> logo_names = {
	{ "NONE", LOGO::NO_LOGO }, { "NO_LOGO", LOGO::NO_LOGO },
	{ "CLASSIC", LOGO::CLASSIC },
	{ "CLASSIC2", LOGO::CLASSIC2 },
	{ "TINY", LOGO::TINY },
	{ "SMALL", LOGO::SMALL },
	{ "THIN", LOGO::THIN },
	{ "SIMPLE", LOGO::SIMPLE },
	{ "ROUND", LOGO::ROUND },
	{ "GRAFFITI", LOGO::GRAFFITI },
	{ "SPORT", LOGO::SPORT },
	{ "PAINT", LOGO::PAINT },
	{ "BLOCKY", LOGO::BLOCKY }, { "DEFAULT", LOGO::BLOCKY }
};

static std::map<std::string, SUBTITLE> subtitle_names = {
	{ "NONE", SUBTITLE::NO_SUBTITLE }, { "NO_SUBTEXT", SUBTITLE::NO_SUBTITLE }, { "NO_SUBTITLE", SUBTITLE::NO_SUBTITLE },
	{ "DEFAULT", SUBTITLE::NO_SUBTITLE },
	{ "PLAIN", SUBTITLE::PLAIN },
	{ "EXTENDED", SUBTITLE::EXTENDED },
	{ "THINTEXT", SUBTITLE::THINTEXT },
	{ "ITALY", SUBTITLE::ITALY },
	{ "BOLD", SUBTITLE::BOLD },
	{ "LARGE", SUBTITLE::LARGE }
};

static std::map<LOGO, std::string> logos = {

	{ LOGO::NO_LOGO, "" },

	{ LOGO::CLASSIC,
		" _______                     ________        __   \n"
		"|       |.-----.-----.-----.|  |  |  |.----.|  |_ \n"
		"|   -   ||  _  |  -__|     ||  |  |  ||   _||   _|\n"
		"|_______||   __|_____|__|__||________||__|  |____|\n"
		"         |__| W I R E L E S S   F R E E D O M     \n"
	},

	{ LOGO::CLASSIC2,
		"  ░░░░░░  ░░░░░░░ ░░░░░░░ ░░░   ░░ ░░░  ░░░░░░░░░░  ░░░░░░░░░\n"
		" ░▒█████░ ░██▓███░░▓█████░███▄  ██░██░ ░░░█▓▒█████▄░▒█████▓▒░\n"
		"░▒██▒  ██▒▒██▒ ▀██▒▓█▒░░▀ ██▀█░ ██▒▓█░░█▒░█▓▒▓█▒ ▀██░ ▒██▒░░░\n"
		"░▒██░ ░██▒▒██░  █▓▒████░  ██░▀█ ██▒░█▒░█▒░█▒░▓█░ ▄█▀  ▒██▒░  \n"
		"░░██░ ░██░▒██▄▄█▓░▒▓█▒░░▄ ██▒ █▌██▒░█▒▒██░█▒▒██▀██▄   ░█▓▒░  \n"
		" ░▒████▓▒░▒██▒░░░░▒██████▒██░ ▀███░ ░██▒██░░░██ ░▒██░ ░██▒░  \n"
		"  ░░░▒▒░░ ░▓█▒░   ░░░░░▒░░▒░░  ░░▒░ ░░░░▒░  ░░▒  ░▒░░ ░░▒░░  \n"
		"                           W I R E L E S S    F R E E D O M  \n"
	},

	{ LOGO::TINY,
		"╔═╗──────╔╦═╦╗─╔╗ \n"
		"║║╠═╦═╦═╦╣║║║╠╦╣╚╗\n"
		"║║║╬║╩╣║║║║║║║╔╣╔╣\n"
		"╚═╣╔╩═╩╩═╩═╩═╩╝╚═╝\n"
		"──╚╝              \n"
	},

	{ LOGO::SMALL,
		"▒█▀▀▀█ █▀▀█ █▀▀ █▀▀▄ ▒█░░▒█ █▀▀█ ▀▀█▀▀\n"
		"▒█░░▒█ █░░█ █▀▀ █░░█ ▒█▒█▒█ █▄▄▀ ░░█░░\n"
		"▒█▄▄▄█ █▀▀▀ ▀▀▀ ▀░░▀ ▒█▄▀▄█ ▀░▀▀ ░░▀░░\n"
	},

	{ LOGO::THIN,
		"░▒█▀▀▀█░▄▀▀▄░█▀▀░█▀▀▄░▒█░░▒█░█▀▀▄░▀█▀\n"
		"░▒█░░▒█░█▄▄█░█▀▀░█░▒█░▒█▒█▒█░█▄▄▀░░█░\n"
		"░▒█▄▄▄█░█░░░░▀▀▀░▀░░▀░▒▀▄▀▄▀░▀░▀▀░░▀░\n"
	},

	{ LOGO::SIMPLE,
		" _______                       ________        __   \n"
		"|       |.-----..-----..-----.|  |  |  |.----.|  |_ \n"
		"|   -   ||  _  ||  -__||     ||  |  |  ||   _||   _|\n"
		"|_______||   __||_____||__|__||________||__|  |____|\n"
		"         |__|                                       \n"
	},

	{ LOGO::ROUND,
		" .--.                   .-.   .-.      .-. \n"
		": ,. :                  : :.-.: :     .' `.\n"
		": :: :.---.  .--. ,-.,-.: :: :: :.--. `. .'\n"
		": :; :: .; `' '_.': ,. :: `' `' ;: ..' : : \n"
		"`.__.': ._.'`.__.':_;:_; `.,`.,' :_;   :_; \n"
		"      : :                                  \n"
		"      :_;                                  \n"
	},

	{ LOGO::GRAFFITI,
		"       ▄▄▄·▄▄▄ . ▐ ▄ ▄▄▌ ▐ ▄▌▄▄▄  ▄▄▄▄▄\n"
		" ▄█▀▄ ▐█ ▄█▀▄.▀·•█▌▐███· █▌▐█▀▄ █·•██  \n"
		"▐█▌.▐▌ ██▀·▐▀▀▪▄▐█▐▐▌██▪▐█▐▐▌▐▀▀▄  ▐█.▪\n"
		"▐█▌.▐▌▐█▪·•▐█▄▄▌██▐█▌▐█▌██▐█▌▐█•█▌ ▐█▌·\n"
		" ▀█▄▀▪.▀    ▀▀▀ ▀▀ █▪ ▀▀▀▀ ▀▪.▀  ▀ ▀▀▀ \n"
	},

	{ LOGO::SPORT,
		"░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
		"░░░░░     ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░   ░░░░░░░░   ░░░░░░░░░░░   ░░░░\n"
		"▒▒▒   ▒▒▒▒   ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒   ▒▒▒▒▒▒▒▒▒▒▒   ▒▒▒▒\n"
		"▒   ▒▒▒▒▒▒▒▒   ▒  ▒   ▒▒▒▒▒▒   ▒▒▒▒▒   ▒   ▒▒▒   ▒▒▒  ▒▒▒   ▒  ▒    ▒       ▒▒\n"
		"▓   ▓▓▓▓▓▓▓▓   ▓  ▓▓   ▓▓▓  ▓▓▓   ▓▓▓   ▓▓   ▓   ▓▓   ▓▓▓   ▓▓   ▓▓▓▓▓▓   ▓▓██\n"
		"▓   ▓▓▓▓▓▓▓▓   ▓  ▓▓▓   ▓         ▓▓▓   ▓▓   ▓   ▓  ▓   ▓   ▓▓   ▓▓▓▓▓▓   ▓▓██\n"
		"▓▓▓   ▓▓▓▓▓   ▓▓   ▓   ▓▓  ▓▓▓▓▓▓▓▓▓▓   ▓▓   ▓  ▓  ▓▓▓▓     ▓▓   ▓▓▓▓▓▓   ▓███\n"
		"█████     ██████   ████████     ████    ██   █   ████████   █    ███████   ███\n"
		"████████████████   ███████████████████████████████████████████████████████████\n"
		"▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n"
	},

	{ LOGO::PAINT,
		" ▄█████▄ ██████▄ ██████ ██    ██ ██     ██ ██████▄ █████████\n"
		" ██  ▀██ ██  ▀██ ██     ██▄   ██ ██     ██ ██  ▀██ ▀▀▀███▀▀▀\n"
		" ██   ██ ██   ██ ██     ████  ██ ██▄   ▄██ ██   ██    ███   \n"
		" ██   ██ ██  ▄██ █████  ███   ██ ███ ▄ ███ ██  ▄█▀    ███   \n"
		" ▒▒   ▒▒ ▒▒▒▒▒▒░ ▒▒▒▒▒  ▒▒ ▒▒ ▒▒ ▒▒▒ ▒ ▒▒▒ ▒▒▒▒▒▒     ▒▒▒   \n"
		" ▒▒   ▒▒ ▒▒      ▒▒     ▒▒ ▒▒ ▒▒ ░▒▒░▒░▒▒░ ▒▒  ▒▒░    ▒▒▒   \n"
		" ▒▒░  ▒▒ ▒▒      ▒▒     ▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒   ▒▒    ▒▒▒   \n"
		"  ░░░░░  ░░      ░░░░░░ ░░   ░░░   ░ ░ ░   ░░   ░░    ░░░   \n"
	},

	{ LOGO::BLOCKY,
		"  ░░░░░░░░  ░░░░░░░░░  ░░░░░░░░░ ░░░░   ░░░░░ ░░░░░░   ░░░░░░ ░░░░░░░░░░  ░░░░░░░░░░\n"
		" ░░▒████▒░░ ░▒█████▒░░ ░▒█████▒░ ░▒██   ▒██▒░ ░▒██▒░   ░▒██▒░ ░▒██████▒░░ ░▒██████▒░\n"
		" ░▒█░  ░█▒░ ░▒█▒░ ▒█▒░ ░▒██▒░    ░▒██▄  ▒██▒░ ░▒██▒░   ░▒██▒░ ░▒█▒░  ▒█▒░ ░░░▒██▒░░░\n"
		" ░▒█░  ░█▒░ ░▒█▒░ ▒█▒░ ░▒█████▒░ ░▒████▄▒██▒░ ░▒██▒░▒█▒░▒██▒░ ░▒█▒░  ▒█▒░   ░▒██▒░  \n"
		" ░▒█░  ░█▒░ ░▒█████▒░░ ░▒██▒░    ░▒██▒▀████▒░ ░▒███▒███▒███▒░ ░▒██████▒░    ░▒██▒░  \n"
		" ░░▒████▒░░ ░▒██▒░░░░  ░▒█████▒░ ░▒██▒  ███▒░ ░░▒████▒████▒░░ ░▒██▒░▒██▒░   ░▒██▒░  \n"
		"  ░░░░░░░░  ░░░░░░░    ░░░░░░░░░ ░░░░░  ░░░░░  ░░░░░░ ░░░░░░  ░░░░░░░░░░░   ░░░░░░  \n"
	}
};

static std::map<SUBTITLE, std::string> subtitles = {

	{ SUBTITLE::NO_SUBTITLE, "" },
	{ SUBTITLE::PLAIN, "Wireless freedom" },
	{ SUBTITLE::EXTENDED, "W I R E L E S S   F R E E D O M " },
	{ SUBTITLE::THINTEXT, "𝖶 𝗂𝗋𝖾𝗅𝖾𝗌𝗌 𝖿𝗋𝖾𝖾𝖽𝗈𝗆" },
	{ SUBTITLE::ITALY, "𝙒 𝙞𝙧𝙚𝙡𝙚𝙨𝙨 𝙛𝙧𝙚𝙚𝙙𝙤𝙢" },
	{ SUBTITLE::BOLD, "𝗪 𝗶𝗿𝗲𝗹𝗲𝘀𝘀 𝗳𝗿𝗲𝗲𝗱𝗼𝗺" },
	{ SUBTITLE::LARGE,
		"▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n"
		"██░███░██▄██░▄▄▀█░▄▄█░██░▄▄█░▄▄█░▄▄████░▄▄▄█░▄▄▀█░▄▄█░▄▄█░▄▀█▀▄▄▀█░▄▀▄░█\n"
		"██░█░█░██░▄█░▀▀▄█░▄▄█░██░▄▄█▄▄▀█▄▄▀████░▄▄██░▀▀▄█░▄▄█░▄▄█░█░█░██░█░█▄█░█\n"
		"██▄▀▄▀▄█▄▄▄█▄█▄▄█▄▄▄█▄▄█▄▄▄█▄▄▄█▄▄▄████░████▄█▄▄█▄▄▄█▄▄▄█▄▄███▄▄██▄███▄█\n"
		"▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n"
	}
};

size_t utf8len(std::string s);

static size_t longest_line(const std::string& s) {

	size_t w = 0;

	if ( !s.empty()) {

		std::stringstream ss(s);
		std::string line;

		while ( std::getline(ss, line, '\n')) {
			size_t l = utf8len(line);
			if ( w < l )
				w = l;
		}
	}

	return w;
}

LOGO parse_logo(const std::string& name) {

	std::string _name = common::to_upper(name);

	if ( auto it = std::find_if(logo_names.begin(), logo_names.end(), [_name](const std::pair<std::string, LOGO>& p) { return p.first == _name; });
		it != logo_names.end())
		return it -> second;

	throw std::runtime_error("invalid logo name " + name + ", try using argument -list to see available logo names");
}

SUBTITLE parse_subtitle(const std::string& name) {

	std::string _name = common::to_upper(name);

	if ( auto it = std::find_if(subtitle_names.begin(), subtitle_names.end(), [_name](const std::pair<std::string, SUBTITLE>& p) { return p.first == _name; });
		it != subtitle_names.end())
		return it -> second;

	throw std::runtime_error("invalid subtitle name " + name + ", try using argument -list to see available subtitle names");
}

const std::string banner::logo(const std::string& filename, const BANNER& props) {

	if ( props.logo == LOGO::NO_LOGO && props.subtitle == SUBTITLE::NO_SUBTITLE )
		return "";

	banner::logo_error = false;
	std::string s(logos[props.logo]);
	if ( props.subtitle != SUBTITLE::NO_SUBTITLE ) {

		std::string _s = subtitles[props.subtitle];
		size_t w = longest_line(s);
		size_t _w = utf8len(_s);

		if ( w > 0 && _w > 0 && _w < w && props.subtitle != SUBTITLE::LARGE ) {

			while ( _w < w ) {
				_s = " " + _s;
				_w++;
			}
		}

		s += _s;
	}

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

	return "\n" + s;
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
