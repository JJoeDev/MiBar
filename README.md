# MiBar

> [!WARNING]
>
> MiBar is at a usable 

![Header_image](./github_assets/github-header-image.png)
(image generated with [github-profile-header-generator](https://github.com/leviarista/github-profile-header-generator?tab=readme-ov-file))

MiBar is a simple Linux X11 status bar with Lua support

## Description

MiBar is a simple Linux X11 status bar built with the C++ programming language. The application uses 2 external libraries, XCB to interact with the X server, and sol2 to run user made lua scripts.

MiBar is supposed to be customizable and easy to configure. This is the reason for using the Lua language for plugins as it makes it easy to develop for MiBar.

## Getting Started

Currently the only way to use MiBar is by building from source.

### Dependencies

* git
* cmake
* make
* All xcb libraries
* sol2

### Installing

MiBar can only be run by building from source until a future release.

But until then this is how you can use MiBar

```
$ git clone https://github.com/JJoeDev/MiBar.git
$ cd MiBar
$ cmake -DCMAKE_BUILD_TYPE=Release .
$ make
```

### Executing program

Once MiBar has bin built from source the executable can be found in the bin directory
```
$ ./bin/MiBar
```

## Images

![Image_1](./github_assets/DemoBar.png)

![Image_2](./github_assets/DemoBar2.png)

![Image_3](./github_assets/DemoBarWLua.png)

## Plugin Development

MiBar allowes for users to create their own bar by creating plugins that do what they want them to do. Lua is a simple and easy scripting language with tons of documentation online.

### Creating a Plugin

* **Director:** Plugins should be located in ```~/.config/MiBar/plugins```
* **Available Functions** MiBar currently only exposes one function to the plugins, Here is an example of how to use it
* * ```DrawString(text, alignment, x_position)```: This function draws a string to the status bar
* * - ```text```: This is the string that will get displayed on the status bar
* * - ```alignment```: This is an alignment option (``LEFT``, ``CENTER``, or ``RIGHT``)
* * - ```x_position```: This is an added position on top of the alignment option

**Time.lua**: Time.lua is a simple plugin that simply displays the current time on the center of the bar

```lua
local time = os.date("%a %d / %H:%M")

DrawString(time, Alignment.CENTER, 0)
```

For a visual example of this script take a look at the third [image](#images)

## License

This project is licensed under the GNU General Public License v3.0 License - see the LICENSE.md file for details
