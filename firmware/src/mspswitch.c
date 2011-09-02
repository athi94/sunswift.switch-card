#include <io.h>
#include <signal.h>
#include <iomacros.h>
#include <string.h>

#include <scandal/timer.h>
#include <scandal/led.h>
#include <scandal/can.h>
#include <scandal/engine.h>
#include <scandal/spi.h>
#include <scandal/devices.h>
#include <scandal/utils.h>
#include <scandal/message.h>
#include <scandal/adc.h>

#include <project/adc.h>
#include <project/spi_devices.h>
#include <project/hardware.h>

#define WDTCTL_INIT     WDTPW|WDTHOLD

void init_ports(void){
  P1OUT = 0x00;
  P1SEL = 0x00;
  P1DIR = 0x00;
  P1IES = CAN_INT;
  P1IE  = 0x00; 

  P2OUT = 0x00; //SW1 | SW2 | SW3 | SW4;
  P2IES = 0x00;
  P2SEL = 0x00;
  P2DIR = SW1 | SW2 | SW3 | SW4;
  P2IES = 0x00;
  P2IE  = 0x00;

  P3OUT = 0x00;
  P3SEL = 0x00;
  P3DIR = 0x00;

  P4OUT = 0x00;
  P4SEL = 0x00;
  P4DIR = 0x00;

  P5OUT = CAN_CS;
  P5SEL = SIMO1 | SOMI1 | UCLK1;
  P5DIR = CAN_CS | SIMO1 | UCLK1 | YELLOWLED | REDLED;

  P6SEL = MEAS_VIN_PIN | MEAS_VOUT1_PIN | MEAS_VOUT2_PIN | MEAS_VOUT3_PIN | MEAS_VOUT4_PIN;  
}

void init_clock(void){
  volatile unsigned int i;

  /* XTAL = LF crystal, ACLK = LFXT1/1, DCO Rset = 4, XT2 = ON */
  BCSCTL1 = 0x04;
  
  /* Clear OSCOFF flag - start oscillator */
  _BIC_SR( OSCOFF );
  do{
    /* Clear OSCFault flag */
    IFG1 &= ~OFIFG; 
    /* Wait for flag to set */
    for( i = 255; i > 0; i-- )
      ;
  } while(( IFG1 & OFIFG ) != 0);
  
  /* Set MCLK to XT2CLK and SMCLK to XT2CLK */
  BCSCTL2 = 0x88; 
}

/*--------------------------------------------------
  Interrupt handing for CAN stuff 
  --------------------------------------------------*/
void enable_can_interrupt(){
  P1IE = CAN_INT;
}

void disable_can_interrupt(){
  P1IE = 0x00;
}

interrupt (PORT1_VECTOR) port1int(void) {
  toggle_red_led();
  can_interrupt();
  P1IFG = 0x00;
}


/* Main function */
int main(void) {
	int i;
	u32 channel_updated[4] = {0,0,0,0};
	sc_time_t my_timer;

	P5OUT = CAN_CS;
	dint();
	WDTCTL = WDTCTL_INIT;               //Init watchdog timer
	init_ports();
	init_clock();
	sc_init_timer();
	scandal_init(); 
	init_adc();
	eint();
  
	for(i=MSPSWITCH_CHANNEL_1; i<=MSPSWITCH_CHANNEL_4; i++)
		channel_updated[i] = scandal_get_in_channel_rcvd_time(i);

	my_timer = sc_get_timer();

	while (1) {
		handle_scandal(); 

		for(i=MSPSWITCH_CHANNEL_1; i<=MSPSWITCH_CHANNEL_4; i++) {
			if(scandal_get_in_channel_rcvd_time(i) > channel_updated[i]) {
				if(scandal_get_in_channel_value(i) != 0)
					P2OUT |= SW(i);
				else
					P2OUT &= ~SW(i);

				channel_updated[i] = scandal_get_in_channel_rcvd_time(i);
			}
		}

		if(sc_get_timer() >= my_timer + 3000){
			my_timer = sc_get_timer();
			toggle_yellow_led();

			scandal_send_scaled_channel(TELEM_LOW, MSPSWITCH_IN_VOLTAGE, sample_adc(MEAS_VIN));
			scandal_send_scaled_channel(TELEM_LOW, MSPSWITCH_VOLTAGE_1, sample_adc(MEAS_VOUT1));
			
			scandal_send_scaled_channel(TELEM_LOW, MSPSWITCH_VOLTAGE_2, sample_adc(MEAS_VOUT2));
			scandal_send_scaled_channel(TELEM_LOW, MSPSWITCH_VOLTAGE_3, sample_adc(MEAS_VOUT3));
			scandal_send_scaled_channel(TELEM_LOW, MSPSWITCH_VOLTAGE_4, sample_adc(MEAS_VOUT4));
		
			scandal_send_channel(TELEM_LOW, MSPSWITCH_CHANNEL_1_STATUS, (scandal_get_in_channel_value(MSPSWITCH_CHANNEL_1) != 0));
			
			scandal_send_channel(TELEM_LOW, MSPSWITCH_CHANNEL_2_STATUS, (scandal_get_in_channel_value(MSPSWITCH_CHANNEL_2) != 0));
			scandal_send_channel(TELEM_LOW, MSPSWITCH_CHANNEL_3_STATUS, (scandal_get_in_channel_value(MSPSWITCH_CHANNEL_3) != 0));
			scandal_send_channel(TELEM_LOW, MSPSWITCH_CHANNEL_4_STATUS, (scandal_get_in_channel_value(MSPSWITCH_CHANNEL_4) != 0));
			
		}
	}
}

