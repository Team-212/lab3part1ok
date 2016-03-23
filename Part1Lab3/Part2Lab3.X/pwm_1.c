#include <xc.h>
#include "pwm.h"


void initPWM()
{
    RPD5Rbits.RPD5R = 0b1100; // map oc1 to rb10 j10 pin13
    RPD1Rbits.RPD1R = 0b1011; // map oc2 to rd1 j11 pin20
    RPF1Rbits.RPF1R = 0b1011; // map oc3 to rf1 j11 pin8
    RPD3Rbits.RPD3R = 0b1011; // map oc4 to rd3 j11 pin18
    
    OC1CON = 0x0000; // Turn off oc1 while doing setup.
    OC1R = 0x0000; // Initialize primary Compare Register
    OC1RS = 0x0000; // Initialize secondary Compare Register
    OC1CON = 0x0006;// Configure for PWM mode
    //OC5CONbits.OCTSEL = 1; // Oscillator
    OC1CONbits.ON = 1; // Enable oc1
    
    OC2CON = 0x0000; 
    OC2R = 0x0000; 
    OC2RS = 0x0000; 
    OC2CON = 0x0006; 
    //OC2CONbits.OCTSEL = 1;  
    OC2CONbits.ON = 1;  // Enable oc2
    
    OC3CON = 0x0000; 
    OC3R = 0x0000; 
    OC3RS = 0x0000; 
    OC3CON = 0x0006;  
    //OC3CONbits.OCTSEL = 1; 
    OC3CONbits.ON = 1; // Enable oc3
    
    OC4CON = 0x0000; 
    OC4R = 0x0000; 
    OC4RS = 0x0000; 
    OC4CON = 0x0006;
    //OC4CONbits.OCTSEL = 1; 
    OC4CONbits.ON = 1; // Enable oc4
}
