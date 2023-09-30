#include <fstream>
#include <filesystem>

#include "failsafe.hpp"

static std::string default_failsafe_msg(void) {

        return
		"================= FAILSAFE MODE active ================\n"
		"special commands:\n"
		"* firstboot	     reset settings to factory defaults\n"
		"* mount_root	 mount root-partition with config files\n"
		"\n"
		"after mount_root:\n"
		"* passwd			 change root's password\n"
		"* /etc/config		    directory with config files\n"
		"\n"
		"for more help see:\n"
		"https://openwrt.org/docs/guide-user/troubleshooting/\n"
		"- failsafe_and_factory_reset\n"
		"- root_password_reset\n"
		"=======================================================\n";
}


std::string banner::failsafe_msg(void) {

	std::string msg;

	if ( std::filesystem::exists("/etc/banner.failsafe"))
		if ( std::ifstream ifs("/etc/banner.failsafe"); ifs && ifs.is_open() && !ifs.fail() && ifs.good() && !ifs.eof()) {

			std::string s;
			while ( std::getline(ifs, s))
				msg += ( msg.empty() ? "" : "\n" ) + s;

			ifs.close();
		}

	return msg.empty() ? default_failsafe_msg() : msg;
}
