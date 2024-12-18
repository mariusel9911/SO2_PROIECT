#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "ui.h"
#include "ipSearch_v1.h"

#define MAX_CONNECTIONS 1024

uint8_t killed = 0;

void sa_sigTERM_handler(int numsig){
    killed = 1;
}

int main(){

    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = &sa_sigTERM_handler;

    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW *table_win = newwin(height - 7, 100, 2, 2);
    box(table_win, 0, 0);

    WINDOW *footer_win = newwin(5, 100, height - 5, 2);
    box(footer_win, 0, 0);

    connection_t connections[MAX_CONNECTIONS];

    int num_connections = 0, start_row = 0;
    int max_visible_rows = height - 10;
    int tx = 1000, rx = 800, total = 1800;
    int ch;

    struct timespec req;
    req.tv_sec = 0;    
    req.tv_nsec = 100000000L;

    while (!killed) {
        num_connections = get_tcp_connections(connections, MAX_CONNECTIONS);
        num_connections += get_udp_connections(&connections[num_connections], MAX_CONNECTIONS);
        num_connections += get_icmp_connections(&connections[num_connections], MAX_CONNECTIONS);
        update_table(table_win, connections, num_connections, start_row);
        draw_footer(footer_win, tx, rx, total);

        memset(&connections, 0, MAX_CONNECTIONS * sizeof(connection_t));

        ch = getch();
        if (ch == 'q') break;
        if (ch == KEY_UP && start_row > 0) start_row--;
        if (ch == KEY_DOWN && start_row + max_visible_rows < num_connections) start_row++;

        nanosleep(&req, NULL);  
    }

    delwin(table_win);
    delwin(footer_win);
    endwin();

    return 0;
}