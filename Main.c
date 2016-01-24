/* 
 * File:   Main.c
 * Author: Shana Nielsen
 *
 * Created on January 23, 2016, 9:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <Generic.h>
#include <uart.h>
#include <p24HJ12GP201.h>
#include <peripheralversion.h>
#include "Globals.h"

// Definitions
#define redLED LATAbits.LATA4
#define off 0
#define on 1

 	//Boot Loader OPTIONS
 	_FBS( BSS_NO_BOOT_CODE  & BWRP_WRPROTECT_OFF )
	//Code protect options
	_FGS( GSS_OFF   & GCP_OFF & GWRP_OFF )
	// Select Internal FRC at POR
	_FOSCSEL(FNOSC_FRC & IESO_OFF);
	// Enable Clock Switching and Configure
	_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF   );
	//Gives time for the power to settle 
	 _FPOR(FPWRT_PWR128 & ALTI2C_OFF);
	//Watchdog is off
	_FWDT( FWDTEN_OFF);

void SetupClock(void);
int main(int argc, char** argv) {
    volatile int Foo;
    
    SetupClock();
    
    Foo = 12;
    
    SetupUART1();

    //printf("Hello world\r\n");
    
    TRISAbits.TRISA4 = 0;
    
    redLED = on;
    
    while(1);
    return (EXIT_SUCCESS);
}

inline void SetupClock(void)
{	// Configure PLL prescaler, PLL postscaler, PLL divisor
	PLLFBD =41;				// M = 43
	CLKDIVbits.PLLPOST = 0; 	// N2 = 2
	CLKDIVbits.PLLPRE = 0;		// N1 = 2
	// Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
	__builtin_write_OSCCONH(0x01);
	__builtin_write_OSCCONL(0x01);
	// Wait for Clock switch to occur
	while (OSCCONbits.COSC != 0b001){}//
	// Wait for PLL to lock
	while(OSCCONbits.LOCK != 1) {}//
}			//Check
