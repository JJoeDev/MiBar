# MiBar

A lightweight and minimal linux X11 status bar

---

> [!WARNING]
>
> MiBar is under heavy development and should not be used yet.

## Table of content
[Intro](#intro)

[Current state](#current-state)

[Current and future features](#features--planned)

[How to build and run](#building-and-running)

- [Building](#building)

- [Configuring](#configuration)

[Dependencies](#dependencies)

[Images](#images)

[Getting involved](#getting-involved)

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

### Configuration
The current way of configuring MiBar is with the help of two header files, the ``bar.config.h`` includes everything related directly to the bar, such as positioning, scale and modules.

The ``general.config.h`` file is the fun file, this is the one the renderer has an interest in, this file includes color, and underline information (with more to come)

Configuring them is quite easy as long as the variable names are never changed.

[bar.config.h](./src/bar.config.h)
```C
#define TARGET_MONITOR "HDMI-0" // The name of the display the bar should position itself to

// The bar has the same width as your target monitor so to scale it down make the width a negative number
#define BAR_WIDTH -20
#define BAR_HEIGHT 40 // The height of the bar is not tied to the monitor

// Both X and Y are alse made to match your target monitors top left corner
#define BAR_X 10
#define BAR_Y 10

// Text Module
#define TEXT_MODULE_ON 1 // Wether to render the text or not
#define TEXT_MODULE_STR "Hello World from arch linux PC!" // String to render to the bar

#define TEXT_MODULE_X 25 // X axis placement on the bar
#define TEXT_MODULE_Y 30 // Custom Y axis placement if TEXT_MODULE_CENTER_Y is 0
#define TEXT_MODULE_CENTER_Y 1 // Auto centers the text to the bar
```
[general.config.h](./src/general.config.h)
```C
// The color format MiBar uses is 0xRRGGBB of hexadecimal (0 to F)
#define BACKGROUND 0x0A3632
#define FOREGROUND 0xC6EFEB
#define COLOR1 0x115852
#define COLOR2 0x237E76
#define COLOR3 0x3CA99F

#define ENABLE_UNDERLINE 1 // enable underlines with 1 disable with 0
#define UNDERLINE_HEIGHT 5 // The height of the bar in pixels
#define UNDERLINE_X_OFFSET 0 // The line is default alligned to the same start pixel as the module
#define UNDERLINE_Y_OFFSET 0 // The default Y position is at the bottom of the bar
```

## Dependencies
[xcb](https://xcb.freedesktop.org/) is used to interact with the X11 server

## Images
![DemoImage](./github_assets/DemoBar.png)

![DemoImage2](./github_assets/DemoBar2.png)

## Getting involved

Currently, the project is not actively seeking contributions. However, if you have any suggestions or find potential improvements, feel free to reach out. Well-explained pull requests with clear descriptions and code comments will be considered for future versions.