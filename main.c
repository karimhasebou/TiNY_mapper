
// PIC32MX320F128H Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
// USERID = No Setting

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
// PRIPLL
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <plib.h>
#include <p32xxxx.h>
#include <limits.h>
#include "uart.h"
#include "delay.h"

#define FPB 80e6 // 80e6
#define TIMER_RATE 50
#define T1_PS 256. // 256.
#define TIMER_OVF ((float)FPB / (float)TIMER_RATE / (float)T1_PS)
#define DELAY 5e-6

#define SPEED 60
#define SPEED_FEED (SPEED / 200.)
#define STOP (1.5 / 20.)
#define BK 	((1.5 - SPEED_FEED) / 20.)
#define FWD_MULT ((1.5 + SPEED_FEED) / 20.)

#define	GetSystemClock()              (80000000ul)
#define	GetPeripheralClock()          (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define DESIRED_BAUDRATE              (115200)      //The desired BaudRate

#define delta 5

int mult1 = 0, mult2 = 0;

enum orientation{N,E,S,W};
enum move{FWD,BACK,RIGHT,LEFT};

enum move mv = FWD;
enum orientation heading = N;
int posx = 0, posy = 0;
int szx = 100, szy = 100;





void forward(void){
    OC1RS = PR2 * FWD_MULT * 3.8;
    OC2RS = PR2 * BK;
    mv = FWD;
}

void forwardX(int x){
    OC1RS = PR2 * FWD_MULT * 3.8;
    OC2RS = PR2 * BK;
    mv = FWD;
    switch(heading){
	case N: 
		posx++; 
		break;
	case E: 
		posy++; 
		break;
	case S: 
		posx--; 
		break;
	case W: 
		posy--; 
		break;
    }
    delay(10e3*x);
}

void backward(void){
    OC1RS = PR2 * BK;
    OC2RS = PR2 * FWD_MULT;
    mv = BACK;
}

void stop() {
    OC1RS = PR2 * STOP;
    OC2RS = PR2 * STOP;
}

void rotate(enum move dir) {
    if (dir == LEFT) {
        OC1RS = PR2 * FWD_MULT;
        OC2RS = PR2 * FWD_MULT;
    }
    else {
        OC1RS = PR2 * BK;
        OC2RS = PR2 * BK;
    }
}


void toggle(void) {
    if (mv == BACK)
        forward();
    else
        backward();
}


void __ISR(_TIMER_2_VECTOR, ipl1) T2ISR(void){
    mT2ClearIntFlag();
}

 void __ISR(_TIMER_1_VECTOR, ipl2) T1ISR(void){
    t_count++;
    mT1ClearIntFlag();
}

void notifyClientOfDistance(ul distance){
    char msg[12];
    
    int i;
    for(i = 0; i < 12;++i)
	    msg[i] = ' ';
    msg[11] = '\0';
    
	switch(heading){
	case N:
		msg[10] = 'N';
		break;
	case S:
		msg[10] = 'S';
		break;
	case W:
		msg[10] = 'W';
		break;
	case E:
		msg[10] = 'E';
		break;
    }
    
    int posyLocal = posy, posxLocal = posx,distLocal = distance;
    int idx = 0;
    
    do{
        msg[idx++] = ((int)'0') + posyLocal % 10;
        posyLocal /= 10;
    }while(posyLocal > 0);
    idx += 1;
    
    do{
        msg[idx++] = ((int)'0') + posxLocal % 10;
        posxLocal /= 10;
    }while(posxLocal > 0);
    
    idx += 1;
    do{
        msg[idx++] = ((int)'0') + distLocal % 10;
        distLocal /= 10;
    }while(distLocal > 0);
    
    
    sendMessage(msg);
}

void turnHead(enum move dir){
    if (dir == LEFT){ 
        OC4RS = PR2 * FWD_MULT;
    }else if(dir == RIGHT){
        OC4RS = PR2 * BK;
    }
    delay(100000. * 1.15);
    OC4RS = PR2 * STOP;
}


void turn90(enum move dir){
	rotate(dir);
	int t = 400000 / 2 / 37 * 45;
	delay(t);
	stop();
	switch(heading){
		case N: heading = dir == LEFT ? W:E; break;
		case E: heading = dir == LEFT ? N:S; break;
		case S: heading = dir == LEFT ? E:W; break;
		case W: heading = dir == LEFT ? S:N; break;
	}
}

void initDA(){
    OpenOC1( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenOC2( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    OpenOC4( OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);

    int ovf = TIMER_OVF;
    OpenTimer2(T2_ON | T2_PS_1_256 | T2_SOURCE_INT, ovf);
    
    PR2 = ovf - 1;
    stop();
    turnHead(FWD);
    mT2SetIntPriority(1);
    mT2IntEnable(1);
}


ul usToCM(ul us){
    return us / (29 * 2);
}

ul getDistance(){
    ul us_count, distance, time;
    mPORTDSetPinsDigitalOut(BIT_10);
    mPORTDClearBits(BIT_10);
    delay(1);
    mPORTDSetBits(BIT_10);
    delay(1);
    mPORTDClearBits(BIT_10);
    mPORTDSetPinsDigitalIn(BIT_10);
    while(!mPORTDReadBits(BIT_10));
    us_count = t_count;
    while(mPORTDReadBits(BIT_10));
    time = (t_count - us_count) * 5; // time in us
    distance = usToCM(time);
    delay(100000);
    return distance;
}

int withinBounds(){
	int val = 1;
	switch(heading){
	     case N: 
		     val = (posx + 1 < szx) ? 1:0; 
		     break;
	     case E: 
		     val = (posy + 1 < szy) ? 1:0; 
		     break;
	     case S: 
		     val = (posx - 1 >= 0) ? 1:0; 
		     break;
	     case W: 
		     val = (posy - 1 >= 0) ? 1:0; 
		     break;
	 }
	return val;
}

int main(){
    
    ul dist;
    initDelay();
    mT1SetIntPriority(2);
    INTEnableSystemMultiVectoredInt();    
    mT1IntEnable(1);
    initDA();
    initUart();
    while(1){
        dist = getDistance();
        if(dist < 30 || !withinBounds())
            turn90(LEFT);
        else{
             forwardX(50);
             stop();
             
	     dist = getDistance();
             notifyClientOfDistance(dist);
	     
             turnHead(LEFT);
             dist = getDistance();
             notifyClientOfDistance(dist);
             
             turnHead(RIGHT);
	     
             turnHead(RIGHT);
             dist = getDistance();
             notifyClientOfDistance(dist);
	     
             turnHead(LEFT);
        }
    }
    return 0;
}