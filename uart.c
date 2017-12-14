#include "uart.h"
#include "delay.h"
#include <plib.h>


#define	GetSystemClock()              (80000000ul)
#define	GetPeripheralClock()          (GetSystemClock() / (1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()         (GetSystemClock())
#define DESIRED_BAUDRATE              (115200)    

void WriteString(char *string) {
	
  while (*string != '\0'){
      while (!UARTTransmitterIsReady(UART1));
      UARTSendDataByte(UART1, *string);
      string++;
      delay(5000);
      while (!UARTTransmissionHasCompleted(UART1));
    }

}

void WriteStringNull(char *string) {
	
  do{
      while (!UARTTransmitterIsReady(UART1));
      UARTSendDataByte(UART1, *string);
      string++;
      delay(5000);
      while (!UARTTransmissionHasCompleted(UART1));
  } while (*string != '\0');

}


void sendMessage(char* msg){
    WriteString("AT+CIPSEND=12\r\n");
    delay(30000);
    WriteStringNull(msg);
}

void initUart(){
  // Configure the device for maximum performance but do not change the PBDIV
  // Given the options, this function will change the flash wait states, RAM
  // wait state and enable prefetch cache but will not change the PBDIV.
  // The PBDIV value is already set via the pragma FPBDIV option above..

  // Explorer-16 uses UART1 to connect to the PC.
  // This initialization assumes 36MHz Fpb clock. If it changes,
  // you will have to modify baud rate initializer.
  UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
  UARTSetFifoMode(UART1, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
  UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
  int actual = UARTSetDataRate(UART1, GetPeripheralClock(), DESIRED_BAUDRATE);
  UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

  // Configure UART1 RX Interrupt
  INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
  INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_3);
  INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_0);
  
  // Connect to AP
  WriteString(AT_CMD);
  delay(100e3);
  WriteString(DISCONNECT_CMD);
  delay(100e3);
  WriteString(CONNECT_CMD);
  delay(5e6);
  WriteString(START_CMD);
  delay(1000e3);
  WriteString(AT_CMD);
  delay(100e3);
  sendMessage("Hello World");
}

// UART 2 interrupt handler
// it is set at priority level 3 with software context saving
void __ISR(_UART1_VECTOR, IPL3SOFT) IntUart1Handler(void) {
  // Is this an RX interrupt?
  if (INTGetFlag(INT_SOURCE_UART_RX(UART1)))
    {
      // Echo what we just received.
      c[count++] = UARTGetDataByte(UART1);

      // Clear the RX interrupt Flag
      INTClearFlag(INT_SOURCE_UART_RX(UART1));
    }

  // We don't care about TX interrupt
  if ( INTGetFlag(INT_SOURCE_UART_TX(UART1)) )
    {
      INTClearFlag(INT_SOURCE_UART_TX(UART1));
    }
}


