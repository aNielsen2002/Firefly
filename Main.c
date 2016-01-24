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

#include "Globals.h"
/*
 */

int main(int argc, char** argv) {
    volatile int Foo;
    Foo = 12;
    printf("Hello world\r\n");
    SetupUART();
    return (EXIT_SUCCESS);
}

void SetupUART()
{
    
    return;
}

