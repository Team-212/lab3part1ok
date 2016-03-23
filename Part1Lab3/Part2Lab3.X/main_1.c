#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <string.h>
#include "timerP2.h"
#include "config.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"
#include "interruptP1.h"

#define pressed 0
#define notPressed 1
#define Switch PORTDbits.RD6

typedef enum stateTypeEnum {
    idle1, idle1pressed, idle2, idle2pressed, forward, forwardPressed, backwardPressed, backward, idle
} stateType;

volatile stateType state = idle1;
volatile float analogVal2 = 0;

void getAnalogVoltage() {
    //unsigned char analogVoltage[5];
    float analogVol = 0.00;
    char analogLCD[5];
    //float analogVal2 = 0;
    analogVal2 = (float) ADC1BUF0;
    // printCharLCD('d');
    analogVol = 3.3 * (analogVal2 / 1023.0);

    sprintf(analogLCD, "%.2f", analogVol);
    printStringLCD(analogLCD);
    moveCursorLCD(1, 1);

}

int main(void) {
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initTMR();
    initLCD();
    initADC();
    initPWM();
    TRISDbits.TRISD6 = 1;
    //testLCD();
    //int i;
    while (1) {
        //printCharLCD('c');
        if (IFS0bits.AD1IF == 1) {

            IFS0bits.AD1IF = 0;
            getAnalogVoltage();
        }

        switch (state) {

            case idle1:
                while (Switch == notPressed) {
                    getAnalogVoltage();
                    OC2RS = 0;
                    OC4RS = 0;
                    OC1RS = 0;
                    OC3RS = 0;
                    if (Switch == pressed) state = idle1pressed;
                }


                break;
            case idle1pressed:
                while (Switch == pressed);
                delayUs(5000);
                state = forward;
                break;

            case forward:
                while (Switch == notPressed);
            {
                if (analogVal2 >= 409 && analogVal2 <= 609) {
                    OC2RS = 1023;
                    OC4RS = 1023;
                    OC1RS = 0;
                    OC3RS = 0;
                } else if (analogVal2 == 1023) {
                    OC4RS = 1023;
                    OC2RS = 0;
                    OC1RS = 0;
                    OC3RS = 0;
                } else if (analogVal2 == 0) {
                    OC2RS = 1023;
                    OC4RS = 0;
                    OC1RS = 0;
                    OC3RS = 0;
                } else if (analogVal2 > 609 && analogVal2 < 1023) {
                    OC4RS = 1023;
                    OC3RS = 1023 - analogVal2;
                    OC1RS = 0;
                    OC2RS = 0;
                } else if (analogVal2 < 409 && analogVal2 > 0) {
                    OC2RS = 1023;
                    OC4RS = analogVal2;
                    OC1RS = 0;
                    OC3RS = 0;
                }
            }
                if (Switch == pressed) state = forwardPressed;
                break;

            case forwardPressed:
                while (Switch == pressed);
                delayUs(500);
                state = idle2;
                break;

            case idle2:
                if (IFS0bits.AD1IF == 1) {

                    IFS0bits.AD1IF = 0;
                    getAnalogVoltage();
                }
                while (Switch == notPressed);
            {

                getAnalogVoltage();
                OC2RS = 0;
                OC4RS = 0;
                OC1RS = 0;
                OC3RS = 0;
                if (Switch == pressed) state = idle2pressed;
            }

                break;

            case idle2pressed:
                while (Switch == pressed);
                delayUs(5000);
                state = backward;
                break;

            case backward:
                if (IFS0bits.AD1IF == 1) {

                    IFS0bits.AD1IF = 0;
                    getAnalogVoltage();
                }
                while (Switch == notPressed);
            {
                if (analogVal2 >= 409 && analogVal2 <= 609) {
\
                    OC1RS = 1023;
                    OC3RS = 1023;
                    OC2RS = 0;
                    OC4RS = 0;
                } else if (analogVal2 == 1023) {

                    OC3RS = 1023;
                    OC1RS = 0;
                    OC2RS = 0;
                    OC4RS = 0;
                } else if (analogVal2 == 0) {

                    OC1RS = 1023;
                    OC3RS = 0;
                    OC2RS = 0;
                    OC4RS = 0;
                } else if (analogVal2 > 609) {

                    OC3RS = 1023;
                    OC1RS = 1023 - analogVal2;
                    OC2RS = 0;
                    OC4RS = 0;
                } else if (analogVal2 < 409) {

                    OC1RS = 1023;
                    OC3RS = analogVal2;
                    OC2RS = 0;
                    OC4RS = 0;
                }
            }
                if (Switch == pressed) state = backwardPressed;

                break;

            case backwardPressed:
                while (Switch == pressed);
                delayUs(500);
                state = idle1;
                break;

        }
    }
}

void __ISR(_ADC_VECTOR, IPL7SRS) _ADCInterrupt(void) {
    IFS0bits.AD1IF = 0;
    getAnalogVoltage();
    analogVal2 = (float) ADC1BUF0;
}