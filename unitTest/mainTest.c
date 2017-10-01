/**
 * @file mainTest.c
 * @brief General unit test of the application
 * @author Adrian Alvarez Lopez
 * @version 1.0.0
 * @date 2017-09-19
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "CUnit/Basic.h"

uint8_t getValidMovements(uint8_t currentPlayer, uint8_t diceValue,
                          uint8_t *movements);
extern uint8_t numberOfTokensAtHome[];
extern uint8_t tokenPositions[][4];


/**
 * @brief The suite initialization function. Set all the tokens at home.
 *
 * @return Zero on success, non-zero otherwise
 */
int init_suite1(void)
{
    for(uint8_t i = 0; i < 4; i++) {
        numberOfTokensAtHome[i] = 4;
    }
    return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */

/**
 * @brief The suite cleanup function.
 *
 * @return Zero on success, non-zero in other case
 */
int clean_suite1(void)
{
    return 0;
}


/**
 * @brief Check differents scenarios and test that the number of movements
 * are correct.
 */
void testValidMovements(void)
{
    uint8_t movements[4];
    uint8_t nMovements = getValidMovements(0, 5, movements);
    CU_ASSERT(nMovements == 1);
    nMovements = getValidMovements(0, 1, movements);
    CU_ASSERT(nMovements == 0);
    tokenPositions[0][0] = 66;
    numberOfTokensAtHome[0] = 3;
    nMovements = getValidMovements(0, 6, movements);
    CU_ASSERT(nMovements == 1);
    CU_ASSERT( movements[0] < 6);
}

void testMoveAcrrosWall(void)
{
    CU_ASSERT(true);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
    CU_pSuite pSuite = NULL;
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }
    /* add a suite to the registry */
    pSuite = CU_add_suite("Core suite case", init_suite1, clean_suite1);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /* add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "Testig validation of movements",
                             testValidMovements)) ||
            (NULL == CU_add_test(pSuite, "Testing movments accross the WALL",
                                 testMoveAcrrosWall))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
