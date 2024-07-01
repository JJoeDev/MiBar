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
bg_color = FF0000 # Sets the background color of the bar
fg_color = 00FFFF # Sets the foreground color of rendered text

bar_width  = 1900 # Sets the width of the bar
bar_height = 40 # Sets the height of the bar

bar_x = 10 # Sets the bars x position
bar_y = 10 # Sets the bars y position

target_monitor = HDMI-0 # uses xrandr to find the specified monitor and adds the correct x and y offset to the bar_x and bar_y
```

| Config Key | Values | Description |
| - | - | - |
| bg_color | RGB HEX | The value is hexadecimal RR-GG-BB so FF0000 would be red |
| fg_color | RGB HEX | The same values as above but for the text color |
| bar_width | unsigned number | The width is a whole number that cannot be negative |
| bar_height | unsigned number | Same values as bar_width |
| bar_x | unsigned number | Adds the value in pixels to the bar x pos |
| bar_y | unsigned number | Adds the value in pixels to the bar y pos |
| target_monitor | monitor name (HDMI-0) | Uses xrandr to find specified monitor and applies the correct x and y position to the bar to get it to the monitor

## Features

| Feature | Status | Message |
| - | - | - |
| Rendering | Working | Can render the bar and precompiled content |
| Configuration | Working | Can read config file and apply configs |
