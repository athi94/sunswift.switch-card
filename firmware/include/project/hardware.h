/* Hardware definitions */
#define BIT(x) (1<<x)

#define CLOCK_SPEED 7372800

/* Port 1 */
#define CAN_INT         BIT(4)

/* Port 2 */
#define SW(a)			BIT(a)
#define SW1             BIT(0)
#define SW2             BIT(1)
#define SW3             BIT(2)
#define SW4             BIT(3)

/* Port 3 */
#define TX              BIT(4) 
#define RX              BIT(5)

/* Port 4 */

/* Port 5 */
#define CAN_CS          BIT(0) /* Note: re-defined in scandal_devices.h */
#define SIMO1           BIT(1)
#define SOMI1           BIT(2)
#define UCLK1           BIT(3)
#define YELLOWLED       BIT(6)
#define REDLED          BIT(7)

/* Port 6 / ADC */
#define MEAS_VIN_PIN	(BIT(MEAS_VIN)) /* supply voltage, 12V */
#define MEAS_VOUT1_PIN	(BIT(MEAS_VOUT1))
#define MEAS_VOUT2_PIN	(BIT(MEAS_VOUT2))
#define MEAS_VOUT3_PIN	(BIT(MEAS_VOUT3))
#define MEAS_VOUT4_PIN	(BIT(MEAS_VOUT4))



/* ADC channel definitions */

#define MEAS_VIN        7
#define MEAS_VOUT1		0
#define MEAS_VOUT2		1
#define MEAS_VOUT3		2
#define MEAS_VOUT4		3

#define MEAS_TEMP       8
#define MEAS_3V3        9

