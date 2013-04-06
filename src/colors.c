#include "colors.h"

RGBValue hsv_to_rgb(const HSVValue hsv){
    RGBValue rgb;

    if(hsv.sat == 0)
    {
        rgb.r = hsv.val;
        rgb.g = hsv.val;
        rgb.b = hsv.val;
    }
    else
    {
        unsigned char f = (((unsigned short)hsv.hue) * 1536) / 256; //simplified by moving multiplication into the modulus. hsv.hue % (60*256/360) * (256 / 60);
        unsigned char p = ((unsigned short)hsv.val * (unsigned short)(255 - hsv.sat)) / 256;
        unsigned char q = ((unsigned short)hsv.val * (unsigned short)(255 - (unsigned short)(hsv.sat) * (unsigned short)(f) / 256)) / 256;
        unsigned char t = ((unsigned short)hsv.val * (unsigned short)(255 - (unsigned short)(hsv.sat) * (unsigned short)(255 - f) / 256)) / 256;

        switch((((unsigned short)hsv.hue) * 192) / 8192)
        {
        case 1:
            rgb.r = q;
            rgb.g = hsv.val;
            rgb.b = p;
            break;

        case 2:
            rgb.r = p;
            rgb.g = hsv.val;
            rgb.b = t;
            break;

        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = hsv.val;
            break;

        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = hsv.val;
            break;

        case 5:
            rgb.r = hsv.val;
            rgb.g = p;
            rgb.b = q;
            break;

        default:
            rgb.r = hsv.val;
            rgb.g = t;
            rgb.b = p;
            break;
        }
    }
    return rgb;
}
