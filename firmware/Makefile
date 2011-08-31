# Makefile for Sunswift MINING-2 board

.PHONY: clean

MAIN_NAME=mspswitch
ARCH=msp430
MCU=msp430x149
BUILD_DIR=.
OUTPUT_DIR=../outputs

TOP=../../..
SCANDAL=$(TOP)/scandal
SCANDAL_SRC=$(TOP)/scandal/src
ARCH=$(TOP)/scandal/src/arch/msp430f149

AS=msp430-as
CC=msp430-gcc
OBJCOPY=msp430-objcopy
OBJDUMP=msp430-objdump

JTAG=msp430-jtag
#JTAG=jtag.py

CFLAGS=-g -O1 -Wall -mmcu=$(MCU) -I$(SCANDAL)/include -I($ARCH)/include
LDFLAGS=-mmcu=$(MCU) 
ASFLAGS=-mmcu=$(MCU) 

all: $(MAIN_NAME).hex $(MAIN_NAME).lst
	cp $(MAIN_NAME).hex $(OUTPUT_DIR)/$(MAIN_NAME).hex

## Copy the .hex and .elf file
 
$(MAIN_NAME).hex: $(MAIN_NAME).elf
	$(OBJCOPY) -O ihex $< $@

$(MAIN_NAME).lst: $(MAIN_NAME).elf
	$(OBJDUMP) -DS $(MAIN_NAME).elf > ${MAIN_NAME}.lst	

## Link the .o files into the .out (.elf) file. 

$(MAIN_NAME).elf :  $(BUILD_DIR)/$(MAIN_NAME).o $(BUILD_DIR)/spi_driver.o $(BUILD_DIR)/led.o $(BUILD_DIR)/scandal_timer.o $(BUILD_DIR)/scandal_obligations.o $(BUILD_DIR)/scandal_eeprom.o $(BUILD_DIR)/mcp2510.o $(BUILD_DIR)/scandal_error.o $(BUILD_DIR)/scandal_engine.o  $(BUILD_DIR)/scandal_utils.o $(BUILD_DIR)/scandal_message.o $(BUILD_DIR)/adc.o
	$(CC) $^ $(LDFLAGS) -o $(MAIN_NAME).elf

## How to build .o, .s and .o files from .c, .c and .s files respectively. 

%.o : %.c 
	$(CC) -c $(CFLAGS) $< -o $@

%.s : %.c
	$(CC) -S $(CFLAGS) $< -o $@

%.o : %.s
	$(AS) -c $(ASFLAGS) $< -o $@

## How to build individual

$(BUILD_DIR)/eeprom.o : $(SCANDAL_SRC)/eeprom.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/mcp2510.o : $(ARCH)/mcp2510.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/error.o : $(SCANDAL_SRC)/error.c
	$(CC) $(CFLAGS) -c -o $@ $<	

$(BUILD_DIR)/engine.o : $(SCANDAL_SRC)/engine.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/utils.o : $(SCANDAL_SRC)/utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/message.o : $(SCANDAL_SRC)/message.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.out *.map *.hex
	rm -f *~
	rm -f $(BUILD_DIR)/*.o
	rm -f $(MAIN_NAME).lst $(MAIN_NAME).elf
	rm -f *.elf
	rm -f *.lst

program: $(MAIN_NAME).hex
	$(JTAG) -eEpvrw $(MAIN_NAME).hex
