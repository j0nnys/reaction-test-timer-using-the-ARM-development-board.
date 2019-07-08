# reaction-test-timer-using-the-ARM-development-board.
The reaction timer displays the time it takes for the user to react to an LED being illuminated. It displays the time in seconds to an accuracy of one thousand of a second.

1. On start-up or after a reaction test has completed, LED 1 should flash to indicate Button 1 is to be pressed to start the next reaction test.
2. When Button 1 is pressed, the display is zeroed; all the LEDs are turned RED and begin to count down (turn off) at the rate of one per second.
3. When the last LED turns off a random LED is lit (turned GREEN) indicating the button that should be pressed to stop the count. At the same time the display begins counting, showing the elapsed time.
4. When the indicated button is pressed the display stops counting and displays the time required for the user to react. The timer is ready for the next reaction test (returns to step 1).
If the user presses any button other than the button indicated by the LED the count should continue. If the user fails to react, the count stops at 9.999 seconds.
If the user presses one or more buttons before the countdown finishes, the countdown is suspended until all the buttons are released.
