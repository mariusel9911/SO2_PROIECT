#ifndef __UI_H__
#define __UI_H__

#include <ncurses.h>
#include "ipSearch_v1.h"

void draw_table(WINDOW *win);
void update_table(WINDOW *win, connection_t *connections, int num_connections, int start_row);
void draw_footer(WINDOW *footer_win, unsigned long long rx_bytes, unsigned long long tx_bytes, unsigned long long total, const char *interface_name);
void verify_terminal_size(int height, int width);
char *interfaces_menu(WINDOW *win);
void format_bytes(unsigned long long bytes, char *output, size_t output_size);

#endif // UI_H
