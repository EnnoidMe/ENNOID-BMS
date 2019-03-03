#include "driverSWSSD1306.h"

static uint8_t displayBufferReal[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];
static uint8_t displayBufferDesired[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];

int8_t _i2caddr, _vccstate;

#define ssd1306_swap(a, b) { int16_t t = a; a = b; b = t; }

void driverSWSSD1306Init(uint8_t switchvcc, uint8_t i2caddr){
	driverHWI2C1Init();
	
  _vccstate = switchvcc;
  _i2caddr = i2caddr;

  // Init sequence
  driverSWSSD1306Command(SSD1306_DISPLAYOFF);                    // 0xAE
  driverSWSSD1306Command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  driverSWSSD1306Command(0x80);                                  // the suggested ratio 0x80

  driverSWSSD1306Command(SSD1306_SETMULTIPLEX);                  // 0xA8
  driverSWSSD1306Command(SSD1306_LCDHEIGHT - 1);

  driverSWSSD1306Command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  driverSWSSD1306Command(0x0);                                   // no offset
  driverSWSSD1306Command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  driverSWSSD1306Command(SSD1306_CHARGEPUMP);                    // 0x8D
  if (switchvcc == SSD1306_EXTERNALVCC)
    { driverSWSSD1306Command(0x10); }
  else
    { driverSWSSD1306Command(0x14); }
  driverSWSSD1306Command(SSD1306_MEMORYMODE);                    // 0x20
  driverSWSSD1306Command(0x00);                                  // 0x0 act like ks0108
  driverSWSSD1306Command(SSD1306_SEGREMAP | 0x1);
  driverSWSSD1306Command(SSD1306_COMSCANDEC);

 #if defined SSD1306_128_32
  driverSWSSD1306Command(SSD1306_SETCOMPINS);                    // 0xDA
  driverSWSSD1306Command(0x02);
  driverSWSSD1306Command(SSD1306_SETCONTRAST);                   // 0x81
  driverSWSSD1306Command(0x8F);

#elif defined SSD1306_128_64
  driverSWSSD1306Command(SSD1306_SETCOMPINS);                    // 0xDA
  driverSWSSD1306Command(0x12);
  driverSWSSD1306Command(SSD1306_SETCONTRAST);                   // 0x81
  if (switchvcc == SSD1306_EXTERNALVCC)
    { driverSWSSD1306Command(0x9F); }
  else
    { driverSWSSD1306Command(0xCF); }
#endif

  driverSWSSD1306Command(SSD1306_SETPRECHARGE);                  // 0xd9
  if (switchvcc == SSD1306_EXTERNALVCC)
    { driverSWSSD1306Command(0x22); }
  else
    { driverSWSSD1306Command(0xF1); }
  driverSWSSD1306Command(SSD1306_SETVCOMDETECT);                 // 0xDB
  driverSWSSD1306Command(0x40);
  driverSWSSD1306Command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  driverSWSSD1306Command(SSD1306_NORMALDISPLAY);                 // 0xA6
  driverSWSSD1306Command(SSD1306_DEACTIVATE_SCROLL);
  driverSWSSD1306Command(SSD1306_DISPLAYON);//--turn on oled panel
	driverSWSSD1306ClearDisplay();
};

void driverSWSSD1306Command(uint8_t c){
	uint8_t writeData[2] = {0x00,c};
	driverHWI2C1Write(_i2caddr,false,writeData,2);
};

void driverSWSSD1306ClearDisplay(void){
  memset(displayBufferDesired, 0x00, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
  memset(displayBufferReal, 0xFF, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));	
};

void driverSWSSD1306FillBuffer(const uint8_t *newContents,uint16_t size){
	memcpy(displayBufferDesired,newContents,size);
};

void driverSWSSD1306InvertDisplay(uint8_t i){
  if (i) {
    driverSWSSD1306Command(SSD1306_INVERTDISPLAY);
  } else {
    driverSWSSD1306Command(SSD1306_NORMALDISPLAY);
  }
};

