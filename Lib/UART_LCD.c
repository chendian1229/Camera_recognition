#include "common.h"
#include "include.h"

/**************************************************************
 *
 * 功       能：LCD结束指令
 *
 **************************************************************/
void UART_End()
{
    uart_putchar (UART4,0xff);
    pit_delay(PIT2, 100);
    uart_putchar (UART4,0xff);
    pit_delay(PIT2, 100);
    uart_putchar (UART4,0xff);
    pit_delay(PIT2, 100);

}
//printf("t0.txt=%s",ftoa(s,2)) 
