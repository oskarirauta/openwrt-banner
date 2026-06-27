[![License:MIT](https://img.shields.io/badge/License-MIT-blue?style=plastic)](LICENSE)
[![C++ CI build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

### openwrt-banner

Simple login banner / motd written in C++ - a replacement for the shell banner
used in a profile script. It draws an OpenWrt logo, the release information, and
warns when root has no password set.

The logo can be picked from several built-in styles or read from a file, and a
release name can be hardcoded at build time for branding. UTF-8 logo files are
supported.

## <sub>Usage</sub>

```
./banner [options]

  -l, --logo <name>      use a named logo
  -s, --subtitle <name>  use a named subtitle
  -c, --classic          use the classic logo
  -f, --failsafe         show the failsafe banner
      --file <filename>  read the logo from a file (UTF-8)
      --list             list the available logo and subtitle names
  -v, --version          show version
  -h, --help             this usage message
```

Built-in logos: `none, classic, classic2, modern, tiny, small, thin, simple,
round, graffiti, sport, paint, blocky` (default: `modern`).

Subtitles: `none, plain, extended, thintext, italy, italy_wide, bold, large`.

With none of `--logo`, `--subtitle` or `--file` given, the default banner is the
`modern` logo with the `italy` subtitle.

The release line is read from `/etc/openwrt_release`; it can also be hardcoded at
build time via the `RELEASE_NAME` define (see the Makefile).

## <sub>Dependencies</sub>

 - [common_cpp](https://github.com/oskarirauta/common_cpp.git)
 - [usage_cpp](https://github.com/oskarirauta/usage_cpp.git)

## <sub>Building</sub>

```
git submodule update --init --recursive
make
```

Requires Linux and a C++17 (or newer) compiler.
