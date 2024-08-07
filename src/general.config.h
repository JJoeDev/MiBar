#ifndef MIBAR_GENERAL_CONFIG_H
#define MIBAR_GENERAL_CONFIG_H

// Colors to use
#define BACKGROUND 0x111111
#define FOREGROUND 0x999999
#define COLOR1 0x777777
#define COLOR2 0x555555
#define COLOR3 0x333333

// Monitor for MiBar to find
#define TARGET_MONITOR "HDMI-0"

// Target font to use (To find available fonts use the xlsfonts command in a terminal)
#define FONT "-bitstream-charter-medium-i-normal--0-0-100-100-p-0-iso8859-1"
//#define FONT "lucidasans-10"
#define FONT_FALLBACK "fixed"

// Bar configuration
#define BAR_WIDTH -100
#define BAR_HEIGHT 32
#define BAR_X 50
#define BAR_Y 3

// If Automatic Updates is enabled the bar will call all lua plugins every set amount of time + when an event is recieved
#define AUTOMATIC_UPDATES_ENABLE 0
// The amount of time between updates in seconds
#define UPDATE_TIME 30

// Underline configuration
#define ENABLE_UNDERLINE 1
#define UNDERLINE_HEIGHT 3
#define UNDERLINE_X_OFFSET 0
#define UNDERLINE_Y_OFFSET 0

// Currently padding has not been implemented to the bar as plugins define their own positions
//#define PADDING_TOP 10
//#define PADDING_RIGHT 0
//#define PADDING_BOTTOM 0
//#define PADDING_LEFT 5

#endif
