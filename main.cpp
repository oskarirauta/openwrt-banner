#include <iostream>
#include <string>

#include <functional>
#include <vector>
#include <map>

#include "logo.hpp"
#include "env.hpp"
#include "release.hpp"
#include "failsafe.hpp"

#include "usage.hpp"

static const std::string root_pw_warning = " warning, root password is not set";
static const std::string banner_file_missing = " access error, banner file: ";

static std::string logo_file;

int main(int argc, char *argv[]) {

	usage_t usage = {
		.args = { argc, argv },
		.name = "banner",
		.version_title = "v",
		.version = "1.0.0",
		.author = "Oskari Rauta",
		.options = {
			{ "classic", { .key = "c", .word = "classic", .desc = "use classic logo" }},
			{ "failsafe", { .key = "f", .word = "failsafe", .desc = "use failsafe banner" }},
			{ "version", { .key = "v", .word = "version", .desc = "show version" }},
			{ "help", { .key = "h", .word = "help", .desc = "this usage message" }}
		}
	};

	if ( usage["help"] ) {

		std::cout << usage << "\n\nusage:\n" << usage.help() << "\n" << std::endl;
		return 0;
	} else if ( usage["version"] ) {

		std::cout << usage.version_info() << std::endl;
		return 0;
	} else if ( !usage.args.empty()) {

		try {
			usage.validate();
		} catch ( const std::runtime_error& e ) {
			std::cout << usage << "\n\nusage:\n" << usage.help() << "\n\n" <<
					"error: " << e.what() << std::endl;
			return 1;
		}
	}

	if ( usage["failsafe"] ) {

		std::cout <<
			banner::failsafe_msg() <<
			std::endl;

		return 0;
	}

	std::string logo(banner::logo(logo_file, usage["classic"]));
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
