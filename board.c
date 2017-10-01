/**
 * @file board.c
 * @brief Draw the board of the game
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2017-09-07
 */
#include <ncurses.h>
#include <string.h>

#include "dice.h"

#define VCELL_WIDTH (6)
#define VCELL_HEIGHT (4)
#define HCELL_WIDTH (10)
#define HCELL_HEIGHT (2)
#define DICE_HEIGHT (15)
#define DICE_WIDTH (30)
#define HOME_WIDTH (7*VCELL_HEIGHT)
#define HOME_HEIGHT (7*HCELL_HEIGHT)

#define HORIZONTAL_LIMIT_0 (43)
#define HORIZONTAL_LIMIT_1 (59)
#define HORIZONTAL_LIMIT_2 (9)
#define HORIZONTAL_LIMIT_3 (25)

typedef enum {
    CELLSHOME,
    CELLSEND
} T_KindOfCells;

typedef enum {
    INCREMENTION,
    DECREMENTION
} T_IncDirection;

#define CORNER1 41
#define CORNER2 42

#define CORNER3 58
#define CORNER4 59

#define CORNER5 7
#define CORNER6 8

#define CORNER7 24
#define CORNER8 25

char __strLog[1024];

void brd_drawDice(WINDOW *window);

void drawHorizontalCells(WINDOW **windows, uint16_t yPos, uint16_t xPos,
                         uint8_t iStart, uint8_t specialIndex, T_KindOfCells kind, uint8_t color,
                         T_IncDirection direction)
{
    uint8_t endOfIndex = (kind == CELLSEND) ? 8 : 7;
    for(int i = 0; i < endOfIndex; i++) {
        uint8_t index = i + iStart;
        if(kind == CELLSEND) {
            if(i == 0) {
                index = iStart;
            } else {
                index = (iStart / 18) * 10 + 70 + i;
            }
        }
        if(direction == INCREMENTION) {
            if(windows[index] == NULL) {
                windows[index] = newwin(VCELL_WIDTH, VCELL_HEIGHT,
                                        yPos, xPos + (i * VCELL_HEIGHT));
            }
        } else {
            if(windows[index] == NULL) {
                windows[index] = newwin(VCELL_WIDTH, VCELL_HEIGHT,
                                        yPos, xPos - (i * VCELL_HEIGHT));
            }
        }
        if( (kind == CELLSHOME) && (i == specialIndex)) {
            wbkgd(windows[index], COLOR_PAIR(color));
        } else if( kind == CELLSEND) {
            if(i > 0) {
                wbkgd(windows[index], COLOR_PAIR(color));
            } else {
                wbkgd(windows[index], COLOR_PAIR(6));
            }
        } else {
            wbkgd(windows[index], COLOR_PAIR(5));
        }
        box(windows[index], 0, 0);
        mvwprintw(windows[index], 0, 0, "%d", index + 1);
        wrefresh(windows[index]);
    }
}

void drawVerticalCells(WINDOW **windows, uint16_t yPos, uint16_t xPos,
                       uint8_t iStart, uint8_t specialIndex, T_KindOfCells kind, uint8_t color,
                       T_IncDirection direction)
{
    uint8_t endOfIndex = (kind == CELLSEND) ? 8 : 7;
    for(int i = 0; i < endOfIndex; i++) {
        uint8_t index = i + iStart;
        if(kind == CELLSEND) {
            if(i == 0) {
                index = iStart;
            } else {
                index = (iStart / 18) * 10 + 70 + i;
            }
        }
        if(direction == INCREMENTION) {
            if(windows[index] == NULL) {
                windows[index] = newwin(HCELL_HEIGHT, HCELL_WIDTH,
                                        i * HCELL_HEIGHT + yPos, xPos);
            }
        } else {
            if(windows[index] == NULL) {
                windows[index] = newwin(HCELL_HEIGHT, HCELL_WIDTH,
                                        yPos - i * HCELL_HEIGHT, xPos);
            }
        }
        if( (kind == CELLSHOME) && (i == specialIndex)) {
            wbkgd(windows[index], COLOR_PAIR(color));
        } else if( kind == CELLSEND) {
            if(i > 0) {
                wbkgd(windows[index], COLOR_PAIR(color));
            } else {
                wbkgd(windows[index], COLOR_PAIR(6));
            }
        } else {
            wbkgd(windows[index], COLOR_PAIR(5));
        }
        box(windows[index], 0, 0);
        mvwprintw(windows[index], 0, 0, "%d", index + 1);
        wrefresh(windows[index]);
    }
}

