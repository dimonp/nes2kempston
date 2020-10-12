/*
 * File:   newmain.c
 * Author: Dimon
 *
 * Created on 24 августа 2020 г., 18:26
 */

#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA3/MCLR pin function select (RA3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#define _XTAL_FREQ 4000000

#include <stdint.h>
#include <xc.h>

void main(void) {
    INTCON  = 0;        // Disable interrupts
    CMCON   = 0x07;     // Disable comparator
    
    TRISA = 0;          // All pins are output    
    TRISC = 0;

    PORTA = 0;   
    PORTC = 0;
    
    // RA3 - controller clock(output) 
    // RA1 - controller latch(output) 
    // RA0 - controller data(input)
    TRISA0 = 1;
       
    // initial clock state
    RA2 = 1;
    
    uint8_t buttons;
    uint8_t turboCounter = 0xFF;
    
    while(1) {
        buttons = 0;
        
        RA1 = 1;
        __delay_us(20);
        RA1 = 0;
        __delay_us(10);        
        
        for(int i = 7; i >= 0; i--) {
            RA2 = 0;
            __delay_us(10);
            
            buttons |= RA0 << i;
            
            RA2 = 1;
            __delay_us(10);
        }
        
        // if B pressed
        if (!((buttons >> 6) & 1)) {
            turboCounter++;
        } else {
            turboCounter = 0xFF;
        }
        
        // v1.1
        // down
        PORTCbits.RC0 = (buttons >> 2) & 1;
        // left
        PORTCbits.RC1 = (buttons >> 1) & 1;
        // right
        PORTCbits.RC2 = buttons & 1;      
        // up
        PORTCbits.RC3 = (buttons >> 3) & 1;
        // fire A
        PORTCbits.RC4 = ((buttons >> 7) & 1) & ((turboCounter >> 4) & 1);
        // fire B
        PORTCbits.RC5 = (buttons >> 6) & 1;
    }
    
    return;
}
