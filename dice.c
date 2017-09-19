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

uint8_t newLaunch(void)
{
    int temp;
    if(__lastValue == 0){
        srand(time(NULL));   // should only be called once
    }
    temp = rand();
    __lastValue =  (temp % 6)+1;
    return __lastValue;
}

uint8_t getLastValue(void)
{
    return __lastValue;
}

void drawDot(WINDOW *window, uint8_t posY, uint8_t posX)
{
    mvwprintw(window, posY , posX-1, " ");
    mvwprintw(window, posY , posX, " ");
    mvwprintw(window, posY , posX+1, " ");
    mvwprintw(window, posY , posX+2, " ");
    mvwprintw(window, posY+1 , posX-1, " ");
    mvwprintw(window, posY+1 , posX, " ");
    mvwprintw(window, posY+1 , posX+1, " ");
    mvwprintw(window, posY+1 , posX+2, " ");
}

void drawNumber(WINDOW *window, uint8_t number)
{
    switch(number){
        case 1:
            drawDot(window, 6, 13);
            break;
        case 2:
            drawDot(window, 3, 6);
            drawDot(window, 9, 20);
            break;
        case 3:
            drawDot(window, 3, 6);
            drawDot(window, 6, 13);
            drawDot(window, 9, 20);
            break;
        case 5:
            drawDot(window, 6, 13);
        case 4:
            drawDot(window, 3, 6);
            drawDot(window, 3, 20);
            drawDot(window, 9, 6);
            drawDot(window, 9, 20);
            break;
        case 6:
            drawDot(window, 3, 6);
            drawDot(window, 6, 6);
            drawDot(window, 9, 6);
            drawDot(window, 3, 20);
            drawDot(window, 6, 20);
            drawDot(window, 9, 20);
            break;
        default:
            drawDot(window, 2, 13);
            drawDot(window, 4, 13);
            drawDot(window, 6, 13);
            drawDot(window, 10, 13);
            break;
    }
}

void drawDice(WINDOW* window)
{
    werase(window);
    box(window,0,0);
    wattron(window, COLOR_PAIR(5));
    drawNumber(window, __lastValue);
    wrefresh(window);
}
