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
#define LED LATAbits.LATA4
#define OFF 0
#define ON 1
#define FLASH_POWER 8000      // power level at which the firefly flashes
#define POWER_BOOST 400       // amount of power to add, for every other flash
#define FLASH_DELAY 200       // how long lasts the flash
#define DAYLIGHT 240          // values higher than that are recognized as daylight
#define DAYLIGHT_DELAY 10000  // wait 10 seconds, if daylight detected
#define BLIND_AFTER_OTHER 800 // how long are we blind after another flash
#define BLIND_AFTER_SELF 100  // how long are we blind after our own flash
#define THRESHOLD_DELTA 20    // added to the ambient light value
static volatile unsigned int act_light = 0; // value for lightness
int FLAGS;
int i = 0; // used for loops
int light = 0; // current lightness 
int threshold = 0; // threshold to determine a flash
int power = 0; // current power level
int blind = 0; // stores for how many cycles we are blind
int flash_power = FLASH_POWER; // if power is > flash_power, then flash

//Boot Loader OPTIONS
_FBS(BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF)
//Code protect options
_FGS(GSS_OFF & GCP_OFF & GWRP_OFF)
// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);
//Gives time for the power to settle 
_FPOR(FPWRT_PWR128 & ALTI2C_OFF);
//Watchdog is off
_FWDT(FWDTEN_OFF);

void Timer1Setup(unsigned int pr1);
void SetupClock(void);

int main(int argc, char** argv) {


    SetupClock();
    Timer1Setup(5020);
    SetupADC();
    TRISAbits.TRISA4 = 0;

    
    
    // compute threshold of the ambient light
    for (i = 0; i < 4; i++) {
        AD1CON1bits.SAMP = 1; //START a2d processes
        while (AD1CON1bits.DONE != 1); //START a2d processes
        act_light = ADC1BUF0; //Grab data
        threshold += act_light;
        PauseBasic(500);
    }
    
    threshold = threshold >> 2; // divides by 4
    threshold += THRESHOLD_DELTA; // move the threshold above the ambient average
    
    // intro, blink red 5 times
    for (i = 0; i < 5; i++) {
        LED = ON;
        PauseBasic(100);
        LED = OFF;
        PauseBasic(100);
    }

    // try to sleep some randomized time
    i = (act_light & 0x03); // use the last (right most) 2 bits of the actual lightness
    while (i--) {
        PauseBasic(1000);
    }
    
    





    // enter the main loop
    while (1) 
    {

        PauseBasic(500); // every cycle takes at least 0.5 ms

        if (power > 6000)
        { // increase the power level with a, first fast ascending,
            power += 100; // later slower ascending, function
        } 
        else if (power > 4000) 
        {
            power += 200;
        } 
        else if (power > 3000) 
        {
            power += 375;
        } 
        else if (power > 2000) 
        {
            power += 750;
            
        } 
        else 
        {
            power += 1500;
            
        }
        // Debug Function
//        if (power > 6000){
//            LED = ON; // Flash the LED
//            PauseBasic(1000); // wait
//            LED = OFF;// Turn the LED off
//        }
        
        AD1CON1bits.SAMP = 1; //START a2d processes
        while (AD1CON1bits.DONE != 1); //START a2d processes
            act_light = ADC1BUF0; //Grab data
        
        light = act_light; // read the actual lightness
        
        if (!blind) 
        {
            if (light > threshold)
            { // was it a flash?
                power += POWER_BOOST; // boost the power
                blind = BLIND_AFTER_OTHER; // and we are blind for the next cycles
            }
        } 
        else if (blind > 0) 
        { // if we are blind, then do nothing
            blind--;
        }

        if (power > flash_power) 
        { // if there is enough power, then we flash
            LED = ON; // Flash the LED
            PauseBasic(FLASH_DELAY); // wait
            LED = OFF;// Turn the LED off
            power = 0; // reset power
            blind = BLIND_AFTER_SELF; // blind after our own flash for some cycles
        }

        
    
    }
    
    return (EXIT_SUCCESS);
}

inline void SetupClock(void) { // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD = 41; // M = 43
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    CLKDIVbits.PLLPRE = 0; // N1 = 2
    // Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001) {
    }//
    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1) {
    }//
} //Check

void PauseBasic(unsigned int Delay) {
    /* Delay for x milliseconds */
    FLAGS = 0;
    while (FLAGS < Delay);
    return;
}

void Pause1() {
    /* Delay for x milliseconds */
    FLAGS = 0;
    while (FLAGS < 1);
    return;
}

int ReadLightLevel() {
    /* Look for Light Level */
    return (0);
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; /* clear interrupt flag */
    FLAGS++;
    return;
}

inline void Timer1Setup(unsigned int pr1) {
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

    _T1IF = 0; //Clear Interrupt Flag

    //printf("Timer 1 setup\r\n");
}