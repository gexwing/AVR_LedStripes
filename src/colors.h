#ifndef _COLORS_H_
#define _COLORS_H_

#include <stdint.h>

enum hsvmode {FIXED,FADE,STROBE,FLASH};

typedef struct
{
	uint8_t r, g, b; //8bit Values
} RGBValue;

typedef struct
{
	uint8_t hue,sat,val; //8bit Values
	enum hsvmode mode;
	uint8_t speed;
	uint8_t steps;
} HSVValue;

RGBValue hsv_to_rgb(HSVValue hsv);

// NEW
typedef struct
{
	uint8_t r, g, b; //8bit Values
} Color_RGB;

typedef struct
{
    uint8_t hue, sat, val; //8bit Values
} Color_HSV;

typedef struct
{
    struct Color_HSV color;
    enum hsvmode mode;
	uint8_t speed;
	uint8_t steps;
} LED_Status;

#endif