void driverSWSSD1306Display(void){	
  driverSWSSD1306Command(SSD1306_COLUMNADDR);
  driverSWSSD1306Command(0);   // Column start address (0 = reset)
  driverSWSSD1306Command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

  driverSWSSD1306Command(SSD1306_PAGEADDR);
  driverSWSSD1306Command(0); // Page start address (0 = reset)
  #if SSD1306_LCDHEIGHT == 64
    driverSWSSD1306Command(7); // Page end address
  #endif
  #if SSD1306_LCDHEIGHT == 32
    driverSWSSD1306Command(3); // Page end address
  #endif
  #if SSD1306_LCDHEIGHT == 16
    driverSWSSD1306Command(1); // Page end address
  #endif

	uint8_t writeData[1+16];
	writeData[0] = SSD1306_SETSTARTLINE;
	
	for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i += 16) {
		memcpy(writeData + 1,displayBufferDesired + i,16);									// Dont overwrite the register pointer
		memcpy(displayBufferReal,displayBufferDesired + i,16);							// Update what is written to the display
		driverHWI2C1Write(_i2caddr,false,writeData,(1+16));
	}
	
	/*
	//This code update a single vertical line on the display
	uint8_t temp[9] = {SSD1306_SETSTARTLINE, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  driverSWSSD1306Command(SSD1306_COLUMNADDR);
  driverSWSSD1306Command(tempColumn);   // Column start address (0 = reset)
  driverSWSSD1306Command(tempColumn); // Column end address (127 = reset)
	
	memcpy(temp + 1,displayBufferDesired + tempColumn*8,8);									// Dont overwrite the register pointer
	
	driverHWI2C1Write(_i2caddr,false,temp,(1+9));
	
	tempColumn++;
	tempColumn %= 127;
	*/
};

void driverSWSSD1306DisplayAsync(void){
	static uint8_t writeData[1+16];
	static uint16_t pixelByte = 0;
	writeData[0] = SSD1306_SETSTARTLINE;
	
	if(!pixelByte) {
		driverSWSSD1306Command(SSD1306_COLUMNADDR);
		driverSWSSD1306Command(0);   // Column start address (0 = reset)
		driverSWSSD1306Command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

		driverSWSSD1306Command(SSD1306_PAGEADDR);
		driverSWSSD1306Command(0); // Page start address (0 = reset)
		#if SSD1306_LCDHEIGHT == 64
			driverSWSSD1306Command(7); // Page end address
		#endif
		#if SSD1306_LCDHEIGHT == 32
			driverSWSSD1306Command(3); // Page end address
		#endif
		#if SSD1306_LCDHEIGHT == 16
			driverSWSSD1306Command(1); // Page end address
		#endif
	}
	
	memcpy(writeData + 1,displayBufferDesired + pixelByte,16);									// Dont overwrite the register pointer
	memcpy(displayBufferReal,displayBufferDesired + pixelByte,16);							// Update what is written to the display
	driverHWI2C1Write(_i2caddr,false,writeData,(1+16));
	
	pixelByte += 16;
	pixelByte %= SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8;
};

void driverSWSSD1306DisplayAsyncEfficient(void) {
	static uint8_t writeData[1+16];
	static uint16_t pixelByte = 0;
	writeData[0] = SSD1306_SETSTARTLINE;
	
	if(!pixelByte) {
		driverSWSSD1306Command(SSD1306_COLUMNADDR);
		driverSWSSD1306Command(0);   // Column start address (0 = reset)
		driverSWSSD1306Command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

		driverSWSSD1306Command(SSD1306_PAGEADDR);
		driverSWSSD1306Command(0); // Page start address (0 = reset)
		#if SSD1306_LCDHEIGHT == 64
			driverSWSSD1306Command(7); // Page end address
		#endif
		#if SSD1306_LCDHEIGHT == 32
			driverSWSSD1306Command(3); // Page end address
		#endif
		#if SSD1306_LCDHEIGHT == 16
			driverSWSSD1306Command(1); // Page end address
		#endif
	}
	
	if(memcmp(displayBufferReal,displayBufferDesired,SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)) {
		memcpy(writeData + 1,displayBufferDesired + pixelByte,16);									// Dont overwrite the register pointer
		memcpy(displayBufferReal + pixelByte,displayBufferDesired + pixelByte,16);							// Update what is written to the display
		driverHWI2C1Write(_i2caddr,false,writeData,(1+16));
		
		pixelByte += 16;
		pixelByte %= SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8;
	};
};

