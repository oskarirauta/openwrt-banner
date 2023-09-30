#include <iostream>
#include <string>

#include <functional>
#include <vector>
#include <map>

#include "logo.hpp"
#include "env.hpp"
#include "release.hpp"
#include "constants.hpp"

#include "cmdparser.hpp"

static const std::string root_pw_warning = " warning, root password is not set";
static const std::string banner_file_missing = " access error, banner file: ";

static std::string logo_file;

static void version_header(void) {

	std::cout << APP_NAME << " v" << APP_VERSION << "\n"
		"author: Oskari Rauta" << std::endl;
}

static void usage(const std::string &cmd) {
	std::cout << "\nusage: " << cmd << " [options] [banner/motd file]" << "\n" << std::endl;
	std::cout <<
			"  options:\n" <<
			"    --help, --h                usage\n" <<
			"    --classic, --c             use classic logo\n" <<
			"    --version, --v		show version\n" <<
			std::endl;
}

int main(int argc, char *argv[]) {

	bool use_classic_logo = false;

	if ( argc > 1 ) {

		CmdParser cmdparser(std::vector<std::string>(argv, argv + argc), {
			{{ "-h", "--h", "-help", "--help", "-usage", "--usage" }, [](const CmdParser::Arg &arg) {

				version_header();
				usage(arg.cmd);
				exit(0);

			}, false },
			{{ "-v", "--v", "-version", "--version" }, [](const CmdParser::Arg &arg) {

				version_header();
				exit(0);

			}, false },
			{{ "-c", "--c", "-classic", "--classic" }, [&use_classic_logo](const CmdParser::Arg &arg) { use_classic_logo = true; }, false },
			{{ "" }, [](const CmdParser::Arg &arg) { logo_file = arg.arg; }, false }
		});

		cmdparser.parse();
	}

	std::string logo(banner::logo(logo_file, use_classic_logo));
	int logo_width = banner::logo_width(logo);
	std::string os_release, os_commit;

	banner::release_info(os_release, os_commit);
	std::string release_text = " OpenWrt";

	if ( !os_release.empty())
		release_text += " " + os_release;

	if ( !os_commit.empty())
		release_text += ", git commit " + os_commit;

	int release_width = release_text.size();
	bool nopasswd = banner::root_password_is_empty();
	int nopasswd_width = nopasswd ? root_pw_warning.size() : 0;

	std::string warning_msg(banner::logo_error ? ( banner_file_missing + logo_file ) : "");
	int warning_width = warning_msg.size();

	int line_width = std::max(logo_width, std::max(release_width, std::max(nopasswd_width, warning_width)));;

	std::string separator(banner::separator(line_width > 0 ? line_width : 0));

	if ( !separator.empty())
		separator = " " + separator;

	std::cout <<
			logo << "\n" <<
			separator << "\n" <<
			release_text << "\n" <<
			( nopasswd ? ( root_pw_warning + "\n" ) : "" ) <<
			( warning_width == 0 ? "" : ( warning_msg + "\n" )) <<
			separator << std::endl;

	return 0;
}
