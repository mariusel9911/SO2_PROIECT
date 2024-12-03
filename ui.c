#include "ui.h"

void draw_table(WINDOW *win) {
    mvwprintw(win, 1, 2, "IP Source");
    mvwprintw(win, 1, 32, "IP Destination");
    mvwprintw(win, 1, 62, "Bytes");
    mvwprintw(win, 1, 77, "Protocol");
    mvwprintw(win, 1, 92, "Rate");
    mvwhline(win, 2, 1, ACS_HLINE, 98);
}

void update_table(WINDOW *win, connection_t *connections, int num_connections, int start_row) {
    int visible_rows = getmaxy(win) - 4;
    werase(win);
    box(win, 0, 0);
    draw_table(win);

    for (int i = 0; i < visible_rows && (start_row + i) < num_connections; i++) {
        mvwprintw(win, 3 + i, 2, "%s:%hu", connections[start_row + i].src_ip, connections[start_row + i].sport);
        mvwprintw(win, 3 + i, 32, "%s:%hu", connections[start_row + i].dst_ip, connections[start_row + i].dport);
        // mvwprintw(win, 3 + i, 62, "%d", connections[start_row + i].bytes);
        mvwprintw(win, 3 + i, 77, "%s", connections[start_row + i].protocol);
        // mvwprintw(win, 3 + i, 92, "%.2f", connections[start_row + i].rate);
    }

    wrefresh(win);
}

void draw_footer(WINDOW *footer_win, int tx, int rx, int total) {
    werase(footer_win);
    box(footer_win, 0, 0);
    mvwprintw(footer_win, 1, 2, "TX: %d bytes", tx);
    mvwprintw(footer_win, 2, 2, "RX: %d bytes", rx);
    mvwprintw(footer_win, 3, 2, "Total: %d bytes", total);
    mvwprintw(footer_win, 3, 70, "PRESS Q TO EXIT...");
    wrefresh(footer_win);
}