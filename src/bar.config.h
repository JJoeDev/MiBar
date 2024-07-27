#ifndef MIBAR_BAR_CONFIG_H
#define MIBAR_BAR_CONFIG_H

#define TARGET_MONITOR "HDMI-0"

// Bar Width adds the custom width to the display width, 
// So if your display is 1920x1080 and want the width to be 1900 set the bar width to be -20
#define BAR_WIDTH -1020
#define BAR_HEIGHT 40

// Bar x and y also adds to the x and y Randr has retrieved
#define BAR_X 510
#define BAR_Y 10


// Text Module
#define TEXT_MODULE_ON 1
#define TEXT_MODULE_STR "Hello, World! This is still a demo build :D"

#define TEXT_MODULE_X 345
#define TEXT_MODULE_Y 30
#define TEXT_MODULE_CENTER_Y 1

#endif