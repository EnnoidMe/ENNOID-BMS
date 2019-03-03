#ifndef _DRIVERSWSSD1306_H
#define _DRIVERSWSSD1306_H

#include "driverHWI2C1.h"
#include "libGraphics.h"
#include "string.h"

#define BLACK 0
#define WHITE 1
#define INVERSE 2

#define SSD1306_I2C_ADDRESS   0x3C  // 011110+SA0+RW - 0x3C or 0x3D
#define SSD1306_128_64

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

void driverSWSSD1306Init(uint8_t switchvcc, uint8_t i2caddr);
void driverSWSSD1306Command(uint8_t c);
void driverSWSSD1306ClearDisplay(void);
void driverSWSSD1306FillBuffer(const uint8_t *newContents,uint16_t size);
void driverSWSSD1306InvertDisplay(uint8_t i);
void driverSWSSD1306Display(void);
void driverSWSSD1306DisplayAsync(void);
void driverSWSSD1306DisplayAsyncEfficient(void);
void driverSWSSD1306Startscrollright(uint8_t start, uint8_t stop);
void driverSWSSD1306Startscrollleft(uint8_t start, uint8_t stop);
void driverSWSSD1306Startscrolldiagright(uint8_t start, uint8_t stop);
void driverSWSSD1306Startscrolldiagleft(uint8_t start, uint8_t stop);
void driverSWSSD1306Stopscroll(void);
void driverSWSSD1306Dim(bool dim);
void driverSWSSD1306DrawPixel(int16_t x, int16_t y, uint16_t color);
void driverSWSSD1306DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void driverSWSSD1306DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void driverSWSSD1306DrawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color);
void driverSWSSD1306DrawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);

#endif
