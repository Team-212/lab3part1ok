//Author: Luke Smeilus and Brad Nees 
//Team 212 


#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <string.h>
#include "timerP2.h"
#include "config.h"
#include "switch.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"
#include "interruptP1_1.h"

#define pressed 0
#define notPressed 1
#define Switch PORTDbits.RD6

typedef enum stateTypeEnum {
    idle1, idle1pressed, idle2, idle2pressed, forward, forwardPressed, backwardPressed, backward, idle
} stateType;

volatile stateType state = idle1; //initial state 
volatile float analogVal2 = 0;

void getAnalogVoltage() {
   
    float analogVol = 0.00;
    char analogLCD[5];
   
    analogVal2 = (float) ADC1BUF0;
   analogVol = 3.3 * (analogVal2 / 1023.0); //converts analogVal to analog voltage value between 0 and 3.3 volts 

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
    initSwitch1();
    
    
    while (1) {
        
        if (IFS0bits.AD1IF == 1) {

            IFS0bits.AD1IF = 0;
            getAnalogVoltage();
        }

        switch (state) {

            case idle1:
                
                    getAnalogVoltage(); //displays analog voltage
                    OC2RS = 0;
                    OC4RS = 0;
                    OC1RS = 0;
                    OC3RS = 0;

                break;
                
            case idle1pressed:
                
                delayUs(500);
                state = forward;
                break;

            case forward:
            
                    
              if (analogVal2 >= 409 && analogVal2 <= 609) {
                    OC2RS = 1023;
                    OC4RS = 1023 - analogVal2;
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
            
                break;

            case forwardPressed:
               
                delayUs(500); //debounce
                state = idle2;
                break;

            case idle2:
               
                getAnalogVoltage();
                OC2RS = 0;
                OC4RS = 0;
                OC1RS = 0;
                OC3RS = 0;
              

                break;

            case idle2pressed:
              
                delayUs(500); //debounce 
                state = backward;
                break;

            case backward:
                if (IFS0bits.AD1IF == 1) {

                    IFS0bits.AD1IF = 0;
                    getAnalogVoltage();
                }
               
                if (analogVal2 >= 409 && analogVal2 <= 609) {

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
           

                break;

            case backwardPressed:
               
                delayUs(500); //debounce 
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

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNINterrupt(){
    
    
    IFS1bits.CNDIF = 0; //Puts down flag 
    int i;
    i = PORTD;
    
    
       
    if(Switch == pressed){
        
        if(state == idle1){
            
            state = idle1pressed;
        }
        
        else if(state == forward){
            
            state = forwardPressed;
        }
        
        else if(state == idle2){
            
            state = idle2pressed;
            
          }
        
        else if(state == backward){
            
            state = backwardPressed;
        }
            
            
        }
    
}