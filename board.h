/**
 * @file board.h
 * @brief Header file of the board drawer
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2017-09-07
 */

#ifndef BOARD_HEADER
#define BOARD_HEADER

void drawHomes(WINDOW **windows);
void drawCells(WINDOW **windows);
void drawCenter(WINDOW **windows);
void drawToken(WINDOW **windows, uint8_t token, uint8_t color);
void drawDoubleToken(WINDOW **windows, uint8_t token, uint8_t *color);
void drawTokentsAtHome(WINDOW **windows, uint8_t *numberOfTokens);
void drawLog(WINDOW *window, char *string);
#endif
