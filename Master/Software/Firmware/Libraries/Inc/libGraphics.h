#include "libGLCDFont.h"
#include "stdlib.h"
#include "stdbool.h"
#include "driverSWSSD1306.h"

void libGraphicsInit(int16_t w, int16_t h); // Constructor
void libGraphicsDrawPixel(int16_t x, int16_t y, uint16_t color);
void libGraphicsDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void libGraphicsDrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void libGraphicsDrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void libGraphicsDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void libGraphicsFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void libGraphicsFillScreen(uint16_t color);
void libGraphicsInvertDisplay(bool i);
void libGraphicsDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void libGraphicsDrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void libGraphicsFillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void libGraphicsFillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void libGraphicsDrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void libGraphicsFillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void libGraphicsDrawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void libGraphicsFillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void libGraphicsDrawBitmap_0(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void libGraphicsDrawBitmap_1(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void libGraphicsDrawBitmap_2(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void libGraphicsDrawBitmap_3(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void libGraphicsDrawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void libGraphicsDrawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void libGraphicsSetCursor(int16_t x, int16_t y);
void libGraphicsSetTextColor_0(uint16_t c);
void libGraphicsSetTextColor_1(uint16_t c, uint16_t bg);
void libGraphicsSetTextSize(uint8_t s);
void libGraphicsSetTextWrap(bool w);
void libGraphicsSetRotation(uint8_t r);
void libGraphicsCp437(bool x);
void libGraphicsSetFont(const GFXfont *f);
void libGraphicsGetTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
//getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
bool libGraphicsWrite(uint8_t);
int16_t libGraphicsHeight(void);
int16_t libGraphicsWidth(void);
uint8_t libGraphicsGetRotation(void);
// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
int16_t libGraphicsGetCursorX(void);
int16_t libGraphicsGetCursorY(void);


/*
class Adafruit_GFX_Button {

 public:
  Adafruit_GFX_Button(void);
  void initButton(Adafruit_GFX *gfx, int16_t x, int16_t y,
   uint8_t w, uint8_t h, uint16_t outline, uint16_t fill,
   uint16_t textcolor, char *label, uint8_t textsize);
  void drawButton(boolean inverted = false);
  boolean contains(int16_t x, int16_t y);

  void press(boolean p);
  boolean isPressed();
  boolean justPressed();
  boolean justReleased();

 private:
  Adafruit_GFX *_gfx;
  int16_t _x, _y;
  uint16_t _w, _h;
  uint8_t _textsize;
  uint16_t _outlinecolor, _fillcolor, _textcolor;
  char _label[10];

  boolean currstate, laststate;
};

class GFXcanvas1 : public Adafruit_GFX {

 public:
  GFXcanvas1(uint16_t w, uint16_t h);
  ~GFXcanvas1(void);
  void     drawPixel(int16_t x, int16_t y, uint16_t color),
           fillScreen(uint16_t color);
  uint8_t *getBuffer(void);
 private:
  uint8_t *buffer;
};

class GFXcanvas16 : public Adafruit_GFX {
  GFXcanvas16(uint16_t w, uint16_t h);
  ~GFXcanvas16(void);
  void      drawPixel(int16_t x, int16_t y, uint16_t color),
            fillScreen(uint16_t color);
  uint16_t *getBuffer(void);
 private:
  uint16_t *buffer;
};

*/
