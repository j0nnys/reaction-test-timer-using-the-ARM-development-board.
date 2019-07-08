/*
 * CE865 Assignment 1 Skeleton
 */

#include <config/AT91SAM7S256.h>   // Hardware description header
#include <board/SevenSegmentDisplay.h> /* Provide access to 7 segment display functions */
#include <board/Button-lib.h> /* Provide access to buttons */
#include <board/LED-lib.h> /* Provide access to LEDs */

#include <atmel/pit.h>
#include <atmel/aic.h>

#define SECONDS				DISPLAY4
#define TENSOFSECONDS		DISPLAY3
#define HUNDREDOFSECONDS	DISPLAY2
#define THOUSANDOFSECONDS	DISPLAY1


#define PIT_PERIOD          20000 // interrupt at 20 milliseconds (50Hz)

/*
 * The array Digit contains the binary patterns to display the digits 0 - 9 to the Seven Segment LED Display.
 * Digit[0] is the pattern for zero, Digit[1] is the pattern for one, etc.
 *
 * See the SevenSegmentDisplay library manual entry for more details.
 */

short Digit[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67 };

/*
 * Global variables.
 */
short seconds, tensofseconds, hundredofseconds, thousandofseconds, hertz;

void ISR_System(void)
{
	/*
	 * Read the PIT status register to clear the pending interrupt.
	 * We are not interested in the value so the result does not have to be assigned to anything.
	 */
    PIT_GetPIVR();

    /*
     * Now Process the interrupt.
     *
     * 50 interrupts equals one second
     */
    if (++hertz == 50)
    {
    	hertz = 0;

		if (++seconds == 10)
			seconds = 0;
    }

    if ((hertz % 5) == 0)
    {
    	/*
    	 * The bars move at a rate 10 times faster than seconds digits increments.
    	 */
    	if (++tensofseconds == 10)
    		tensofseconds = 0;
    }
    if ((hertz % 5) == 0){

    	if (++hundredofseconds == 100)
    		hundredofseconds = 0;
    }
    if ((hertz % 5) == 0){

        if (++thousandofseconds == 1000)
        		thousandofseconds = 0;
        }
}

int main()
{
	/*
	 * First we need to configure the additional functionality required and initialise the OLED display.
	 */
	Configure7SegmentDisplay();

	/*
	 * Reset all the values to zero
	 */

	hertz = 0;
	seconds = 0;
	tensofseconds = 0;
	hundredofseconds = 0;
	thousandofseconds = 0;

	/*
	 * Reset the Seven Segment Display.
	 */

	Set7SegmentDisplayValue(SECONDS, Digit[0]); // Displays the digit 0
	Set7SegmentDisplayValue(TENSOFSECONDS, Digit[0]);  // Sets the display to blank
	Set7SegmentDisplayValue(HUNDREDOFSECONDS, Digit[0]);
	Set7SegmentDisplayValue(THOUSANDOFSECONDS, Digit[0]);

	/*
	 * Initialise and enable the PIT
	 * First argument (PIT_PERIOD) is number of milliseconds between timer interrupts (maximum 20 bit number)
	 * Second argument is clock rate in MHz (BOARD_MCK is in Hz so divide by 1000000)
	 */

	PIT_Init(PIT_PERIOD, BOARD_MCK / 1000000);

	/*
	 * Disable the interrupt on the interrupt controller
	 */

	AIC_DisableIT(AT91C_ID_SYS);

	/*
	 * Configure the AIC for PIT interrupts
	 * The PIT is a System interrupt, so we need to associate the ID of the System interrupt with the interrupt
	 * routine ISR_System.
	 */

	AIC_ConfigureIT(AT91C_ID_SYS, 0, ISR_System);

	/*
	 * Enable the interrupt on the interrupt controller
	 */

	AIC_EnableIT(AT91C_ID_SYS);

	/*
	 * Enable the PIT interrupt and start the PIT
	 *
	 * The interrupt handler is always assigned before the interrupt is enabled.
	 * Using the library function the PIT interrupt is enabled separately from the PIT itself. the interrupt is enabled first so none are missed when the timer is enabled.
	 */
	PIT_EnableIT();
	PIT_Enable();

	/*
	 * Now start the display update running.
	 */

	while (1)
	{
		Set7SegmentDisplayValue(DISPLAY1, Digit[thousandofseconds]);
		Set7SegmentDisplayValue(DISPLAY2, Digit[hundredofseconds]);
		Set7SegmentDisplayValue(DISPLAY3, Digit[tensofseconds]);
		Set7SegmentDisplayValue(DISPLAY4, Digit[seconds]);
	}
	return 0;
}