void driverSWSSD1306Startscrollright(uint8_t start, uint8_t stop){
  driverSWSSD1306Command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(start);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(stop);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(0XFF);
  driverSWSSD1306Command(SSD1306_ACTIVATE_SCROLL);
};

void driverSWSSD1306Startscrollleft(uint8_t start, uint8_t stop){
  driverSWSSD1306Command(SSD1306_LEFT_HORIZONTAL_SCROLL);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(start);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(stop);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(0XFF);
  driverSWSSD1306Command(SSD1306_ACTIVATE_SCROLL);
};

void driverSWSSD1306Startscrolldiagright(uint8_t start, uint8_t stop){
  driverSWSSD1306Command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(SSD1306_LCDHEIGHT);
  driverSWSSD1306Command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(start);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(stop);
  driverSWSSD1306Command(0X01);
  driverSWSSD1306Command(SSD1306_ACTIVATE_SCROLL);
};

void driverSWSSD1306Startscrolldiagleft(uint8_t start, uint8_t stop){
  driverSWSSD1306Command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(SSD1306_LCDHEIGHT);
  driverSWSSD1306Command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(start);
  driverSWSSD1306Command(0X00);
  driverSWSSD1306Command(stop);
  driverSWSSD1306Command(0X01);
  driverSWSSD1306Command(SSD1306_ACTIVATE_SCROLL);
};

void driverSWSSD1306Stopscroll(void){
  driverSWSSD1306Command(SSD1306_DEACTIVATE_SCROLL);
};

