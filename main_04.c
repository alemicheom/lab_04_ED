/*
 * File:   main_04.c
 * Author: aleja
 *
 * Created on August 11, 2022, 8:55 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "i2c_04.h"
#include "tmr0_01.h"
#include "oscilador_01.h"
#include "LCD_02.h"

//Definición de variables
#define _XTAL_FREQ 4000000


// POT A LCD
uint8_t unit_POT = 0;
uint8_t dec_POT = 0;
uint8_t valor_POT = 0;
char s[];

// variables modulo de reloj 
uint8_t second = 0;  
uint8_t minute= 0; 
uint8_t hour = 0;                             
                          
                        

//Definicion de funciones
void setup (void);

unsigned short POT = 0; 
unsigned short map(uint8_t val, uint8_t in_min, uint8_t in_max, //Función del mapeo
            unsigned short out_min, unsigned short out_max);




void main(void){
    setup();
    Lcd_Init(); // start
    Lcd_Clear(); //limpiar LCD
    
    while (1){
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(PORTB);
        I2C_Master_Stop();
        __delay_ms(20);
       
        I2C_Master_Start();
        I2C_Master_Write(0x51);             // address slave 
        valor_POT = I2C_Master_Read(0);     // valor del POT
        I2C_Master_Stop(); //STOP
        
        
            
        __delay_ms(20);
        
        POT = map(valor_POT, 0, 255, 0, 500);       //map voltaje de 0 a 500
        unit_POT = POT/100;                         //entero
        dec_POT = POT-unit_POT*100;                //decimal
            
        //Mostramos en LCD
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(" POT ");
        Lcd_Set_Cursor(2,1);
        sprintf(s, "%d.%d%dV ", unit_POT, dec_POT/10, dec_POT%10); 
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(s);
      
    }
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    TRISB = 0b00001111; 
    PORTB = 0; 
    
    TRISE = 0;
    PORTE = 0; 
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    int_osc_MHz(2);
    I2C_Master_Init(100000);
    tmr0_init(256);  
}

unsigned short map(uint8_t x, uint8_t x0, uint8_t x1,
            unsigned short y0, unsigned short y1){
    return (unsigned short)(y0+((float)(y1-y0)/(x1-x0))*(x-x0));}