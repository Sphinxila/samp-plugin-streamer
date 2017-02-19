SA-MP Streamer Plugin with CMake Support
=====================

This plugin streams objects, pickups, checkpoints, race checkpoints, map icons, 3D text labels and actors at user-defined server ticks. Basic area detection is also included. Because it is written entirely in C++, much of the overhead from PAWN is avoided. This streamer, as a result, is quite a bit faster than any other implementation currently available in PAWN.

This project is a fork of 
- [samp-incognito/samp-streamer-plugin](https://github.com/samp-incognito/samp-streamer-plugin)

Documentation
-------------

Documentation can  be found on the [wiki](https://github.com/samp-incognito/samp-streamer-plugin/wiki).

Download
--------

The latest binaries for Windows and Linux can be found on the [releases page](https://github.com/samp-incognito/samp-streamer-plugin/releases).

## Build
#### Requirements
- [Modified version of gclient](https://github.com/timniederhausen/gclient)
- CMake >=3.1
- C++11
  - debian package: libc6-dev-i386

Synchronize dependencies from [DEPS](DEPS) File.

    gclient.py sync -v -f

You can also download it manually and place it into the external directory.

    mkdir build
    cd build
    cmake ..\samp-streamer-plugin
    make

- [How to use cmake](https://github.com/bast/cmake-example)