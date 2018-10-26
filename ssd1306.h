#ifndef SSD1306_H_
#define SSD1306_H_

#define SSD1306_128_64

#define	SCK		(1<<PD6)				//<-SCL
#define	MOSI	(1<<PD7)				//<-SDA	
#define	RST		(1<<PC1)				//<-RST	
#define	DC		(1<<PC0)				//<-D/C	

#define SCK_PORT	PORTD
#define SCK_DDR		DDRD

#define MOSI_PORT	PORTD
#define MOSI_DDR	DDRD

#define RST_PORT	PORTC
#define RST_DDR		DDRC

#define DC_PORT		PORTC
#define DC_DDR		DDRC

#define SCK_LO		SCK_PORT &= ~SCK
#define SCK_HI		SCK_PORT |= SCK

#define MOSI_LO		MOSI_PORT &= ~MOSI
#define MOSI_HI		MOSI_PORT |= MOSI

#define RST_LO		RST_PORT &= ~RST
#define RST_HI		RST_PORT |= RST

#define DC_LO		DC_PORT &= ~DC
#define DC_HI		DC_PORT |= DC

#define SSD1306_WIDTH	128
#define SSD1306_HEIGHT	64

#define REFRESH_MIN		0x80
#define REFRESH_MID		0xB0
#define REFRESH_MAX		0xF0

typedef enum {page0, page1, page2, page3, page4, page5, page6, page7} TPAGE;

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
#define SSD1306_SEGREMAP 0xA1
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_NOP 0xE3

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#define swap( a, b ) { int16_t t = a; a = b; b=t; }

//deklaracje zmiennych
extern const uint8_t font[];
extern int cursor_x, cursor_y;

//funkcje
void ssd1306_init( uint8_t vcc, uint8_t refresh );
void ssd1306_cmd( uint8_t cmd );
void ssd1306_data( uint8_t dat );
void ssd1306_display( void );
void ssd1306_InitSpi( void );
void ssd1306_setPixel( int x, int y, uint8_t bw );

void ssd1306_drawBitmap_P( int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color );
void ssd1306_drawChar( int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size );
void ssd1306_fillRect( int x, int y, int w, int h, uint8_t color );
void ssd1306_drawFastVLine(int x, int y, int h, uint8_t color);
void ssd1306_drawLine( int x0, int y0, int x1, int y1, uint8_t color);
void ssd1306_puts( int x, int y, char *str, uint8_t txt_size, uint8_t color, uint8_t bg );
void ssd1306_put_int( int x, int y, int data, uint8_t txt_size, uint8_t color, uint8_t bg );

#endif
