/* 
 * File:   uart.h
 * Author: KARIM
 *
 * Created on December 13, 2017, 8:34 PM
 */

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif
   
    typedef unsigned long ul;
    void delay(unsigned int);
    void initDelay();
    volatile ul t_count = 0;
    
#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

