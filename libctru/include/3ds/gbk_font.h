#pragma once

#include <3ds/types.h>

typedef struct gbk_font_header{
    u8	magic[4];	//'U'('S', 'M'), 'F', 'L', X---Unicode(Simple or MBCS) Font Library, X: version number,Ver 1.2
    u8	section; // sections。
    u8	height;    /* height of glypha cell  */
    u16	cpFlag;    // codepageflag:  bit0~bit13 meaning defferent codepage
    u16	totalChars;  // total chars
    u8	scanMode;   // scan mode
} GBK_FONT_HEADER, *PGBK_FONT_HEADER;

typedef struct gbk_font_section{
    u16	 	first;	// first char unicode
    u16		last;	// last char unicode
    u16		startIndex;	// first char offset in section（index）
}GBK_FONT_SECTION;


typedef struct gbk_font_index{
    u8 		glyphWidth;	// width of a glyph
    u32		offset;	 // offset in font data
}GBK_FONT_INDEX;

typedef struct font_cell {
    u16   unicode;
    u8    width;
    u8    height;
    u8*   glyphData;
}FONT_CELL;

int gbk_get_font_cell(u16 unicode, FONT_CELL* cell);