void drawInernalCell(WINDOW **windows, uint8_t cell, uint16_t yPos,
                     uint16_t xPos,
                     uint8_t kind)
{
    if(windows[cell] == NULL) {
        windows[cell] = newwin(VCELL_WIDTH / 2, HCELL_WIDTH,
                               yPos, xPos);
    }
    wattron(windows[cell], COLOR_PAIR(5));
    switch(kind) {
    case 0:
        mvwprintw(windows[cell], 0, 0, "           ");
        mvwprintw(windows[cell], 1, 0, "| \\ %d           ", cell + 1);
        mvwprintw(windows[cell], 2, 0, "|   \\____________");
        break;
    case 1:
        mvwprintw(windows[cell], 0, 0, "|   |");
        mvwprintw(windows[cell], 1, 0, "|%d |", cell + 1);
        mvwprintw(windows[cell], 2, 0, "|___|");
        break;
    case 2:
        mvwprintw(windows[cell], 0, 0, "|   |");
        mvwprintw(windows[cell], 1, 0, "|%d |", cell + 1);
        mvwprintw(windows[cell], 2, 0, "|   |");
        break;
    case 3:
        mvwprintw(windows[cell], 0, 0, "|    /     ");
        mvwprintw(windows[cell], 1, 0, "|  /%d        ", cell + 1);
        mvwprintw(windows[cell], 2, 0, "|/____________");
        break;
    case 4:
        mvwprintw(windows[cell], 0, 0, "|    \\       ");
        mvwprintw(windows[cell], 1, 0, "|%d    \\     ", cell + 1);
        mvwprintw(windows[cell], 2, 0, "|_______\\    ");
        break;
    case 5:
        mvwprintw(windows[cell], 0, 5, "|    ");
        mvwprintw(windows[cell], 1, 5, "| %d  ", cell + 1);
        mvwprintw(windows[cell], 2, 5, "|    ");
        break;
    case 6:
        mvwprintw(windows[cell], 0, 0, "|       /  ");
        mvwprintw(windows[cell], 1, 0, "|%d    /   ", cell + 1);
        mvwprintw(windows[cell], 2, 0, "|_____/   ");
        break;
    case 7:
        mvwprintw(windows[cell], 0, 5, "|    ");
        mvwprintw(windows[cell], 1, 5, "| %d ", cell + 1);
        mvwprintw(windows[cell], 2, 5, "|____");
        break;
    default:
        break;
    }
    wrefresh(windows[cell]);
}


void brd_drawHomes(WINDOW **windows)
{
    for(uint8_t i = 0; i < 4; i++) {
        uint16_t yPos = ((i == 0)
                         || (i == 3)) ? 0 : (HCELL_HEIGHT * 7) + (VCELL_WIDTH * 3);
        uint16_t xPos = (i / 2) == 0 ? 0 : (VCELL_HEIGHT * 7) + (HCELL_WIDTH * 3);
        if(windows[i] == NULL) {
            windows[i] = newwin(HOME_HEIGHT, HOME_WIDTH, yPos, xPos);
        }
        wbkgd(windows[i], COLOR_PAIR(i + 1));
        wrefresh(windows[i]);
    }
}

