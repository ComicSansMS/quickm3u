# QuickM3U

**Build status:** [![Build status](https://ci.appveyor.com/api/projects/status/github/ComicSansMS/quickm3u?svg=true)](https://ci.appveyor.com/project/ComicSansMS/quickm3u)

A minimalistic editor for M3U multimedia playlists.

![QuickM3U Logo](https://github.com/ComicSansMS/quickm3u/raw/master/resources/logo_128.png "QuickM3U")

## Downloads

### Latest Builds (unstable)

Note that these might be down while a build is running or if the current master is broken.

 * [Windows](https://ci.appveyor.com/project/ComicSansMS/quickm3u/build/artifacts)

## Open Issues

 * Support for Extended M3U files - Currently all extended tags are lost upon saving
 * UI improvements
   * Path conversion for individual entries
   * Copying of individual elements to directories
   * Copy/Pasting & Drag'n'Drop between instances
 * Cross-platform support for Linux and OSX (this is mainly blocked by missing C++20 standard library features)

## Build Instructions

### Build Requirements

 * CMake Version 3.14 or later
 * Visual Studio 2019
 * Qt 5.13 (earlier versions of Qt 5 will probably work as well, but the build is currently only tested with 5.13)

### How to build

1. Invoke CMake from the build directory to generate the build files
   ```
   cd build
   cmake -DCMAKE_INSTALL_PREFIX=<desired install directory> ..
   ```
1. Use CMake to build
   ```
   cmake --build . --config RelWithDebInfo
   ```
1. Use CMake to install
   ```
   cmake --build . --target install
   ```

## License

This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details.
