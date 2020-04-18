/*
 * File:   main.c
 * Author: Hossam Eldeen Altokhy
 *
 * Created on April 4, 2020, 6:24 PM
 */
#define F_CPU 16000000UL

//Build in headers
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//User defined Headers

#include "Config.h"
#include "DIO.h"
#include "lcd.h"


#define portA           1
#define portB           2
#define portC           3
#define portD           4

int i = 0;

void INT0_init(int mode) {
    switch (mode) {
        case SenseMode_L:
            break;
        case SenseMode_T:
            MCUCR &= ~(1 << ISC01);
            MCUCR |= (1 << ISC00);
            break;
        case SenseMode_F:
            MCUCR &= ~((1 << ISC01) | (1 << ISC00));
            break;
        case SenseMode_R:
            MCUCR |= (1 << ISC01) | (1 << ISC00);
            break;
    }
    GICR |= (1 << INT0);
}

void INT1_init(int mode) {

    switch (mode) {
        case SenseMode_L:
            break;
        case SenseMode_T:
            MCUCR &= ~(1 << ISC11);
            MCUCR |= (1 << ISC10);
            break;
        case SenseMode_F:
            MCUCR &= ~((1 << ISC11) | (1 << ISC10));
            break;
        case SenseMode_R:
            MCUCR |= (1 << ISC11) | (1 << ISC10);
            break;
    }
    GICR |= (1 << INT1);
}

void INT2_init(int mode) {
    switch (mode) {
        case SenseMode_R:
            MCUCSR |= (1 << ISC2);
            break;
        case SenseMode_F:
            MCUCSR &= ~(1 << ISC2);
            break;
    }
    GICR |= (1 << INT2);
}

ISR(INT0_vect) {
    togglePINB(LED_Board);
}

ISR(INT1_vect) {

    togglePINC(LED1);

}

ISR(INT2_vect) {
    {
        togglePINC(LED0);
    }
}

int main(void) {
    PINBas(BUTTON0, IN);
    PINBas(BUTTON1, IN);
    PINCas(LED0, OUT);
    PINCas(LED1, OUT);
    PINBas(LED_Board, OUT);

    PINDas(Interrupt1, OUT);
    PINBas(Interrupt2, OUT);


    INT0_init(SenseMode_R);
    INT1_init(SenseMode_R);
    INT2_init(SenseMode_R);

    //Enable Global Interrupt
    sei();



    while (1) {
        if (isPressedB(BUTTON1)) {
            setPIND(Interrupt1);
        }
        if (isPressedB(BUTTON0)) {
            setPINB(Interrupt2);

        }
        if (isPressedD(BUTTON2)){
            setPIND(Interrupt0);
        }
        else {
            resetPIND(Interrupt1);
            resetPINB(Interrupt2);
            
        }
     _delay_ms(200);
    }
}

