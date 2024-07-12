# MiBar

A lightweight and minimal linux X11 status bar

---

> [!WARNING]
>
> MiBar is under heavy development and should not be used yet.

## Intro
MiBar is a C++17 app designed to be a user-friendly and configurable status bar for the X11 window system on linux. The goal is to create a minimal and light weight status bar with easy customization and extendability so users can create their own bar components.

## Current state
MiBar is in its early stages of development. While core functionalities like configuration parsing are in place, the project is yet to use any of the parsed data. The renderer is also under development and only supports rendering a hard coded string at the moment.

## Features (+ planned)

*More features might be added in the future*

* (planned) **Modules:** MiBar would like for users to be able to create their own modules for the bar to allow for even more customizability
* (W.I.P) **Configuration:** I am working on getting the bar ready to use the parsed config.toml file that is created at ```$HOME/.config/MiBar/config.toml```

## Dependencies
[xcb](https://xcb.freedesktop.org/) is used to interact with the X11 server

[toml++](https://marzer.github.io/tomlplusplus/) is used for configuration parsing

## Getting involved

Currently, the project is not actively seeking contributions. However, if you have any suggestions or find potential improvements, feel free to reach out. Well-explained pull requests with clear descriptions and code comments will be considered for future versions.