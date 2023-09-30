#include <fstream>
#include <filesystem>

#include "release.hpp"

#define xstr(s) str(s)
#define str(s) #s

void trim_quotes(std::string &s) {

	if ( !s.empty() && ( s.front() == '\'' || s.front() == '"' ))
		s.erase(0, 1);
	if ( !s.empty() && ( s.back() == '\'' || s.back() == '"' ))
		s.pop_back();
}

const bool parse_value(const std::string& line, const std::string& var, std::string& res) {

	std::string s(line);

	if ( s.size() >= ( var.size() + 1 ) &&
		s.rfind(var + "=", 0 ) == 0 ) {

		s.erase(0, var.size() + 1);
		trim_quotes(s);

		res = s;
		return true;
	}

	return false;
}

void banner::release_info(std::string& os_release, std::string& os_commit) {

	std::string release(xstr(RELEASE_NAME));

	trim_quotes(release);

	if ( !release.empty())
		os_release = release;

	if ( std::filesystem::exists("/etc/openwrt_release"))
		if ( std::ifstream ifs("/etc/openwrt_release"); ifs && ifs.is_open() && !ifs.fail() && ifs.good() && !ifs.eof()) {

			os_commit = "hello";

			std::string s;
			while ( std::getline(ifs, s)) {

				if ( release.empty())
					parse_value(s, "DISTRIB_RELEASE", os_release);
				parse_value(s, "DISTRIB_REVISION", os_commit);
			}

			ifs.close();
		}

	if ( release.empty() && os_release.empty())
		os_release = "unknown_release_version";

}
