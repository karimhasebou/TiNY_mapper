//// DEVCFG2
//#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
//#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
//#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
//
//// DEVCFG1
//// PRIPLL
//#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
//#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
//#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
//#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
//#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
//#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
//#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
//#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
//#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
//
//// DEVCFG0
//#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
//#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
//#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
//#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
//#pragma config CP = OFF                 // Code Protect (Protection Disabled)
//
//
//#include <plib.h>			// Peripheral Library
//
//#define	GstSystemClock()              (80000000ul)
//#define	GetPeripheralClock()          (GetSystemClock() / (1 << OSCCONbits.PBDIV))
//#define	GetInstructionClock()         (GetSystemClock())
//
//#define DESIRED_BAUDRATE              (9600)      //The desired BaudRate
//int main()
//{
//   UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
//     
//   return 0;
//}