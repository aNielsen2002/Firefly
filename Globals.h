/* 
 * File:   Globals.h
 * Author: Shana Nielsen
 *
 * Created on January 23, 2016, 9:22 PM
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define __FOSC_                 40000000
#define __FTCY_                 (__FOSC_/2)
#define __U1BAUD_               115200
#define __BAUDUART1_            ((__FTCY_/(16*__U1BAUD_))-1)
    
extern void SetupUART1(void);
extern void PauseBasic(void);
extern void Pause1(void);
extern int ReadLightLevel(void);
#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

