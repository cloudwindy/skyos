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
 * None-blocking serial recieve.
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
    /* remove received buf */
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
 * Get receive buffer fullness.
 */
size_t serial_recvlen(void)
{
  return rxcur;
}

/**
 * Non-blocking serial send.
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

  if (((USART_CR1(SERIAL_PORT) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(SERIAL_PORT) & USART_SR_RXNE) != 0))
  { /* We were called because of RXNE. */
    if (rxcur < SERIAL_RX_BUFFER_SIZE)
    {
      rxbuf[rxcur] = usart_recv(SERIAL_PORT);
      rxcur++;
    }
    else
    { /* Discard the RX register. */
      usart_recv(SERIAL_PORT);
    }
    return;
  }

  if (((USART_CR1(SERIAL_PORT) & USART_CR1_TXEIE) != 0) &&
      ((USART_SR(SERIAL_PORT) & USART_SR_TXE) != 0))
  { /* We were called because of TXE. */
    int ret = ring_read_ch(&ring, NULL);

    if (ret == -1)
    { /* Disable the TXE interrupt. */
      usart_disable_tx_interrupt(SERIAL_PORT);
    }
    else
    { /* Put data into the TX register. */
      usart_send(SERIAL_PORT, ret);
    }
    return;
  }
}