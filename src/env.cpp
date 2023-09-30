#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <unistd.h>

#include "env.hpp"

const std::string banner::username(void) {
	return std::string(getlogin());
}

const bool banner::root_password_is_empty(void) {

	if ( banner::username() != "root" )
		return false;

	bool res = false;

        if ( std::filesystem::exists("/etc/shadow"))
                if ( std::ifstream ifs("/etc/shadow"); ifs && ifs.is_open() && !ifs.fail() && ifs.good() && !ifs.eof()) {

			std::string s;
			while ( std::getline(ifs, s))
				if ( s.size() >= 6 && s.rfind("root::", 0 ) == 0 )
					res = true;

			ifs.close();
		}

	return res;
}

const std::string banner::env(const std::string& var) {

	if ( !var.empty())
		if ( const char* env_p = std::getenv(var.c_str()))
			return std::string(env_p);
	return "";
}
