#include <plib.h>
#include "delay.h"

// x: multiples of 5 us
void initDelay(){
	OpenTimer1(T1_ON|T1_PS_1_1, 400); // initDelay
}

void delay(int x){
    unsigned int count = t_count + x;
    while(t_count != count);
}