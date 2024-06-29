#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>
#include <stdint.h>

#define PI 3.14159265f
#define TWO_PI 6.28318530f

#define TILE_SIZE 64

#define NUM_TEXTURES 14
#define MINIMAP_SCALE_FACTOR 0.1f

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400
#define PIXEL_SIZE 2

#define MAP_NUM_ROWS_Y 14
#define MAP_NUM_COLS_X 20

#define TEXTURE_WIDTH 64
#define TEXUTRE_HEIGHT 64

#define NUM_SPRITES 1
#define NUM_CHARTEXTURES 7
#define FOV_ANGLE (60.0f * (PI / 180.0f))

#define NUM_RAYS WINDOW_WIDTH
//640               .5775
#define Dist_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2)) //1108.51

#define FPS 60
#define FRAME_TIME_LENGTH (1000 / FPS)



typedef uint32_t color_t;

#endif

