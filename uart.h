/* 
 * File:   uart.h
 * Author: KARIM
 *
 * Created on December 13, 2017, 8:34 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif
    /*
  WriteString(AT_CMD);
  delay(300);
  WriteString(DISCONNECT_CMD);
  delay(300);
  WriteString(CONNECT_CMD);
  delay(300);
  WriteString(START_CMD);
  delay(300);
  WriteString(AT_CMD);
}
     */
    #define AT (0)
    #define CONNECT (3)
    #define DISCONNECT (2)
    #define START (4)
    #define CLIENT (1)
    #define AT_CMD command[AT]
    #define CONNECT_CMD command[CONNECT]
    #define START_CMD command[START]
    #define CLIENT_CMD command[CLIENT]
    #define DISCONNECT_CMD command[DISCONNECT]

    char *command[5] = {
        "AT\r\n",
        "AT+CWMODE=1\r\n",
        "AT+CWQAP\r\n",
        "AT+CWJAP=\"bkhmsi\",\"12345678\"\r\n",
        "AT+CIPSTART=\"TCP\",\"192.168.43.198\",60000\r\n"
    };
    int count = 0;
    char c[300];
    void initUart();
    void WriteString(char *);
//    void sendCMD(char *);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

