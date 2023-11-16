#include "serial.h"
#include "ring.h"
#include "led.h"
#include "delay.h"

#include <stdlib.h>
#include <string.h>
#include <libopencm3/stm32/usart.h>

#define SERIAL_PORT (USART1)

uint8_t txbuf[SERIAL_TX_BUFFER_SIZE];
uint8_t rxbuf[SERIAL_RX_BUFFER_SIZE];
size_t rxcur;
Ring ring = {
    .data = txbuf,
    .size = SERIAL_TX_BUFFER_SIZE,
    .begin = 0,
    .end = 0};

/**
 * 串口异步接收
 */
int serial_recv(char *buf, size_t len)
{
  int ret;
  if (len == 0 || rxcur == 0)
  {
    return 0;
  }
  if (rxcur > len)
  {
    /* Remove received buf */
    memcpy(buf, rxbuf, len);
    memmove(rxbuf, rxbuf + len, rxcur - len);
    ret = len;
    rxcur -= len;
  }
  else
  {
    /* RX buf not enough */
    memcpy(buf, rxbuf, rxcur);
    ret = rxcur;
    rxcur = 0;
  }
  return ret;
}

/**
 * 串口异步发送
 */
int serial_send(const char *s, size_t len)
{
  int ret = ring_write(&ring, (uint8_t *)s, len);
  usart_enable_tx_interrupt(SERIAL_PORT);
  if (ret < 0)
  {
    return -1;
  }
  return ret;
}

void serial_handler(void)
{
  /* Check if we were called because of RXNE. */
  if (((USART_CR1(SERIAL_PORT) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(SERIAL_PORT) & USART_SR_RXNE) != 0))
  {
    if (rxcur < SERIAL_RX_BUFFER_SIZE)
    {
      rxbuf[rxcur] = usart_recv(SERIAL_PORT);
      rxcur++;
    }
    else
    {
      /* 丢弃数据 */
      usart_recv(SERIAL_PORT);
    }
    return;
  }

  /* Check if we were called because of TXE. */
  if (((USART_CR1(SERIAL_PORT) & USART_CR1_TXEIE) != 0) &&
      ((USART_SR(SERIAL_PORT) & USART_SR_TXE) != 0))
  {
    int ret = ring_read_ch(&ring, NULL);

    if (ret == -1)
    {
      /* Disable the TXE interrupt, it's no longer needed. */
      usart_disable_tx_interrupt(SERIAL_PORT);
    }
    else
    {
      /* Put data into the transmit register. */
      usart_send(SERIAL_PORT, ret);
    }
    return;
  }
}