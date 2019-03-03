#include "libGraphics.h"
	
int16_t WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
int16_t _width, _height, // Display w/h as modified by current rotation
				cursor_x, cursor_y;
uint16_t textcolor, textbgcolor;
uint8_t textsize, rotation;
bool wrap,   // If set, 'wrap' text at right edge of display
    _cp437; // If set, use correct CP437 charset (default is off)
GFXfont *gfxFont;

/*
Copyright (c) 2013 Adafruit Industries.  All rights reserved.
*/

#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

// Pointers are a peculiar case...typically 16-bit on AVR boards,
// 32 bits elsewhere.  Try to accommodate both...

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

void libGraphicsInit(int16_t w, int16_t h){
  _width    = WIDTH = w;
  _height   = HEIGHT = h;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap      = true;
  _cp437    = false;
  gfxFont   = NULL;
}; // Constructor
	
void libGraphicsDrawPixel(int16_t x, int16_t y, uint16_t color){
	driverSWSSD1306DrawPixel(x,y,color);
};
	
void libGraphicsDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      libGraphicsDrawPixel(y0, x0, color);
    } else {
      libGraphicsDrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
};
	
void libGraphicsDrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
  libGraphicsDrawLine(x, y, x, y+h-1, color);
};
	
void libGraphicsDrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
  libGraphicsDrawLine(x, y, x+w-1, y, color);
};
	
void libGraphicsDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  libGraphicsDrawFastHLine(x, y, w, color);
  libGraphicsDrawFastHLine(x, y+h-1, w, color);
  libGraphicsDrawFastVLine(x, y, h, color);
  libGraphicsDrawFastVLine(x+w-1, y, h, color);
};
	
void libGraphicsFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  for (int16_t i=x; i<x+w; i++) {
    libGraphicsDrawFastVLine(i, y, h, color);
  }
};
	
void libGraphicsFillScreen(uint16_t color){
	libGraphicsFillRect(0, 0, _width, _height, color);
};
	
void libGraphicsInvertDisplay(bool i){
	
};
	
void libGraphicsDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  libGraphicsDrawPixel(x0  , y0+r, color);
  libGraphicsDrawPixel(x0  , y0-r, color);
  libGraphicsDrawPixel(x0+r, y0  , color);
  libGraphicsDrawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    libGraphicsDrawPixel(x0 + x, y0 + y, color);
    libGraphicsDrawPixel(x0 - x, y0 + y, color);
    libGraphicsDrawPixel(x0 + x, y0 - y, color);
    libGraphicsDrawPixel(x0 - x, y0 - y, color);
    libGraphicsDrawPixel(x0 + y, y0 + x, color);
    libGraphicsDrawPixel(x0 - y, y0 + x, color);
    libGraphicsDrawPixel(x0 + y, y0 - x, color);
    libGraphicsDrawPixel(x0 - y, y0 - x, color);
	};
};
	
void libGraphicsDrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color){
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      libGraphicsDrawPixel(x0 + x, y0 + y, color);
      libGraphicsDrawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      libGraphicsDrawPixel(x0 + x, y0 - y, color);
      libGraphicsDrawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      libGraphicsDrawPixel(x0 - y, y0 + x, color);
      libGraphicsDrawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      libGraphicsDrawPixel(x0 - y, y0 - x, color);
      libGraphicsDrawPixel(x0 - x, y0 - y, color);
    }
  }
};
	
void libGraphicsFillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
  libGraphicsDrawFastVLine(x0, y0-r, 2*r+1, color);
  libGraphicsFillCircleHelper(x0, y0, r, 3, 0, color);
};
	
void libGraphicsFillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color){
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      libGraphicsDrawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      libGraphicsDrawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      libGraphicsDrawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      libGraphicsDrawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
};
	
void libGraphicsDrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
  libGraphicsDrawLine(x0, y0, x1, y1, color);
  libGraphicsDrawLine(x1, y1, x2, y2, color);
  libGraphicsDrawLine(x2, y2, x0, y0, color);
};
	
void libGraphicsFillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    libGraphicsDrawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) _swap_int16_t(a,b);
    libGraphicsDrawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) _swap_int16_t(a,b);
    libGraphicsDrawFastHLine(a, y, b-a+1, color);
  }
};
	
void libGraphicsDrawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color){
  // smarter version
  libGraphicsDrawFastHLine(x0+radius  , y0    , w-2*radius, color); // Top
  libGraphicsDrawFastHLine(x0+radius  , y0+h-1, w-2*radius, color); // Bottom
  libGraphicsDrawFastVLine(x0    , y0+radius  , h-2*radius, color); // Left
  libGraphicsDrawFastVLine(x0+w-1, y0+radius  , h-2*radius, color); // Right
  // draw four corners
  libGraphicsDrawCircleHelper(x0+radius    , y0+radius    , radius, 1, color);
  libGraphicsDrawCircleHelper(x0+w-radius-1, y0+radius    , radius, 2, color);
  libGraphicsDrawCircleHelper(x0+w-radius-1, y0+h-radius-1, radius, 4, color);
  libGraphicsDrawCircleHelper(x0+radius   , y0+h-radius-1, radius, 8, color);
};
	
void libGraphicsFillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color){
  // smarter version
  libGraphicsFillRect(x0+radius, y0, w-2*radius, h, color);

  // draw four corners
  libGraphicsFillCircleHelper(x0+w-radius-1, y0+radius, radius, 1, h-2*radius-1, color);
  libGraphicsFillCircleHelper(x0+radius    , y0+radius, radius, 2, h-2*radius-1, color);
};
	
void libGraphicsDrawBitmap_0(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){
  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++) {
      if(i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x80) libGraphicsDrawPixel(x+i, y+j, color);
    }
  }
};
	
void libGraphicsDrawBitmap_1(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg){
  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x80) libGraphicsDrawPixel(x+i, y+j, color);
      else            libGraphicsDrawPixel(x+i, y+j, bg);
    }
  }
};
	
void libGraphicsDrawBitmap_2(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){
  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte <<= 1;
      else      byte   = bitmap[j * byteWidth + i / 8];
      if(byte & 0x80) libGraphicsDrawPixel(x+i, y+j, color);
    }
  }
};
	
void libGraphicsDrawBitmap_3(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg){
  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte <<= 1;
      else      byte   = bitmap[j * byteWidth + i / 8];
      if(byte & 0x80) libGraphicsDrawPixel(x+i, y+j, color);
      else            libGraphicsDrawPixel(x+i, y+j, bg);
    }
  }
};
	
void libGraphicsDrawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){
  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(i & 7) byte >>= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x01) libGraphicsDrawPixel(x+i, y+j, color);
    }
  }
};
	
void libGraphicsDrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size){
  if(!gfxFont) { // 'Classic' built-in font

    if((x >= _width)            || // Clip right
       (y >= _height)           || // Clip bottom
       ((x + 6 * size - 1) < 0) || // Clip left
       ((y + 8 * size - 1) < 0))   // Clip top
      return;

    if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

    for(int8_t i=0; i<6; i++ ) {
      uint8_t line;
      if(i < 5) line = pgm_read_byte(font+(c*5)+i);
      else      line = 0x0;
      for(int8_t j=0; j<8; j++, line >>= 1) {
        if(line & 0x1) {
          if(size == 1) libGraphicsDrawPixel(x+i, y+j, color);
          else          libGraphicsFillRect(x+(i*size), y+(j*size), size, size, color);
        } else if(bg != color) {
          if(size == 1) libGraphicsDrawPixel(x+i, y+j, bg);
          else          libGraphicsFillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
    }

  } else { // Custom font

    // Character is assumed previously filtered by write() to eliminate
    // newlines, returns, non-printable characters, etc.  Calling drawChar()
    // directly with 'bad' characters of font may cause mayhem!

    c -= pgm_read_byte(&gfxFont->first);
    GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
    uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
    uint8_t  w  = pgm_read_byte(&glyph->width),
             h  = pgm_read_byte(&glyph->height);
//             xa = pgm_read_byte(&glyph->xAdvance);
    int8_t   xo = pgm_read_byte(&glyph->xOffset),
             yo = pgm_read_byte(&glyph->yOffset);
    uint8_t  xx, yy, bits, bit = 0;
    int16_t  xo16, yo16;

    if(size > 1) {
      xo16 = xo;
      yo16 = yo;
    }

    // Todo: Add character clipping here

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.

    for(yy=0; yy<h; yy++) {
      for(xx=0; xx<w; xx++) {
        if(!(bit++ & 7)) {
          bits = pgm_read_byte(&bitmap[bo++]);
        }
        if(bits & 0x80) {
          if(size == 1) {
            libGraphicsDrawPixel(x+xo+xx, y+yo+yy, color);
          } else {
            libGraphicsFillRect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
          }
        }
        bits <<= 1;
      }
    }

  } // End classic vs custom font
};
	
void libGraphicsSetCursor(int16_t x, int16_t y){
  cursor_x = x;
  cursor_y = y;
};
	
void libGraphicsSetTextColor_0(uint16_t c){
	textcolor = textbgcolor = c;
};
	
void libGraphicsSetTextColor_1(uint16_t c, uint16_t bg){
  textcolor   = c;
  textbgcolor = bg;
};
	
void libGraphicsSetTextSize(uint8_t s){
	textsize = (s > 0) ? s : 1;
};
	
void libGraphicsSetTextWrap(bool w){
	wrap = w;
};
	
void libGraphicsSetRotation(uint8_t r){
  rotation = (r & 3);
  switch(rotation) {
   case 0:
   case 2:
    _width  = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
};
	
void libGraphicsCp437(bool x){
	_cp437 = x;
};
	
void libGraphicsSetFont(const GFXfont *f){
  if(f) {          // Font struct pointer passed in?
    if(!gfxFont) { // And no current font struct?
      // Switching from classic to new font behavior.
      // Move cursor pos down 6 pixels so it's on baseline.
      cursor_y += 6;
    }
  } else if(gfxFont) { // NULL passed.  Current font struct defined?
    // Switching from new to classic font behavior.
    // Move cursor pos up 6 pixels so it's at top-left of char.
    cursor_y -= 6;
  }
  gfxFont = (GFXfont *)f;
};
	
void libGraphicsGetTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h){
  uint8_t c; // Current character

  *x1 = x;
  *y1 = y;
  *w  = *h = 0;

  if(gfxFont) {

    GFXglyph *glyph;
    uint8_t   first = pgm_read_byte(&gfxFont->first),
              last  = pgm_read_byte(&gfxFont->last),
              gw, gh, xa;
    int8_t    xo, yo;
    int16_t   minx = _width, miny = _height, maxx = -1, maxy = -1,
              gx1, gy1, gx2, gy2, ts = (int16_t)textsize,
              ya = ts * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);

    while((*string)) { // BUG? 
			c = *string++;
      if(c != '\n') { // Not a newline
        if(c != '\r') { // Not a carriage return, is normal char
          if((c >= first) && (c <= last)) { // Char present in current font
            c    -= first;
            glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
            gw    = pgm_read_byte(&glyph->width);
            gh    = pgm_read_byte(&glyph->height);
            xa    = pgm_read_byte(&glyph->xAdvance);
            xo    = pgm_read_byte(&glyph->xOffset);
            yo    = pgm_read_byte(&glyph->yOffset);
            if(wrap && ((x + (((int16_t)xo + gw) * ts)) >= _width)) {
              // Line wrap
              x  = 0;  // Reset x to 0
              y += ya; // Advance y by 1 line
            }
            gx1 = x   + xo * ts;
            gy1 = y   + yo * ts;
            gx2 = gx1 + gw * ts - 1;
            gy2 = gy1 + gh * ts - 1;
            if(gx1 < minx) minx = gx1;
            if(gy1 < miny) miny = gy1;
            if(gx2 > maxx) maxx = gx2;
            if(gy2 > maxy) maxy = gy2;
            x += xa * ts;
          }
        } // Carriage return = do nothing
      } else { // Newline
        x  = 0;  // Reset x
        y += ya; // Advance y by 1 line
      }
    }
    // End of string
    *x1 = minx;
    *y1 = miny;
    if(maxx >= minx) *w  = maxx - minx + 1;
    if(maxy >= miny) *h  = maxy - miny + 1;

  } else { // Default font

    uint16_t lineWidth = 0, maxWidth = 0; // Width of current, all lines

    while((*string)) {	// BUG? 
			c = *string++;
      if(c != '\n') { // Not a newline
        if(c != '\r') { // Not a carriage return, is normal char
          if(wrap && ((x + textsize * 6) >= _width)) {
            x  = 0;            // Reset x to 0
            y += textsize * 8; // Advance y by 1 line
            if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
            lineWidth  = textsize * 6; // First char on new line
          } else { // No line wrap, just keep incrementing X
            lineWidth += textsize * 6; // Includes interchar x gap
          }
        } // Carriage return = do nothing
      } else { // Newline
        x  = 0;            // Reset x to 0
        y += textsize * 8; // Advance y by 1 line
        if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
        lineWidth = 0;     // Reset lineWidth for new line
      }
    }
    // End of string
    if(lineWidth) y += textsize * 8; // Add height of last (or only) line
    *w = maxWidth - 1;               // Don't include last interchar x gap
    *h = y - *y1;

  } // End classic vs custom font
};
	
//getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
bool libGraphicsWrite(uint8_t c){
  if(!gfxFont) { // 'Classic' built-in font

    if(c == '\n') {
      cursor_y += textsize*8;
      cursor_x  = 0;
    } else if(c == '\r') {
      // skip em
    } else {
      if(wrap && ((cursor_x + textsize * 6) >= _width)) { // Heading off edge?
        cursor_x  = 0;            // Reset x to zero
        cursor_y += textsize * 8; // Advance y one line
      }
      libGraphicsDrawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
      cursor_x += textsize * 6;
    }

  } else { // Custom font

    if(c == '\n') {
      cursor_x  = 0;
      cursor_y += (int16_t)textsize *
                  (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    } else if(c != '\r') {
      uint8_t first = pgm_read_byte(&gfxFont->first);
      if((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
        uint8_t   c2    = c - pgm_read_byte(&gfxFont->first);
        GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c2]);
        uint8_t   w     = pgm_read_byte(&glyph->width),
                  h     = pgm_read_byte(&glyph->height);
        if((w > 0) && (h > 0)) { // Is there an associated bitmap?
          int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
          if(wrap && ((cursor_x + textsize * (xo + w)) >= _width)) {
            // Drawing character would go off right edge; wrap to new line
            cursor_x  = 0;
            cursor_y += (int16_t)textsize *
                        (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
          }
          libGraphicsDrawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
        }
        cursor_x += pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize;
      }
    }

  }
	
	return true;
};
	
int16_t libGraphicsHeight(void){
	return _height;
};
	
int16_t libGraphicsWidth(void){
	return _width;
};
	
uint8_t libGraphicsGetRotation(void){
	return rotation;
};
	
// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
int16_t libGraphicsGetCursorX(void){
	return cursor_x;
};
	
int16_t libGraphicsGetCursorY(void){
	return cursor_y;
};
