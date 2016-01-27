/* 
 * File:   Main.c
 * Author: Aiden Nielsen
 *
 * Created on January 23, 2016, 9:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <Generic.h>
#include <uart.h>
#include <p24HJ12GP201.h>
#include <peripheralversion.h>
#include <timer.h>
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

int FLAGS;
int AmbLight;
void Timer1Setup(unsigned int pr1);
void SetupClock(void);
int SetAmbient(void);

int main(int argc, char** argv) {
    volatile int Foo;
    int LightLevel;
    SetupClock();
    Timer1Setup(5020);
    SetupADC();
    TRISAbits.TRISA4 = 0;
    SetAmbient();
    Foo = 12;
    
    //SetupUART1();

    //printf("Hello world\r\n");
    
    //TRISAbits.TRISA4 = 0;
    
   // redLED = on;
    
    
    
    while(1)
    {      
         redLED = on;
        
        
        PauseBasic(500);
        redLED = off;
        AD1CON1bits.SAMP = 1;	//START a2d processes
        
        while(AD1CON1bits.DONE != 1);	//START a2d processes
        
            LightLevel = ADC1BUF0;	//Grab data
        
        if (LightLevel > AmbLight )
        {
            PauseBasic(250);
        }
        PauseBasic(500);
        
    }
    while(1);
    return (EXIT_SUCCESS);
}

int SetAmbient(void)
{
    volatile int ReadLight_H;
    volatile int ReadLight_L;
    
    
    AD1CON1bits.SAMP = 1;	//START a2d processes
    
    while(AD1CON1bits.DONE != 1);	//START a2d processes
        volatile int ReadLight = ADC1BUF0;	//Grab data
        
    ReadLight_H = ReadLight;
    ReadLight_L = ReadLight;
    int i;
    for( i=0 ; i < 200 ; i++)   
    {
        AD1CON1bits.SAMP = 1;	//START a2d processes
        while(AD1CON1bits.DONE != 1);	//START a2d processes
        int ReadLight = ADC1BUF0;	//Grab data
        
        if(ReadLight < ReadLight_L)
        {
            ReadLight_L = ReadLight;
        }
        
        if(ReadLight > ReadLight_H)
        {
            ReadLight_H = ReadLight;
        }
        
    }
    
    AmbLight = (ReadLight_H+ReadLight_L)/2;
    
    int o;
    for( o=0; o < 2; o++ )
    {
    redLED = on;
    PauseBasic(500);
    redLED = off;
    PauseBasic(250);
    }
    PauseBasic(3000);
    
    return AmbLight;
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
void PauseBasic(unsigned int Delay)
{
    /* Delay for x milliseconds */
    FLAGS = 0;
    while(FLAGS < Delay);
    return;
}
void Pause1()
{
    /* Delay for x milliseconds */
    FLAGS = 0;
    while(FLAGS < 1);
    return;
}
int ReadLightLevel()
{
    /* Look for Light Level */
    return(0);
}
void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void)
 { 
    IFS0bits.T1IF = 0; /* clear interrupt flag */
    FLAGS++;
    return;
 }
inline void Timer1Setup(unsigned int pr1)
{
    OpenTimer1(
            T1_ON &
            T1_IDLE_CON &
            T1_GATE_OFF &
            T1_PS_1_8 &
            T1_SYNC_EXT_OFF &
            T1_SOURCE_INT,
            pr1);
    
   
    ConfigIntTimer1(
            T1_INT_PRIOR_1 &
            T1_INT_ON
                    );
    
    _T1IF = 0;  //Clear Interrupt Flag

    //printf("Timer 1 setup\r\n");
}