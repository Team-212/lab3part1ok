
#include <xc.h>
#include "adc.h"

void initADC(){
    
    ANSELBbits.ANSB0 = 0; // pin 34 j11
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0; 
    AD1CON2bits.CSCNA = 0; // Disable scanning
    AD1CON2bits.SMPI = 15;
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 2; 
    AD1CON3bits.ADCS = 0xFF; // 512 times the PBCLK
    AD1CHSbits.CH0NA = 0; 
    AD1CHSbits.CH0SA = 2; 
    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;
    IPC5bits.AD1IP = 7;
    AD1CON1bits.ADON = 1; // turn on the ADC
}
