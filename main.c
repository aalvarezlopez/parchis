#include <ncurses.h>
#include "core.h"


int main()
{	
    initscr();
    curs_set(FALSE);
    noecho();

    start_color();

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_CYAN);

    initialization();
    while( getUserAction() != USERACTION_EXIT){
        executeUserAction();
    }

    endwin();			/* End curses mode		  */

    return 0;
}
