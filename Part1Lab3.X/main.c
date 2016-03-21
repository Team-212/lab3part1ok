#include <xc.h>
#include <sys/attribs.h>
#include "timerP2.h"
#include "config.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"

volatile unsigned int analogVal = 0;

void getAnalogVoltage (unsigned int val)
{
    //unsigned char analogVoltage[5];
    float analogVol = 0.00;
    char analogLCD[5];
    
    analogVol = (val/(1023.0) * 3.3);
    
    sprintf(analogLCD, "%.2f", analogVol);
    printStringLCD(analogLCD);
    moveCursorLCD(1,1);
    
}

int main (void) {
    SYSTEMConfigPerformance(10000000);
    initLCD();
    initADC();
    initPWM();
    
    int i;
    while(1) {
        if (IFS0bits.AD1IF == 1)
        {
            analogVal = ADC1BUF0;
            IFS0bits.AD1IF = 0;
            getAnalogVoltage(analogVal);
            OC2RS = analogVal;
            OC1RS = 1023;
        }
               
    }
}
