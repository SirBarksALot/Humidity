#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "ssd1306.h"

static uint8_t ssd1306_buf[1024] = 
{

};

static void SPIwrite( uint8_t data )
{
	uint8_t ii;
	for( ii = 0x80; ii; ii >>= 1)
	{
		SCK_LO;
		if( data & ii )
		{
			MOSI_HI;
		}
		else
		{
			MOSI_LO;
		}
		SCK_HI;
	}
}

void ssd1306_cmd( uint8_t cmd )
{
	
	DC_LO;
	SPIwrite( cmd );
	
}

void ssd1306_data ( uint8_t dat )
{
	
	DC_HI;
	SPIwrite( dat );
	
}

void ssd1306_display( void )
{
	  ssd1306_cmd( SSD1306_COLUMNADDR );
	  ssd1306_cmd( 0 );
	  ssd1306_cmd( SSD1306_WIDTH-1 );
	  ssd1306_cmd( SSD1306_PAGEADDR );
	  ssd1306_cmd( 0 );
	  ssd1306_cmd( 7 );
	
	for ( uint16_t i=0; i<(SSD1306_WIDTH*SSD1306_HEIGHT/8); i++ )
	{
		ssd1306_data( ssd1306_buf[i] );
	}

}

void ssd1306_InitSpi(void)
{
	
	MOSI_DDR	|= MOSI;
	
	SCK_DDR		|= SCK;
	SCK_PORT	|= SCK;
	
	RST_DDR		|= RST;
	RST_PORT	|= RST;
	
	DC_DDR		|=DC;
	
}

void ssd1306_setPixel( int x, int y, uint8_t bw )
{
	if( (x<0) || (x>=SSD1306_WIDTH) || (y<0) || y>=SSD1306_HEIGHT )
	{
		return;
	}
	
	if(bw)
	{
		ssd1306_buf[x+(y/8)*SSD1306_WIDTH] |= (1<<(y%8));
	}
	else
	{
		ssd1306_buf[x+(y/8)*SSD1306_WIDTH] &= ~(1<<(y%8));		
	}
}

void ssd1306_init( uint8_t vcc, uint8_t refresh )
{

	ssd1306_InitSpi();
	
	RST_HI;
	_delay_ms(50);
	RST_LO;
	_delay_ms(50);
	RST_HI;

	ssd1306_cmd( SSD1306_DISPLAYOFF );
	ssd1306_cmd( SSD1306_SETDISPLAYCLOCKDIV );
	ssd1306_cmd( refresh );
	ssd1306_cmd( SSD1306_SETMULTIPLEX );
	ssd1306_cmd( 0x3F );
	ssd1306_cmd( SSD1306_SETDISPLAYOFFSET );
	ssd1306_cmd( 0x0 );
	ssd1306_cmd( SSD1306_SETSTARTLINE | 0x0);
	ssd1306_cmd( SSD1306_CHARGEPUMP );
	
	if( vcc == SSD1306_EXTERNALVCC )
	{
		ssd1306_cmd( 0x10 );
	}
	else
	{
		ssd1306_cmd( 0x14 );
	}
	
	ssd1306_cmd( SSD1306_MEMORYMODE );
	ssd1306_cmd( 0x00 );	
	ssd1306_cmd( SSD1306_SEGREMAP | 0x1 );
	ssd1306_cmd( SSD1306_COMSCANDEC );
	ssd1306_cmd( SSD1306_SETCOMPINS );
	ssd1306_cmd( 0x12 );
	ssd1306_cmd( SSD1306_SETCONTRAST );
	
	if( vcc == SSD1306_EXTERNALVCC )
	{
		ssd1306_cmd( 0x9F );
	}
	else
	{
		ssd1306_cmd( 0xCF );
	}
	
	ssd1306_cmd( SSD1306_SETPRECHARGE );
	
	if( vcc == SSD1306_EXTERNALVCC )
	{
		ssd1306_cmd( 0x22 );
	}
	else
	{
		ssd1306_cmd( 0xF1 );
	}
	
	ssd1306_cmd( SSD1306_SETVCOMDETECT );
	ssd1306_cmd( 0x40 );
	ssd1306_cmd( SSD1306_DEACTIVATE_SCROLL );
	ssd1306_cmd( SSD1306_DISPLAYALLON_RESUME );
	ssd1306_cmd( SSD1306_NORMALDISPLAY );
	ssd1306_cmd( SSD1306_DISPLAYON );

}

