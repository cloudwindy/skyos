#include "setup.h"
#include "led.h"
#include "delay.h"
#include "ssd1306.h"
#include "ssd1306_conf.h"
#include "tty.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>

static void setup_iwdg(void);
static void setup_rcc(void);
static void setup_gpio(void);
static void setup_timer(void);
static void setup_rtc(void);
static void setup_spi(void);
static void setup_i2c(void);
static void setup_usart(void);

/**
 * Start setup routine.
 * 
 * Notice that RTOS is not ready at this point, so do not
 * call any RTOS functions in setup_*().
 */
void setup(void)
{
  setup_iwdg();
  setup_rcc();
  setup_timer();
  setup_rtc();
  setup_gpio();
  setup_spi();
  setup_i2c();
  setup_usart();
  setup_led();
  setup_ssd1306();
  setup_tty();
  led_off();
}

/**
 * IWDG Setup
 *
 * Period 3.2 s
 * This should be on the top of setup routine.
 */
static void setup_iwdg(void)
{
  iwdg_set_period_ms(3276);
  iwdg_start();
}

/**
 * RCC Clock Setup
 * 
 * Source     HSE (8 MHz)
 * Multiplier 9
 * Frequency  72 MHz
 * Period     14 ns
 */
static void setup_rcc(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

/**
 * TIM Timer Setup
 *
 * Source    AHB
 * Divider   AHB / 1000000
 * Frequency 1 MHz
 * Period    1 us
 */
static void setup_timer(void)
{
  rcc_periph_clock_enable(RCC_TIM3);
  rcc_periph_reset_pulse(RST_TIM3);

  timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT,
                 TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM3, rcc_ahb_frequency / 1e6 - 1);
  timer_disable_preload(TIM3);
  timer_continuous_mode(TIM3);
  timer_set_period(TIM3, 0xFFFF);
  timer_enable_counter(TIM3);
}

/**
 * RTC Timer Setup
 *
 * Source    LSE (32.768 kHz)
 * Divider   32768
 * Frequency 1 Hz
 * Period    1 s
 */
static void setup_rtc(void)
{
  rtc_auto_awake(RCC_LSE, 0x7FFF);
}

/**
 * GPIO Ports Setup
 *
 * Set all unused pins as analog input to save power.
 * Set SPI and I2C pins as alternative function output.
 * Set SSD1306 control pins.
 * Set keyboard scan pins.
 */
static void setup_gpio(void)
{
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);

  gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, GPIO_ALL);
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_ANALOG, GPIO_ALL);

  gpio_set_mode(GPIO_BANK_SPI1_SCK, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_SPI1_SCK | GPIO_SPI1_MOSI);

  gpio_set_mode(GPIO_BANK_I2C1_SCL, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SCL | GPIO_I2C1_SDA);

  gpio_set_mode(SSD1306_BANK_RES, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, SSD1306_RES);
  gpio_set_mode(SSD1306_BANK_DC, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, SSD1306_DC);
  gpio_set_mode(SSD1306_BANK_CS, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, SSD1306_CS);

  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, GPIO0 | GPIO1 | GPIO2 | GPIO3);
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_PULL_UPDOWN, GPIO6 | GPIO7 | GPIO8 | GPIO9);
}

/**
 * SPI Port Setup
 *
 * Source   APB
 * Baud     9 Mbps
 * Format   8 bits
 * Polarity Low
 * Phase    1
 * Endian   Big
 */
static void setup_spi(void)
{
  rcc_periph_clock_enable(RCC_SPI1);
  rcc_periph_reset_pulse(RST_SPI1);

  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

  /* TODO: Switch to full duplex after connecting to flash chip. */
  spi_set_bidirectional_transmit_only_mode(SPI1);

  /* This is necessary to manage Chip Select signal by ourself. */
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);

  spi_enable(SPI1);
}

/**
 * I2C Port Setup
 *
 * Source APB
 * Baud   100 kbps
 */
static void setup_i2c(void)
{
  rcc_periph_clock_enable(RCC_I2C1);
  rcc_periph_reset_pulse(RST_I2C1);

  i2c_peripheral_disable(I2C1);
  i2c_set_speed(I2C1, i2c_speed_sm_100k, rcc_apb1_frequency / 1e6);
  i2c_peripheral_enable(I2C1);
}

/**
 * USART Port Setup
 * 
 * Source       APB
 * Baud         115.2 kbps
 * Data         8 bits
 * Stop         1 bit
 * Parity       None
 * Flow Control None
 */
void setup_usart(void)
{
  rcc_periph_clock_enable(RCC_USART1);
  rcc_periph_reset_pulse(RST_USART1);

  gpio_set_mode(GPIO_BANK_USART1_TX, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
  gpio_set_mode(GPIO_BANK_USART1_RX, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_USART1_RX);

  nvic_enable_irq(NVIC_USART1_IRQ);

  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  usart_enable_rx_interrupt(USART1);
  usart_enable(USART1);
}