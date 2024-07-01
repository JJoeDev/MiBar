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

The config file syntax consists of Type:Key = Value and the following example will create a red bar with blue foreground (text) with a custom string to render

### Example
```
bar:target_monitor = HDMI-0 # uses xrandr to find the specified monitor and adds the correct x and y offset to the bar_x and bar_y

bar:bg_color = FFFF0000 # Sets the bar color to red following the AA-RR-GG-BB syntax
bar:fg_color = FF0000FF # Sets the foreground / text to blue using AA-RR-GG-BB

bar:width  = 1900 # Sets the width of the bar
bar:height = 40 # Sets the height of the bar

bar:x = 10 # Sets the bars x position
bar:y = 10 # Sets the bars y position

text:create = Hello, Linux From MiBar
```

| Type | Config Key | Values | Description |
| - | - | - |
| bar | bg_color | RGB HEX | The value is hexadecimal RR-GG-BB so FF0000 would be red |
| bar | fg_color | RGB HEX | The same values as above but for the text color |
| bar | bar_width | unsigned number | The width is a whole number that cannot be negative |
| bar | bar_height | unsigned number | Same values as bar_width |
| bar | bar_x | unsigned number | Adds the value in pixels to the bar x pos |
| bar | bar_y | unsigned number | Adds the value in pixels to the bar y pos |
| bar | target_monitor | monitor name (HDMI-0) | Uses xrandr to find specified monitor and applies the correct x and y position to the bar to get it to the monitor
| text | create | string | Renders the desired string on the bar |

## Features

| Feature | Status | Message |
| - | - | - |
| Rendering | Working | custom color and position |
| Configuration | Working | config file from .config/MiBar/config.mi |
| Transform | Working | user defined width height x and y |
| Modules | W.I.P | demo text module for custom text rendering |
