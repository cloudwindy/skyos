#include "setup.h"
#include "led.h"
#include "delay.h"
#include "ssd1306.h"
#include "tty.h"
#include "keypad.h"

#include <libopencm3/cm3/systick.h>
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
static void setup_systick(void);
static void setup_gpio(void);
static void setup_timer(void);
static void setup_rtc(void);
static void setup_spi(void);
static void setup_i2c(void);
static void setup_uart(void);

/**
 * 初始化函数
 */
void setup(void)
{
  (void)setup_iwdg;
  // setup_iwdg();
  setup_rcc();
  setup_systick();
  setup_gpio();
  setup_timer();
  setup_rtc();
  setup_spi();
  setup_i2c();
  setup_uart();
  setup_led();
  setup_keypad();
  setup_ssd1306();
  setup_tty();
  led_off();
}

/**
 * 独立看门狗配置
 *
 * 此项配置应当放在最前
 */
static void setup_iwdg(void)
{
  iwdg_set_period_ms(3276);
  iwdg_start();
}

/**
 * RCC 时钟配置
 */
static void setup_rcc(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

/**
 * SysTick 时钟配置
 *
 * 时钟源 系统时钟
 * 分频系数 8000
 */
static void setup_systick(void)
{
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
  systick_set_reload(rcc_ahb_frequency / 8 / 1000 - 1);
  systick_counter_enable();
  systick_interrupt_enable();
}

/**
 * GPIO 端口配置
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
}

/**
 * TIM 计时器配置
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
 * RTC 计时器配置
 */
static void setup_rtc(void)
{
  rtc_auto_awake(RCC_LSE, 0x7FFF);
  nvic_enable_irq(NVIC_RTC_IRQ);
  nvic_set_priority(NVIC_RTC_IRQ, 1);
  rtc_interrupt_enable(RTC_SEC);
}

/**
 * SPI 端口配置
 *
 * 数据位数 8 位
 * 波特率 8 Mhz
 * 时钟极性 低
 * 时钟相位 1
 * 端序 大端序
 */
static void setup_spi(void)
{
  rcc_periph_clock_enable(RCC_SPI1);
  rcc_periph_reset_pulse(RST_SPI1);

  gpio_set_mode(GPIO_BANK_SPI1_SCK, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_SPI1_SCK | GPIO_SPI1_MOSI);

  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
  spi_set_bidirectional_transmit_only_mode(SPI1);
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  spi_enable(SPI1);
}

/**
 * I2C 端口配置
 */
static void setup_i2c(void)
{
  rcc_periph_clock_enable(RCC_I2C1);
  rcc_periph_reset_pulse(RST_I2C1);

  gpio_set_mode(GPIO_BANK_I2C1_SCL, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO_I2C1_SCL | GPIO_I2C1_SDA);

  i2c_peripheral_disable(I2C1);
  i2c_set_speed(I2C1, i2c_speed_sm_100k, rcc_apb1_frequency / 1e6);
  i2c_peripheral_enable(I2C1);
}

/**
 * UART 端口配置
 */
static void setup_uart(void)
{
  rcc_periph_clock_enable(RCC_USART1);
  rcc_periph_reset_pulse(RST_USART1);

  gpio_set_mode(GPIO_BANK_USART1_TX, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
  gpio_set_mode(GPIO_BANK_USART1_RX, GPIO_MODE_INPUT,
                GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_USART1_RX);

  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  usart_enable(USART1);
}