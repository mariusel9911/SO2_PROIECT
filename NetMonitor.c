#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "ui.h"
#include "ipSearch_v1.h"

#define MAX_CONNECTIONS 4096

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
    verify_terminal_size(height, width);
    
    // unsigned long tx = 0, rx = 0, total = 0;
    
    WINDOW *interfaces_win = newwin(height/2, width/2, height/4, width/4);
    char *selected_interface = interfaces_menu(interfaces_win);

    if (!selected_interface){
        mvprintw(height - 2, 2, "No interface selected. Exiting.");
        refresh();
        sleep(1);
        delwin(interfaces_win);
        endwin();
        return 0;
    }


    WINDOW *table_win = newwin(height - 7, 100, 2, 2);
    box(table_win, 0, 0);

    WINDOW *footer_win = newwin(5, 100, height - 5, 2);
    box(footer_win, 0, 0);
    refresh();
    getch();

    connection_t connections[MAX_CONNECTIONS];
    interface_stat_t *interface;

    interface_stat_t last_interface;
    interface_stat_t curr_interface;
    interface_stat_t result;

    int num_connections = 0, start_row = 0;
    int max_visible_rows = height - 10;
    int ch;


    struct timespec req;
    req.tv_sec = 0;    
    req.tv_nsec = 1e8L;

    interface = get_interface_statistics(selected_interface);
    last_interface.rx_bytes = interface->rx_bytes;
    last_interface.tx_bytes = interface->tx_bytes;

    while (!killed) {

        num_connections = get_tcp_connections(connections, MAX_CONNECTIONS);
        num_connections += get_udp_connections(&connections[num_connections], MAX_CONNECTIONS);
        num_connections += get_icmp_connections(&connections[num_connections], MAX_CONNECTIONS);
        
        interface = get_interface_statistics(selected_interface);
        curr_interface.rx_bytes = interface->rx_bytes;
        curr_interface.tx_bytes = interface->tx_bytes;

        result = calculate_interface_bytes(&curr_interface, &last_interface);
        curr_interface = last_interface;


        update_table(table_win, connections, num_connections, start_row);
        draw_footer(footer_win, result.rx_bytes, result.tx_bytes, result.total, selected_interface);

        memset(&connections, 0, MAX_CONNECTIONS * sizeof(connection_t));

        ch = getch();
        if (ch == 'q') break;
        if (ch == KEY_UP && start_row > 0) start_row--;
        if (ch == KEY_DOWN && start_row + max_visible_rows < num_connections) start_row++;

        nanosleep(&req, NULL);  
    }

    delwin(interfaces_win);
    delwin(table_win);
    delwin(footer_win);
    free(selected_interface);
    endwin();

    return 0;
}