/**
 * @file core.c
 * @brief Core of the play. Get the decisions from the player and execute them.
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2017-09-13
 */
#include <ncurses.h>
#include <string.h>
#include "core.h"
#include "board.h"
#include "dice.h"

WINDOW *home[4];
WINDOW *cells[110];
WINDOW *tools[10];
WINDOW *logWdw;

#define NMAXWALLS 16
#define MAXCELLVALUE 68
#define BASEOFENDS 70
#define LENENDS 10

uint8_t __homePosition[4] = {38, 55, 4, 21};
uint8_t __finalsPosition[4] = {33, 50, 0, 16};

uint8_t tokenPositions[4][4] = {{HOMEVALUE, HOMEVALUE, HOMEVALUE, HOMEVALUE},
    {HOMEVALUE, HOMEVALUE, HOMEVALUE, HOMEVALUE},
    {HOMEVALUE, HOMEVALUE, HOMEVALUE, HOMEVALUE},
    {HOMEVALUE, HOMEVALUE, HOMEVALUE, HOMEVALUE}
};
uint8_t currentPlayer = 0;
uint8_t numberOfTokensAtHome[] = {0, 0, 0, 0};
uint8_t walls[NMAXWALLS] = { INVALIDVALUE, INVALIDVALUE, INVALIDVALUE, INVALIDVALUE,
                             INVALIDVALUE, INVALIDVALUE, INVALIDVALUE, INVALIDVALUE,
                             INVALIDVALUE, INVALIDVALUE, INVALIDVALUE, INVALIDVALUE,
                             INVALIDVALUE, INVALIDVALUE, INVALIDVALUE, INVALIDVALUE
                           };

void refreshBoard(void)
{
    uint8_t tokenPosition;
    uint8_t numberOfWalls = 0;
    uint8_t isDouble = false;
    uint8_t color[] = {0, 0};
    uint8_t copyTokenPositions[4][4];
    brd_cleanCells(cells, 110);
    brd_drawCells(cells);
    brd_drawCenter(cells);
    memcpy(copyTokenPositions, tokenPositions, sizeof(tokenPositions));
    memset(walls, INVALIDVALUE, sizeof(walls));
    for(uint8_t player = 0; player < 4; player++) {
        numberOfTokensAtHome[player] = 0;
        for(uint8_t i = 0; i < 4; i++) {
            isDouble = false;
            tokenPosition = copyTokenPositions[player][i];
            if(tokenPosition == HOMEVALUE) {
                numberOfTokensAtHome[player]++;
            } else if(tokenPosition != INVALIDVALUE) {
                for(uint8_t j = i + 1; j < 4; j++) {
                    if( tokenPosition == copyTokenPositions[player][j] ) {
                        isDouble = true;
                        copyTokenPositions[player][j] = INVALIDVALUE;
                        color[0] = player + 1;
                        color[1] = player + 1;
                    }
                }
                for(uint8_t nextPlayer = player + 1; nextPlayer < 4; nextPlayer++) {
                    for(uint8_t j = 0; j < 4; j++) {
                        if( tokenPosition == copyTokenPositions[nextPlayer][j] ) {
                            isDouble = true;
                            copyTokenPositions[nextPlayer][j] = INVALIDVALUE;
                            color[0] = player + 1;
                            color[1] = nextPlayer + 1;
                        }
                    }
                }
                if(isDouble) {
                    brd_drawDoubleToken(cells, tokenPosition, color);
                    walls[numberOfWalls] = tokenPosition;
                    numberOfWalls++;
                } else {
                    brd_drawToken(cells, tokenPosition, player + 1);
                }
            }
        }
    }
    brd_drawTokentsAtHome(home, numberOfTokensAtHome);
}

bool movementAccrossWall(uint8_t initialPosition, uint8_t finalPosition)
{
    bool result = false;
    if(initialPosition == HOMEVALUE) {
        brd_addToLog(logWdw, "Cheking movements when token is at home");
        for(uint8_t i; i < NMAXWALLS; i++) {
            if(walls[i] == finalPosition && walls[i] != INVALIDVALUE) {
                result = true;
            }
        }
    } else if(initialPosition <= __finalsPosition[currentPlayer] &&
              finalPosition >= __finalsPosition[currentPlayer]) {
        brd_addToLog(logWdw,
                     "Cheking movements when token goes over its final position");
        for(uint8_t i; i < NMAXWALLS; i++) {
            if(walls[i] <= __finalsPosition[currentPlayer] && walls[i] != INVALIDVALUE) {
                result = true;
            }
        }
    } else if(initialPosition < MAXCELLVALUE &&
              finalPosition > MAXCELLVALUE) {
        brd_addToLog(logWdw,
                     "Cheking movements when token goes over general final position");
        for(uint8_t i; i < NMAXWALLS; i++) {
            if((walls[i] <= finalPosition || walls[i] > initialPosition)
                    && walls[i] != INVALIDVALUE) {
                char info[100];
                sprintf(info, "wal number %d in position %d", i, walls[i]);
                brd_addToLog(logWdw, info);
                result = true;
            }
        }
    } else {
        brd_addToLog(logWdw, "Cheking movements ");
        for(uint8_t i; i < NMAXWALLS; i++) {
            if(walls[i] > initialPosition && walls[i] <= finalPosition
                    && walls[i] != INVALIDVALUE) {
                result = true;
            }
        }
    }
    return result;
}

