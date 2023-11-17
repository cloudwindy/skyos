#include "ssd1306.h"
#include "delay.h"
#include "led.h"

#include <math.h>
#include <stdlib.h>
#include <string.h> // For memcpy

static void ssd1306_reset(void);
static void ssd1306_write_command(uint8_t command);
static void ssd1306_write_data(uint8_t *data, size_t size);
static void ssd1306_select(void);
static void ssd1306_deselect(void);

/* Initialize the oled screen */
void setup_ssd1306(void)
{
  gpio_set_mode(SSD1306_BANK_RES, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, SSD1306_RES);
  gpio_set_mode(SSD1306_BANK_DC, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, SSD1306_DC);
  gpio_set_mode(SSD1306_BANK_CS, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, SSD1306_CS);

  // Reset OLED
  ssd1306_reset();

  // Wait for the screen to boot
  delay(100);

  // Init OLED
  ssd1306_select();
  ssd1306_set_display_on(0); // display off

  ssd1306_write_command(0x20); // Set Memory Addressing Mode
  ssd1306_write_command(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                               // 10b,Page Addressing Mode (RESET); 11b,Invalid

  ssd1306_write_command(0xB0); // Set Page Start Address for Page Addressing Mode,0-7

#ifdef SSD1306_MIRROR_VERT
  ssd1306_write_command(0xC0); // Mirror vertically
#else
  ssd1306_write_command(0xC8); // Set COM Output Scan Direction
#endif

  ssd1306_write_command(0x00); //---set low column address
  ssd1306_write_command(0x10); //---set high column address

  ssd1306_write_command(0x40); //--set start line address - CHECK

  ssd1306_set_contrast(0xFF);

#ifdef SSD1306_MIRROR_HORIZ
  ssd1306_write_command(0xA0); // Mirror horizontally
#else
  ssd1306_write_command(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef SSD1306_INVERSE_COLOR
  ssd1306_write_command(0xA7); //--set inverse color
#else
  ssd1306_write_command(0xA6); //--set normal color
#endif

// Set multiplex ratio.
#if (SSD1306_HEIGHT == 128)
  // Found in the Luma Python lib for SH1106.
  ssd1306_write_command(0xFF);
#else
  ssd1306_write_command(0xA8); //--set multiplex ratio(1 to 64) - CHECK
#endif

#if (SSD1306_HEIGHT == 32)
  ssd1306_write_command(0x1F); //
#elif (SSD1306_HEIGHT == 64)
  ssd1306_write_command(0x3F); //
#elif (SSD1306_HEIGHT == 128)
  ssd1306_write_command(0x3F); // Seems to work for 128px high displays too.
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

  ssd1306_write_command(0xA4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content

  ssd1306_write_command(0xD3); //-set display offset - CHECK
  ssd1306_write_command(0x00); //-not offset

  ssd1306_write_command(0xD5); //--set display clock divide ratio/oscillator frequency
  ssd1306_write_command(0xF0); //--set divide ratio

  ssd1306_write_command(0xD9); //--set pre-charge period
  ssd1306_write_command(0x22); //

  ssd1306_write_command(0xDA); //--set com pins hardware configuration - CHECK
#if (SSD1306_HEIGHT == 32)
  ssd1306_write_command(0x02);
#elif (SSD1306_HEIGHT == 64)
  ssd1306_write_command(0x12);
#elif (SSD1306_HEIGHT == 128)
  ssd1306_write_command(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

  ssd1306_write_command(0xDB); //--set vcomh
  ssd1306_write_command(0x20); // 0x20,0.77xVcc

  ssd1306_write_command(0x8D); //--set DC-DC enable
  ssd1306_write_command(0x14); //
  ssd1306_set_display_on(1);   //--turn on SSD1306 panel
  ssd1306_deselect();
}

/* Write the screenbuffer with changed to the screen */
void ssd1306_update(uint8_t *buf)
{
  // Write data to each page of RAM. Number of pages
  // depends on the screen height:
  //
  //  * 32px   ==  4 pages
  //  * 64px   ==  8 pages
  //  * 128px  ==  16 pages
  ssd1306_select();
  for (uint8_t i = 0; i < SSD1306_HEIGHT / 8; i++)
  {
    ssd1306_write_command(0xB0 + i); // Set the current RAM page address.
    ssd1306_write_command(0x00 + SSD1306_X_OFFSET_LOWER);
    ssd1306_write_command(0x10 + SSD1306_X_OFFSET_UPPER);
    ssd1306_write_data(&buf[SSD1306_WIDTH * i], SSD1306_WIDTH);
  }
  ssd1306_deselect();
}

void ssd1306_set_contrast(const uint8_t value)
{
  const uint8_t contrast_control_register = 0x81;
  ssd1306_write_command(contrast_control_register);
  ssd1306_write_command(value);
}

void ssd1306_set_display_on(bool on)
{
  ssd1306_write_command(on ? 0xAF : 0xAE);
}

static void ssd1306_reset(void)
{
  // CS = High (not selected)
  ssd1306_deselect();

  // Reset the OLED
  gpio_clear(SSD1306_BANK_RES, SSD1306_RES);
  delay(3);
  gpio_set(SSD1306_BANK_RES, SSD1306_RES);
}

// Send a byte to the command register
static void ssd1306_write_command(uint8_t command)
{
  // switch to command mode
  gpio_clear(SSD1306_BANK_DC, SSD1306_DC);
  spi_send(SSD1306_SPI, (uint16_t)command);
}

// Send data
static void ssd1306_write_data(uint8_t *data, size_t size)
{
  // delay before switching to data mode
  udelay(5);
  gpio_set(SSD1306_BANK_DC, SSD1306_DC);
  while (--size)
  {
    spi_send(SSD1306_SPI, (uint16_t)*data++);
  }
}

static void ssd1306_select(void)
{
  gpio_clear(SSD1306_BANK_CS, SSD1306_CS);
}

static void ssd1306_deselect(void)
{
  // delay before deselecting
  udelay(3);
  gpio_set(SSD1306_BANK_CS, SSD1306_CS);
}