void brd_drawCells(WINDOW **windows)
{
    drawHorizontalCells(windows, 7 * HCELL_HEIGHT, 6 * VCELL_HEIGHT, 43, 2,
                        CELLSHOME, 6, DECREMENTION);
    drawHorizontalCells(windows, 7 * HCELL_HEIGHT + VCELL_WIDTH, 0, 50, 0,
                        CELLSEND, 2, INCREMENTION);
    drawHorizontalCells(windows, 7 * HCELL_HEIGHT + (2 * VCELL_WIDTH), 0, 51, 4,
                        CELLSHOME, 2, INCREMENTION);
    drawHorizontalCells(windows, 7 * HCELL_HEIGHT,
                        (13 * VCELL_HEIGHT) + (3 * HCELL_WIDTH), 17, 4,
                        CELLSHOME, 4, DECREMENTION);
    drawHorizontalCells(windows, 7 * HCELL_HEIGHT + VCELL_WIDTH,
                        (13 * VCELL_HEIGHT) + (3 * HCELL_WIDTH), 16, 0,
                        CELLSEND, 4, DECREMENTION);
    drawHorizontalCells(windows, 7 * HCELL_HEIGHT + (2 * VCELL_WIDTH),
                        (7 * VCELL_HEIGHT) + (3 * HCELL_WIDTH), 9, 2,
                        CELLSHOME, 6, INCREMENTION);
    drawVerticalCells(windows, 0, 7 * VCELL_HEIGHT, 34, 4,
                      CELLSHOME, 1, INCREMENTION);
    drawVerticalCells(windows, 0, 7 * VCELL_HEIGHT + HCELL_WIDTH, 33, 0,
                      CELLSEND, 1, INCREMENTION);
    drawVerticalCells(windows, 6 * HCELL_HEIGHT,
                      7 * VCELL_HEIGHT + (2 * HCELL_WIDTH), 26, 2,
                      CELLSHOME, 6, DECREMENTION);
    drawVerticalCells(windows, (7 * HCELL_HEIGHT) + (3 * VCELL_WIDTH),
                      7 * VCELL_HEIGHT, 60, 2,
                      CELLSHOME, 6, INCREMENTION);
    drawVerticalCells(windows, (13 * HCELL_HEIGHT) + (3 * VCELL_WIDTH),
                      7 * VCELL_HEIGHT + HCELL_WIDTH, 67, 0,
                      CELLSEND, 3, DECREMENTION);
    drawVerticalCells(windows, (13 * HCELL_HEIGHT) + (3 * VCELL_WIDTH),
                      7 * VCELL_HEIGHT + (2 * HCELL_WIDTH), 0, 4,
                      CELLSHOME, 3, DECREMENTION);
}

void brd_drawCenter(WINDOW **windows)
{
    drawInernalCell(windows, CORNER1, 7 * HCELL_HEIGHT, 7 * VCELL_HEIGHT, 0);
    drawInernalCell(windows, CORNER2, 7 * HCELL_HEIGHT + (VCELL_WIDTH / 2),
                    7 * VCELL_HEIGHT, 1);
    drawInernalCell(windows, CORNER3, 7 * HCELL_HEIGHT + (2 * VCELL_WIDTH),
                    7 * VCELL_HEIGHT, 2);
    drawInernalCell(windows, CORNER4, 7 * HCELL_HEIGHT + ((5 * VCELL_WIDTH) / 2),
                    7 * VCELL_HEIGHT, 3);
    drawInernalCell(windows, CORNER6, 7 * HCELL_HEIGHT + 2 * VCELL_WIDTH,
                    7 * VCELL_HEIGHT + 2 * HCELL_WIDTH, 5);
    drawInernalCell(windows, CORNER5, 7 * HCELL_HEIGHT + ((5 * VCELL_WIDTH) / 2),
                    7 * VCELL_HEIGHT + 2 * HCELL_WIDTH, 4);
    drawInernalCell(windows, CORNER7, 7 * HCELL_HEIGHT + VCELL_WIDTH / 2,
                    7 * VCELL_HEIGHT + 2 * HCELL_WIDTH, 7);
    drawInernalCell(windows, CORNER8, 7 * HCELL_HEIGHT,
                    7 * VCELL_HEIGHT + 2 * HCELL_WIDTH, 6);
}

void drawSpeciallToken(WINDOW **windows, uint8_t token, uint8_t color)
{
    uint8_t xPos[2];
    uint8_t yPos[2];
    switch(token) {
    case CORNER1:
        xPos[0] = 7;
        xPos[1] = 7;
        yPos[0] = 0;
        yPos[1] = 1;
        break;
    case CORNER2:
        xPos[0] = 2;
        xPos[1] = 3;
        yPos[0] = 0;
        yPos[1] = 0;
        break;
    case CORNER3:
        xPos[0] = 1;
        xPos[1] = 2;
        yPos[0] = 0;
        yPos[1] = 0;
        break;
    case CORNER4:
        xPos[0] = 7;
        xPos[1] = 7;
        yPos[0] = 0;
        yPos[1] = 1;
        break;
    case CORNER5:
        xPos[0] = 1;
        xPos[1] = 2;
        yPos[0] = 0;
        yPos[1] = 0;
        break;
    case CORNER6:
        xPos[0] = 6;
        xPos[1] = 7;
        yPos[0] = 0;
        yPos[1] = 0;
        break;
    case CORNER7:
        xPos[0] = 6;
        xPos[1] = 7;
        yPos[0] = 0;
        yPos[1] = 0;
        break;
    case CORNER8:
        xPos[0] = 1;
        xPos[1] = 2;
        yPos[0] = 0;
        yPos[1] = 0;
        break;
    }
    mvwprintw(windows[token], yPos[0], xPos[0], "#");
    mvwprintw(windows[token], yPos[1], xPos[1], "#");
}

