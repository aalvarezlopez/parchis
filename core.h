/**
 * @file core.h
 * @brief Core of the play. Get the decisions from the player and execute them.
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2017-09-13
 */

#ifndef CORE_HEADER
#define CORE_HEADER

typedef enum actions{
	USERACTION_EXIT,
	USERACTION_LAUNCH,
	USERACTION_MOVE1,
	USERACTION_MOVE2,
	USERACTION_MOVE3,
	USERACTION_MOVE4
}T_actions;

#define INVALIDVALUE 200
#define HOMEVALUE 201

void core_initialization(void);
T_actions core_getUserAction(void);
void core_executeUserAction(void);
#endif