uint8_t getValidMovements(uint8_t currentPlayer, uint8_t diceValue,
                          uint8_t *movements)
{
    uint8_t validMovements = 0;
    if(numberOfTokensAtHome[currentPlayer] != 0) {
        if((diceValue == 5)
                && !movementAccrossWall(HOMEVALUE, __homePosition[currentPlayer])) {
            validMovements = 1;
            movements[0] = numberOfTokensAtHome[currentPlayer] - 1;
        } else if(numberOfTokensAtHome[currentPlayer] != 4) {
            validMovements = 4 - numberOfTokensAtHome[currentPlayer];
            uint8_t index = 0;
            for(uint8_t i = numberOfTokensAtHome[currentPlayer]; i < 4; i++) {
                if(movementAccrossWall(tokenPositions[currentPlayer][i],
                                       tokenPositions[currentPlayer][i] + diceValue)) {
                    validMovements--;
                } else {
                    movements[index] = i;
                    index++;
                }
            }
        }
    } else {
        validMovements = 4 - numberOfTokensAtHome[currentPlayer];
        uint8_t index = 0;
        for(uint8_t i = numberOfTokensAtHome[currentPlayer]; i < 4; i++) {
            if(movementAccrossWall(tokenPositions[currentPlayer][i],
                                   tokenPositions[currentPlayer][i] + diceValue)) {
                validMovements--;
            } else {
                movements[index] = i;
                index++;
            }
        }
    }
    return validMovements;
}

uint8_t getTokenToMove(uint8_t numberOfMovements, uint8_t *validMovements,
                       WINDOW *logWdw)
{
    bool validKey = false;
    char ch;
    char string[100];
    sprintf(string, "Select token to move:\n");
    for(uint8_t i = 0; i < numberOfMovements; i++) {
        char tempStr[100];
        sprintf(tempStr, " * [key=%d] Token in position %d\n", i + 1,
                tokenPositions[currentPlayer][validMovements[i]] + 1);
        strcat(string, tempStr);
    }
    brd_drawLog(logWdw, string);
    refreshBoard();
    while(validKey != true) {
        ch = wgetch(cells[0]);
        if( (ch < '1') || (ch >= ('1' + numberOfMovements)) ) {
            char invalidStr[100];
            strcpy(invalidStr, string);
            strcat(invalidStr, "Invalid key, please select a correct token to move");
            brd_drawLog(logWdw, invalidStr);
            refreshBoard();
        } else {
            validKey = true;
        }
    }
    return (ch - '1');
}

void core_initialization(void)
{
    brd_drawHomes(home);
    brd_drawBoardTools(tools);
    brd_drawLog(logWdw, "Press q for exit, any key for launch dice");
    refreshBoard();
#ifdef TESTING
    /*Replacement of the position of the tokens, this is done to testing how
     * the board shows some specific situations
     */
    tokenPositions[0][3] = 41;
    tokenPositions[1][3] = 7;
#endif
}

T_actions core_getUserAction(void)
{
    brd_drawCurrentUser(home, currentPlayer);
    char actionKey = wgetch(home[0]);
    return (actionKey == 'q') ? USERACTION_EXIT : USERACTION_LAUNCH;
}

void core_executeUserAction(void)
{
    uint8_t numberOfMovements;
    uint8_t validMovements[4];
    uint8_t diceValue = dice_newLaunch();
    uint8_t tokenToMove = 0;
    char string[100];
    brd_drawBoardTools(tools);
    numberOfMovements = getValidMovements(currentPlayer, diceValue, validMovements);
    if((tokenPositions[currentPlayer][validMovements[0]] == HOMEVALUE) &&
            (numberOfMovements == 1)) {
        sprintf(string, "Token moving out of home. Press any key to continue");
        tokenPositions[currentPlayer][validMovements[0]] =
            __homePosition[currentPlayer];
    } else if(numberOfMovements > 0) {
        tokenToMove = getTokenToMove(numberOfMovements, validMovements, logWdw);
        if((tokenPositions[currentPlayer][validMovements[tokenToMove]] <
                __finalsPosition[currentPlayer]) &&
                ((tokenPositions[currentPlayer][validMovements[tokenToMove]] + diceValue) >
                 __finalsPosition[currentPlayer])) {
            tokenPositions[currentPlayer][validMovements[tokenToMove]] =
                ((tokenPositions[currentPlayer][validMovements[tokenToMove]] + diceValue)
                 % __finalsPosition[currentPlayer]) + ( BASEOFENDS + (LENENDS * currentPlayer));
        }
        tokenPositions[currentPlayer][validMovements[tokenToMove]] =
            tokenPositions[currentPlayer][validMovements[tokenToMove]]  + diceValue;
        tokenPositions[currentPlayer][validMovements[tokenToMove]] %= MAXCELLVALUE;
        sprintf(string, "Token moving. Press any key to continue");
    } else {
        sprintf(string, "Movement not possible. Press any key to continue");
    }
    brd_drawLog(logWdw, string);
    refreshBoard();
    wgetch(cells[0]);
    currentPlayer++;
    currentPlayer = currentPlayer % 4;
}
