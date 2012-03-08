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
 * This example reads the config register of a Nordic NRF24l01+ connected to
 * USCI_B SPI of the MSP460g2553. The register contents then is printed out
 * through USCI_A UART. When using the TI Launchpad, /dev/ttyACM0 could be
 * used as serial port at 9600 Bauds, but RX/TX must be crossed on the jumper
 * bridge of the Launchpad.
 * <br/><br/>
 * The wiring for this example is as follows:
 * <br/><br/>
 * <pre>
 * MSP430					To
 * ----------------------------------------
 * RXD  P1.1				TXD serial line
 * TXD	P1.2				RXD serial line
 * SCLK P1.5				NRF SCK	
 * SOMI P1.6				NRF MO
 * SIMO P1.7				NRF MI
 * CS   P1.3				NRF CS
 * </pre>
 * <br/><br/>
 * For the NRF also connect the following:
 * <br/><br/>
 * <pre>
 * NRF						To
 * ----------------------------------------
 * VCC						3.3v
 * CE						3.3v
 * GND						GND
 * </pre>
 * <br/><br/>
 * Stefan Wendler
 * sw@kaltpost.de
 * http://gpio.kaltpost.de
 *
 ******************************************************************************/

#include <msp430.h>

#include "uart.h"
#include "spi.h"

/**
 * Command to send for requesting to read a register from the NRF
 */
#define NRF_CMD_RREG		0b00000000

/**
 * Command to send when only reading a result from the NRF
 */
#define NRF_CMD_NOP			0b11111111

/**
 * Address of the configuration register of the NRF
 */
#define NRF_REG_CONFIG		0x00

void uart_printi(int n)
{
     int buffer[16];
     int i;
     int j;

     if(n == 0) {
          uart_putc('0');
          return;
     }

     for (i = 15; i > 0 && n > 0; i--) {
          buffer[i] = (n%10)+'0';
          n /= 10;
     }

     for(j = i+1; j <= 15; j++) {
          uart_putc(buffer[j]);
     }
}


/**
 * Print the value in binary for a regisrer.
 *
 * @param[in]	*name	register name to print
 * @param[in]	*n		value of the register to print in binary format
 */
void printreg(char *name, int n)
{
	int size = 8;
    int i;

	uart_puts((char *)"- "); 
	uart_puts(name); 
	uart_puts((char *)": "); 

    int mask = 1 << (size - 1);

    for(i = 0; i < size; i++) {
         if((n & (mask >> i)) != 0) {
             uart_putc('1');
         } else {
             uart_putc('0');
         }
    }
	uart_puts((char *)" ("); 
	uart_printi(n);
	uart_puts((char *)")\n\r"); 
}

/**
 * Main routine
 */
int main(void)
{
    WDTCTL  = WDTPW + WDTHOLD; 	// Stop WDT
	BCSCTL1 = CALBC1_1MHZ;      // Set DCO
  	DCOCTL  = CALDCO_1MHZ;
 
	uart_init();
	spi_init();
    	
	uart_puts((char *)"*** MSP430 SPI Example ***\n\r\n\r");
	
	unsigned char stat;
	unsigned char conf;

    spi_csl();

    // send command
    stat = spi_xfer_byte(NRF_CMD_RREG | NRF_REG_CONFIG);

	// read response
    conf = spi_xfer_byte(NRF_CMD_NOP);

    spi_csh();

	uart_puts((char *)"NRF24l01 registers:\n\r");
	printreg((char *)"status", stat);
	printreg((char *)"config", conf);

    while(1);
}