void driverSWSSD1306Dim(bool dim){
  uint8_t contrast;

  if (dim) {
    contrast = 0; // Dimmed display
  } else {
    if (_vccstate == SSD1306_EXTERNALVCC) {
      contrast = 0x9F;
    } else {
      contrast = 0xCF;
    }
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  driverSWSSD1306Command(SSD1306_SETCONTRAST);
  driverSWSSD1306Command(contrast);
};

void driverSWSSD1306DrawPixel(int16_t x, int16_t y, uint16_t color){
	uint16_t WIDTH = libGraphicsWidth();
	uint16_t HEIGHT = libGraphicsHeight();	
	
  if ((x < 0) || (x >= libGraphicsWidth()) || (y < 0) || (y >= libGraphicsHeight()))
    return;

  // check rotation, move pixel around if necessary
  switch (libGraphicsGetRotation()) {
  case 1:
    ssd1306_swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
  case 3:
    ssd1306_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  // x is which column
    switch (color)
    {
      case WHITE:   displayBufferDesired[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
      case BLACK:   displayBufferDesired[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break;
      case INVERSE: displayBufferDesired[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break;
    }
};

void driverSWSSD1306DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
	uint16_t WIDTH = libGraphicsWidth();
	uint16_t HEIGHT = libGraphicsHeight();
  bool bSwap = false;
  switch(libGraphicsGetRotation()) {
    case 0:
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x and adjust x for h (now to become w)
      bSwap = true;
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      x -= (h-1);
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      y -= (h-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y
      bSwap = true;
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      break;
  }

  if(bSwap) {
    driverSWSSD1306DrawFastHLineInternal(x, y, h, color);
  } else {
    driverSWSSD1306DrawFastVLineInternal(x, y, h, color);
  }
};

void driverSWSSD1306DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
	uint16_t WIDTH = libGraphicsWidth();
	uint16_t HEIGHT = libGraphicsHeight();	
  bool bSwap = false;
	
  switch(libGraphicsGetRotation()) {
    case 0:
      // 0 degree rotation, do nothing
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x
      bSwap = true;
      ssd1306_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      x -= (w-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
      bSwap = true;
      ssd1306_swap(x, y);
      y = HEIGHT - y - 1;
      y -= (w-1);
      break;
  }

  if(bSwap) {
    driverSWSSD1306DrawFastVLineInternal(x, y, w, color);
  } else {
    driverSWSSD1306DrawFastHLineInternal(x, y, w, color);
  }
};

inline void driverSWSSD1306DrawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color){
	uint16_t WIDTH = libGraphicsWidth();
	uint16_t HEIGHT = libGraphicsHeight();
  // do nothing if we're off the left or right side of the screen
  if(x < 0 || x >= WIDTH) { return; }

  // make sure we don't try to draw below 0
  if(__y < 0) {
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;

  }

  // make sure we don't go past the height of the display
  if( (__y + __h) > HEIGHT) {
    __h = (HEIGHT - __y);
  }

  // if our height is now negative, punt
  if(__h <= 0) {
    return;
  }

  // this display doesn't need ints for coordinates, use local byte registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;


  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = displayBufferDesired;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * SSD1306_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  // do the first partial byte, if necessary - this requires some masking
  register uint8_t mod = (y&7);
  if(mod) {
    // mask off the high n bits we want to set
    mod = 8-mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this byte
    if( h < mod) {
      mask &= (0XFF >> (mod-h));
    }

  switch (color)
    {
    case WHITE:   *pBuf |=  mask;  break;
    case BLACK:   *pBuf &= ~mask;  break;
    case INVERSE: *pBuf ^=  mask;  break;
    }

    // fast exit if we're done here!
    if(h<mod) { return; }

    h -= mod;

    pBuf += SSD1306_LCDWIDTH;
  }


  // write solid bytes while we can - effectively doing 8 rows at a time
  if(h >= 8) {
    if (color == INVERSE)  {          // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
      do  {
      *pBuf=~(*pBuf);

        // adjust the buffer forward 8 rows worth of data
        pBuf += SSD1306_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    else {
      // store a local value to work with
      register uint8_t val = (color == WHITE) ? 255 : 0;

      do  {
        // write our value in
      *pBuf = val;

        // adjust the buffer forward 8 rows worth of data
        pBuf += SSD1306_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    }

  // now do the final partial byte, if necessary
  if(h) {
    mod = h & 7;
    // this time we want to mask the low bits of the byte, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    register uint8_t mask = postmask[mod];
    switch (color)
    {
      case WHITE:   *pBuf |=  mask;  break;
      case BLACK:   *pBuf &= ~mask;  break;
      case INVERSE: *pBuf ^=  mask;  break;
    }
  }
};

inline void driverSWSSD1306DrawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color){
	uint16_t WIDTH = libGraphicsWidth();
	uint16_t HEIGHT = libGraphicsHeight();
  // Do bounds/limit checks
  if(y < 0 || y >= HEIGHT) { return; }

  // make sure we don't try to draw below 0
  if(x < 0) {
    w += x;
    x = 0;
  }

  // make sure we don't go off the edge of the display
  if( (x + w) > WIDTH) {
    w = (WIDTH - x);
  }

  // if our width is now negative, punt
  if(w <= 0) { return; }

  // set up the pointer for  movement through the buffer
  register uint8_t *pBuf = displayBufferDesired;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * SSD1306_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  register uint8_t mask = 1 << (y&7);

  switch (color)
  {
		case WHITE:         while(w--) { *pBuf++ |= mask; }; break;
    case BLACK: mask = ~mask;   while(w--) { *pBuf++ &= mask; }; break;
		case INVERSE:         while(w--) { *pBuf++ ^= mask; }; break;
  }
};
