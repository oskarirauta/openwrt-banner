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

// the one place to bump on a release
#define BANNER_VERSION "1.3.2"

static const std::string root_pw_warning = " warning, root password is not set";
static const std::string banner_file_missing = " access error, banner file: ";

static std::string logo_file;

int main(int argc, char *argv[]) {

	usage_t usage = {
		.args = { argc, argv },
		.info = {
			.name = "banner",
			.version = BANNER_VERSION,
			.author = "Oskari Rauta",
			.copyright = "2026, Oskari Rauta",
			.description = "\nbanner is program used to display a logo of Openwrt system after user logs in\n"
		},
		.options = {
			{ "logo", { .key = "l", .word = "logo", .desc = "use a named logo", .flag = usage_t::REQUIRED, .name = "name" }},
			{ "subtitle", { .key = "s", .word = "subtitle", .desc = "use a named subtitle", .flag = usage_t::REQUIRED, .name = "name" }},
			{ "classic", { .key = "c", .word = "classic", .desc = "use classic logo" }},
			{ "failsafe", { .key = "f", .word = "failsafe", .desc = "use failsafe banner" }},
			{ "file", { .word = "file", .desc = "read logo from file", .flag = usage_t::REQUIRED, .name = "filename" }},
			{ "list", { .word = "list", .desc = "list logo and subtext names" }},
			{ "version", { .key = "v", .word = "version", .desc = "show version" }},
			{ "help", { .key = "h", .word = "help", .desc = "this usage message" }}
		}
	};

	if ( usage["help"] ) {

		std::cout << usage << "\n" << usage.help() << "\n" << std::endl;
		return 0;

	} else if ( usage["version"] ) {

		std::cout << usage.version() << std::endl;
		return 0;

	} else if ( usage["list"] ) {

		std::cout << usage.version() << "\n" << std::endl;

		std::cout << "list of logos:\n"
			<< "none, classic, classic2, modern, tiny, small, thin, simple, round, graffiti, sport, paint, blocky\n"
			<< "\nlist of subtitles:\n"
			<< "none, plain, extended, thintext, italy, italy_wide, bold, large\n"
			<< "\ndefault logo: modern"
			<< "\ndefault subtitle: none (unless no logo, subtitle or file defined, when default is italy)" << std::endl;

		return 0;
	}

	if ( !usage.validated ) {

		auto errors = usage.errors();

		std::cout << "command-line errors found:\n" << errors << std::endl;

		if ( auto it = std::find_if(errors.begin(), errors.end(),
			[](usage_t::error_t e) {
				return e.error != usage_t::error_type::DUPLICATE && e.error != usage_t::error_type::UNKNOWN_OPTION;
			}); it != errors.end()) {

			std::cout << "\naborting, fatal errors occurred while parsing command-line arguments." << std::endl;
			return 1;
		}
	}

	if ( usage["failsafe"] ) {

		std::cout <<
			banner::failsafe_msg() <<
			std::endl;

		return 0;
	}

	BANNER props;

	if ( !usage["logo"] && !usage["subtitle"] && !usage["file"] ) {

		props.logo = default_logo;
		props.subtitle = SUBTITLE::ITALY;
	}

	if ( usage["logo"] ) {

		try {
			props.logo = parse_logo(usage["logo"].value);
		} catch ( const std::runtime_error &e ) {
			std::cerr << "error: " << e.what() << std::endl;
			return 1;
		}
	}

	if ( usage["subtitle"] ) {

		try {
			props.subtitle = parse_subtitle(usage["subtitle"].value);
		} catch ( const std::runtime_error &e ) {
			std::cerr << "error: " << e.what() << std::endl;
			return 1;
		}
	}

	if ( usage["file"] )
		logo_file = usage["file"].value;

	std::string logo(banner::logo(logo_file, props));
	int logo_size = banner::logo_size(props.logo);
	int subtitle_size = banner::subtitle_size(props.subtitle);
	std::string os_release, os_commit;

	banner::release_info(os_release, os_commit);
	std::string release_text = " OpenWrt";

	if ( !os_release.empty())
		release_text += " " + os_release;

	if ( !os_commit.empty())
		release_text += ", git commit " + os_commit;

	bool nopasswd = banner::root_password_is_empty();
	int nopasswd_size = nopasswd ? root_pw_warning.size() : 0;

	std::string warning_msg(banner::logo_error ? ( banner_file_missing + logo_file ) : "");
	int warning_size = std::max(nopasswd_size, (int)warning_msg.size());
	bool nowarning = warning_msg.empty();

	int line_width = std::max(
				  std::max(logo_size, subtitle_size),
				  std::max((int)release_text.size(), warning_size)
			);

	std::string separator(banner::separator(line_width > 0 ? line_width : 0));

	if ( !separator.empty())
		separator = " " + separator;

	std::cout <<
			logo << "\n" <<
			separator << "\n" <<
			release_text << "\n" <<
			( nopasswd ? ( root_pw_warning + "\n" ) : "" ) <<
			( nowarning ? "" : ( warning_msg + "\n" )) <<
			separator << std::endl;

	return 0;
}
