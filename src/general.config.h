#ifndef MIBAR_GENERAL_CONFIG_H
#define MIBAR_GENERAL_CONFIG_H

// Colors to use
#define BACKGROUND 0x0A3632
#define FOREGROUND 0xC6EFEB
#define COLOR1 0x115852
#define COLOR2 0x237E76
#define COLOR3 0x3CA99F

// Monitor for MiBar to find
#define TARGET_MONITOR "HDMI-0"

// Bar configuration
#define BAR_WIDTH 0
#define BAR_HEIGHT 40
#define BAR_X 0
#define BAR_Y 0

// Text module
#define TEXT_MODULE_STR "Hello, World! This is still a demo build :D"
#define TEXT_MODULE_X 345
#define TEXT_MODULE_Y 0

// Underline configuration
#define ENABLE_UNDERLINE 1
#define UNDERLINE_HEIGHT 3
#define UNDERLINE_X_OFFSET -1
#define UNDERLINE_Y_OFFSET -10

// Currently only PADDING_TOP & PADDING_LEFT is in use
#define PADDING_TOP 10
#define PADDING_RIGHT 0
#define PADDING_BOTTOM 0
#define PADDING_LEFT 5

#endif