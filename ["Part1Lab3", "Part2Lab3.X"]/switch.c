/* 
 * File:   switch.c
 * Author: gvanhoy
 *
 * Created on August 27, 2015, 3:12 PM
 */

#include <xc.h>

void initSwitch1(){
    
    TRISDbits.TRISD6 = 1;
    CNPUDbits.CNPUD6 = 1;
    CNCONDbits.ON = 1;    //Turn of CN for PORT D
    CNENDbits.CNIED6 = 1; //Enables CN for RD6
    
    IFS1bits.CNDIF = 0; // lowers flag 
    IEC1bits.CNDIE = 1; //enables inturrupt calls ISR 
    IPC8bits.CNIP = 7; //priority
    
}
