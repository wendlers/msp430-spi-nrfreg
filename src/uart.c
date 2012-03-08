/*
 * This file is part of the MSP430 SPI example.
 *
 * Copyright (C) 2012 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/******************************************************************************
 * SPI example for MSP430
 *
 * Initialize UART on USCI_A at 9600 bauds.
 *
 * Stefan Wendler
 * sw@kaltpost.de
 * http://gpio.kaltpost.de
 ******************************************************************************/

#include <msp430.h>
#include <legacymsp430.h>

#include "uart.h"

/**
 * Receive Data (RXD) at P1.1
 */
#define RXD		BIT1

/**
 * Transmit Data (TXD) at P1.2
 */
#define TXD		BIT2

void uart_init(void)
{
	P1SEL  |= RXD + TXD;                       
  	P1SEL2 |= RXD + TXD;                       
  	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  	UCA0BR0 = 104;                            // 1MHz 9600
  	UCA0BR1 = 0;                              // 1MHz 9600
  	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  	UCA0CTL1 &= ~UCSWRST;                     // Initialize USCI state machine
}

void uart_putc(unsigned char c)
{
	while (!(IFG2&UCA0TXIFG));              // USCI_A0 TX buffer ready?
  	UCA0TXBUF = c;                    		// TX
}

void uart_puts(const char *str)
{
     while(*str) uart_putc(*str++);
}

