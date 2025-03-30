#pragma once
#include <string>

enum LOGO {
	NO_LOGO, CLASSIC, CLASSIC2, TINY, SMALL, THIN, SIMPLE, ROUND, GRAFFITI, SPORT, PAINT, BLOCKY
};

enum SUBTITLE {
	NO_SUBTITLE, PLAIN, EXTENDED, THINTEXT, ITALY, BOLD, LARGE
};

static const LOGO classic_logo = LOGO::CLASSIC;
static const LOGO default_logo = LOGO::BLOCKY;
static const SUBTITLE default_subtitle = SUBTITLE::NO_SUBTITLE;

struct BANNER {

	public:

		LOGO logo = default_logo;
		SUBTITLE subtitle = default_subtitle;
};

LOGO parse_logo(const std::string& name);
SUBTITLE parse_subtitle(const std::string& name);

namespace banner {

	extern bool logo_error;

	const std::string logo(const std::string& filename = "", const BANNER& props = BANNER());
	const int logo_width(const std::string& logo);
	const std::string separator(int width, const char& sep = '-');
}
