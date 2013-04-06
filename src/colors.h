#ifndef _COLORS_H_
#define _COLORS_H_

#include <stdint.h>

typedef struct
{
	uint8_t r, g, b;
} Color_RGB;

typedef struct
{
    uint8_t hue, sat, val;
} Color_HSV;

enum hsvmode {FIXED,FADE,STROBE,FLASH};

typedef struct
{
    Color_HSV hsv;
    enum hsvmode mode;
	uint8_t speed;
	uint8_t steps;
} LED_Status;

Color_RGB hsv_to_rgb(Color_HSV hsv);

#endif