void drawSpeciallDoubleToken(WINDOW **windows, uint8_t token, uint8_t *color)
{
    switch(token) {
    case CORNER1:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 6, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 1, 6, "##");
        break;
    case CORNER2:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 1, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 2, 1, "##");
        break;
    case CORNER3:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 1, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 2, 1, "##");
        break;
    case CORNER4:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 6, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 2, 6, "##");
        break;
    case CORNER5:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 2, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 2, 2, "##");
        break;
    case CORNER6:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 7, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 2, 7, "##");
        break;
    case CORNER7:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 7, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 2, 7, "##");
        break;
    case CORNER8:
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 2, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 2, 2, "##");
        break;
    }
}

void brd_drawToken(WINDOW **windows, uint8_t token, uint8_t color)
{
    wattron(windows[token], COLOR_PAIR(color));
    if( (token == CORNER1) || (token == CORNER2) || (token == CORNER3)
            || (token == CORNER4) || (token == CORNER5)
            || (token == CORNER6) || (token == CORNER7) || (token == CORNER8)) {
        drawSpeciallToken(windows, token, color);
    } else if((token > HORIZONTAL_LIMIT_0 && token < HORIZONTAL_LIMIT_1) ||
              (token > HORIZONTAL_LIMIT_2 && token < HORIZONTAL_LIMIT_3)) {
        mvwprintw(windows[token], 2, 1, "#");
        mvwprintw(windows[token], 3, 1, "#");
    } else {
        mvwprintw(windows[token], 0, 4, "##");
    }
    wrefresh(windows[token]);
}

void brd_drawDoubleToken(WINDOW **windows, uint8_t token, uint8_t *color)
{
    if( (token == CORNER1) || (token == CORNER2) || (token == CORNER3)
            || (token == CORNER4) || (token == CORNER5)
            || (token == CORNER6) || (token == CORNER7) || (token == CORNER8)) {
        drawSpeciallDoubleToken(windows, token, color);
    } else if((token > HORIZONTAL_LIMIT_0 && token < HORIZONTAL_LIMIT_1) ||
              (token > HORIZONTAL_LIMIT_2 && token < HORIZONTAL_LIMIT_3)) {
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 1, 2, "#");
        mvwprintw(windows[token], 2, 2, "#");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 3, 2, "#");
        mvwprintw(windows[token], 4, 2, "#");
    } else {
        wattron(windows[token], COLOR_PAIR(color[0]));
        mvwprintw(windows[token], 0, 3, "##");
        wattron(windows[token], COLOR_PAIR(color[1]));
        mvwprintw(windows[token], 0, 6, "##");
    }
    wrefresh(windows[token]);
}

void drawBigNumber(WINDOW *window, uint8_t number)
{
    box(window, 0, 0);
    switch(number) {
    case 0:
        mvwprintw(window, 4, 7, " ");
        mvwprintw(window, 4, 8, " ");
        mvwprintw(window, 5, 6, " ");
        mvwprintw(window, 5, 9, " ");
        mvwprintw(window, 6, 6, " ");
        mvwprintw(window, 6, 9, " ");
        mvwprintw(window, 7, 6, " ");
        mvwprintw(window, 7, 9, " ");
        mvwprintw(window, 8, 6, " ");
        mvwprintw(window, 8, 9, " ");
        mvwprintw(window, 9, 7, " ");
        mvwprintw(window, 9, 8, " ");
        break;
    case 1:
        mvwprintw(window, 4, 7, " ");
        mvwprintw(window, 4, 8, " ");
        mvwprintw(window, 5, 6, " ");
        mvwprintw(window, 5, 8, " ");
        mvwprintw(window, 6, 8, " ");
        mvwprintw(window, 7, 8, " ");
        mvwprintw(window, 8, 8, " ");
        mvwprintw(window, 9, 7, " ");
        mvwprintw(window, 9, 8, " ");
        mvwprintw(window, 9, 9, " ");
        break;
    case 2:
        mvwprintw(window, 4, 7, " ");
        mvwprintw(window, 4, 8, " ");
        mvwprintw(window, 5, 6, " ");
        mvwprintw(window, 5, 9, " ");
        mvwprintw(window, 6, 10, " ");
        mvwprintw(window, 7, 8, " ");
        mvwprintw(window, 7, 9, " ");
        mvwprintw(window, 8, 7, " ");
        mvwprintw(window, 9, 6, " ");
        mvwprintw(window, 9, 7, " ");
        mvwprintw(window, 9, 8, " ");
        mvwprintw(window, 9, 9, " ");
        mvwprintw(window, 9, 10, " ");
        break;
    case 3:
        mvwprintw(window, 4, 7, " ");
        mvwprintw(window, 4, 8, " ");
        mvwprintw(window, 4, 9, " ");
        mvwprintw(window, 5, 9, " ");
        mvwprintw(window, 6, 8, " ");
        mvwprintw(window, 6, 9, " ");
        mvwprintw(window, 7, 9, " ");
        mvwprintw(window, 8, 9, " ");
        mvwprintw(window, 9, 7, " ");
        mvwprintw(window, 9, 8, " ");
        mvwprintw(window, 9, 9, " ");
        break;
    case 4:
        mvwprintw(window, 4, 6, " ");
        mvwprintw(window, 4, 8, " ");
        mvwprintw(window, 5, 6, " ");
        mvwprintw(window, 5, 8, " ");
        mvwprintw(window, 6, 6, " ");
        mvwprintw(window, 6, 7, " ");
        mvwprintw(window, 6, 8, " ");
        mvwprintw(window, 7, 8, " ");
        mvwprintw(window, 8, 8, " ");
        mvwprintw(window, 9, 8, " ");
        break;
    default:
        mvwprintw(window, 5, 5, " ");
    }
}

