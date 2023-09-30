# openwrt-banner

simple login banner/motd written in c++

replacement for script version of banner used in profile.
Does not bring much advantage. I just wanted my profile file
to be cleaner without empty root password detection so, I
wrote a small program that does the same. And re-designed the
logo.

This allows you to brand by hardcoding release version if
needed. And allows also to use logo from file (not in initial version).

utf-8 logo files are supported.

### Usage

```
./banner [options] [alternative logo text file]

Command-line options:

 --help     usage
 --classic  use classic logo
 --version  version information
```

### License

MIT
