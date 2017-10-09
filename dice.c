/**
 * @file dice.c
 * @brief Draw the dice and also manage the launch of the dice
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2017-09-13
 */
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include "dice.h"

uint8_t __lastValue = 0;

uint8_t dice_newLaunch(void)
{
    int temp;
    if(__lastValue == 0) {
        srand(time(NULL));   // should only be called once
    }
    temp = rand();
    __lastValue =  (temp % 6) + 1;
    #ifdef TESTING
    __lastValue = 1;
    #endif
    return __lastValue;
}

uint8_t dice_getLastValue(void)
{
    return __lastValue;
}