void brd_drawTokentsAtHome(WINDOW **windows, uint8_t *numberOfTokens)
{
    for(uint8_t i = 0; i < 4; i++) {
        werase(windows[i]);
        wrefresh(windows[i]);
        drawBigNumber(windows[i], numberOfTokens[i]);
        wrefresh(windows[i]);
    }
}

void brd_drawBoardTools(WINDOW **windows)
{
    if(windows[0] == NULL) {
        windows[0] = newwin(DICE_HEIGHT, DICE_WIDTH,
                            0, (2 * HOME_WIDTH) + (3 * HCELL_WIDTH) + 3);
    }
    brd_drawDice(windows[0]);
}

void brd_addToLog(WINDOW *window, char *string)
{
    werase(window);
    strcat(__strLog, string);
    if(window == NULL) {
        window = newwin(2 * HOME_HEIGHT + 3 * VCELL_WIDTH - DICE_HEIGHT, DICE_WIDTH * 2,
                        DICE_HEIGHT,
                        HOME_WIDTH * 2 + 3 * HCELL_WIDTH + 3);
    }
    wbkgd(window, COLOR_PAIR(7));
    mvwprintw(window, 1, 1, __strLog);
    wrefresh(window);
    wgetch(window);
}

void brd_drawLog(WINDOW *window, char *string)
{
    strcpy(__strLog, string);
    if(window == NULL) {
        window = newwin(2 * HOME_HEIGHT + 3 * VCELL_WIDTH - DICE_HEIGHT, DICE_WIDTH * 2,
                        DICE_HEIGHT,
                        HOME_WIDTH * 2 + 3 * HCELL_WIDTH + 3);
    }
    wbkgd(window, COLOR_PAIR(7));
    mvwprintw(window, 1, 1, string);
    wrefresh(window);
}

void brd_drawCurrentUser(WINDOW **windows, uint8_t user)
{
    wattron(windows[user], COLOR_PAIR(6));
    mvwprintw(windows[user], 3, 12, "****");
    mvwprintw(windows[user], 4, 12, "****");
    wrefresh(windows[user]);
}

void drawDot(WINDOW *window, uint8_t posY, uint8_t posX)
{
    mvwprintw(window, posY, posX - 1, " ");
    mvwprintw(window, posY, posX, " ");
    mvwprintw(window, posY, posX + 1, " ");
    mvwprintw(window, posY, posX + 2, " ");
    mvwprintw(window, posY + 1, posX - 1, " ");
    mvwprintw(window, posY + 1, posX, " ");
    mvwprintw(window, posY + 1, posX + 1, " ");
    mvwprintw(window, posY + 1, posX + 2, " ");
}

void drawNumber(WINDOW *window, uint8_t number)
{
    switch(number) {
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

void brd_drawDice(WINDOW *window)
{
    werase(window);
    box(window, 0, 0);
    wattron(window, COLOR_PAIR(5));
    drawNumber(window, dice_getLastValue());
    wrefresh(window);
}
