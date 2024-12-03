#ifndef __UI_H__
#define __UI_H__

#include <ncurses.h>
#include "ipSearch_v1.h"

void draw_table(WINDOW *win);
void update_table(WINDOW *win, connection_t *connections, int num_connections, int start_row);
void draw_footer(WINDOW *footer_win, int tx, int rx, int total);

#endif // UI_H
