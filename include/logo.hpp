#pragma once
#include <string>

namespace banner {

	extern bool logo_error;

	const std::string logo(const std::string& filename = "", const bool& use_classic_logo = false);
	const int logo_width(const std::string& logo);
	const std::string separator(int width, const char& sep = '-');
}
