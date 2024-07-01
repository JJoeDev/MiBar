# MiBar

MiBar is a simple Linux X11 status bar

---

> [!WARNING]
>
> This project is no where near being a viabal daily driver for a bar and a ton of work is still required to get it to work correctly.

## Configuration

**This has not been completed yet and is still a work in progress**

First time running MiBar the application will look for the config directory at ```$HOME/.config/MiBar```. If this directory does not exist it will be created for you.

Inside this directory you can create a ```config.mi``` file. This is where all the bar configuration will take place.

The config file is space insensitive but requires a key = value syntax

### Example
```
bg_color = FF0000
fg_color = 00FFFF

bar_width  = 1900
bar_height = 40

bar_x = 10
```

| Config Key | Values | Description |
| - | - | - |
| bg_color | RGB HEX | The value is hexadecimal RR-GG-BB so FF0000 would be red |
| fg_color | RGB HEX | The same values as above but for the text color |
| bar_width | unsigned number | The width is a whole number that cannot be negative |
| bar_height | unsigned number | Same values as bar_width |
| bar_x | unsigned number | Same value as above, Controlls where the bar is placed (WILL CHANGE AT  SOME POINT TO USE XRANDR)

## Features

| Feature | Status | Message |
| - | - | - |
| Rendering | Working | Can render the bar and precompiled content |
| Configuration | Working | Can read config file and apply configs |
