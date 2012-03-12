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
 * Initialize SPI on USCI_B to communicate with the Nordic NRF24l01+ 
 *
 * Stefan Wendler
 * sw@kaltpost.de
 * http://gpio.kaltpost.de
 ******************************************************************************/

#include <msp430.h>

#include "spi.h"

/**
 * SPI Clock at P1.5
 */
#define SCLK    BIT5

/**
 * SPI SOMI (Slave Out, Master In) at P1.6
 */
#define SOMI    BIT6

/**
 * SPI SIMO (Slave In, Master Out) at P1.7
 */
#define SIMO    BIT7

/**
 * CS (Chip Select) at P1.3
 */
#define CS      BIT3

void spi_init(void)
{
	/**
	 * From TIs users manual
	 *
     * The recommended USCI initialization/re-configuration process is:
 	 * 1. Set UCSWRST (BIS.B #UCSWRST,&UCxCTL1)
 	 * 2. Initialize all USCI registers with UCSWRST=1 (including UCxCTL1)
 	 * 3. Configure ports
 	 * 4. Clear UCSWRST via software (BIC.B #UCSWRST,&UCxCTL1)
 	 * 5. Enable interrupts (optional) via UCxRXIE and/or UCxTXIE
 	 */

	// (1)
	UCB0CTL1 = UCSWRST;

	// (2)
    P1DIR  |= CS; 
    P1OUT  |= CS;
  	P1SEL  |= SOMI + SIMO + SCLK;
  	P1SEL2 |= SOMI + SIMO + SCLK;

    // (3) 3-pin, 8-bit SPI master
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; 
	UCB0CTL1 |= UCSSEL_2;   // SMCLK

	// (4)	
	UCB0CTL1 &= ~UCSWRST; 

	spi_csh();
}

void spi_csh(void)
{
    P1OUT |= CS;
}

void spi_csl(void)
{
    P1OUT &= ~CS;
}

unsigned char spi_xfer_byte(unsigned char data)
{
  	UCB0TXBUF = data; 

	// wait for TX 
	while (!(IFG2 & UCB0TXIFG)); 	

	return UCB0RXBUF;
}
