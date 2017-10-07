/**
 * @file board.h
 * @brief Header file of the board drawer
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2017-09-07
 */

#ifndef BOARD_HEADER
#define BOARD_HEADER

void brd_drawHomes(WINDOW **windows);
void brd_drawCells(WINDOW **windows);
void brd_drawCenter(WINDOW **windows);
void brd_drawToken(WINDOW **windows, uint8_t token, uint8_t color);
void brd_drawDoubleToken(WINDOW **windows, uint8_t token, uint8_t *color);
void brd_drawTokentsAtHome(WINDOW **windows, uint8_t *numberOfTokens);
void brd_drawLog(WINDOW *window, char *string);
void brd_drawCurrentUser(WINDOW **windows, uint8_t user);
void brd_addToLog(WINDOW *window, char *string);
void brd_drawDice(WINDOW* window);
void brd_drawBoardTools(WINDOW **windows);
void brd_cleanCells(WINDOW **cells, uint8_t numberOfCells);
#endif
