# MiBar

A lightweight and minimal linux X11 status bar

---

> [!WARNING]
>
> MiBar is under heavy development and should not be used yet.

## Intro
MiBar is a C++17 app designed to be a user-friendly and configurable status bar for the X11 window system on linux. The goal is to create a minimal and light weight status bar with easy customization and extendability so users can create their own bar components.

## Current state
MiBar is in the early stages of development so components like configuration are not 100% in place and will have to take place in a C header file for now, this has the consequence of requireing the application to be recompiled every time the configuration file is modifed. 

Custom components are also not implemented yet and neither is components like: time, volume, and so on.

## Features (+ planned)

*More features might be added in the future*

* (planned) **Modules:** MiBar would like for users to be able to create their own modules for the bar to allow for even more customizability
* (planned) **Configuration:** At some point in the future MiBar should move away from the C header file to  a toml or json approach for configuration
* (planned) **Fonts** Currently MiBar only supports X11 fonts (use xlsfonts to see all fonts) in the future we should switch to cairo

## Building and running
The current version includes a config file in the form of a C header file located in ``miBar/src/general.config.h``. Each time this config file is modified a recompilation is required.

### Building
```
$ cd /Path/To/MiBar/CMakeLists.txt
$ cmake . -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
$ ./bin/MiBar
```

## Dependencies
[xcb](https://xcb.freedesktop.org/) is used to interact with the X11 server

## Images
![DemoImage](./github_assets/DemoBar.png)
Configuration for [this](./src/general.config.h) bar

## Getting involved

Currently, the project is not actively seeking contributions. However, if you have any suggestions or find potential improvements, feel free to reach out. Well-explained pull requests with clear descriptions and code comments will be considered for future versions.