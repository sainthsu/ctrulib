#pragma once

#include <3ds/types.h>

#define CELL_HEIGHT 16

typedef struct font_index {
	u16 unicode;
	u8 width;
	u8 data[32];
}FontIndex;

typedef struct font_cell {
    u16   unicode;
    u8    width;
    u8    height;
    u8*   glyphData;
}FontCell;

int unifont_get_cell(u16 unicode, FontCell* cell);
int unifont_get_size(int *width, int* height, u16 unicode);
