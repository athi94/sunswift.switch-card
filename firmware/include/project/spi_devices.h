/* -------------------------------------------------------------------------- 
	SPI Devices 
	 
	File name: spi_devices.h
	Author: David Snowdon 
	Date: 12/04/02 
   ------------------------------------------------------------------------*/ 
  
#ifndef __SPIDEVICES__ 
#define __SPIDEVICES__ 

#include <io.h>

#define BIT(x) (1<<x)

#define MCP2510			0              
#define SPI_NUM_DEVICES         1
#define SPI_DEVICE_NONE		SPI_NUM_DEVICES 

/* MCP2510 */
#define ENABLE_MCP2510()        (P5OUT &= ~BIT(0))
#define DISABLE_MCP2510()       (P5OUT |= BIT(0))

#endif
