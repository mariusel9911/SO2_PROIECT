#include "ui.h"
#include <stdlib.h>
#include <menu.h>
#include <string.h>
#include "ipSearch_v1.h" 


char *interfaces_menu(WINDOW *win) {

    char **interfaces = NULL;

    int total_allocated_interfaces = 0;
    int num_items = 0;

    interfaces = get_interfaces(&num_items, &total_allocated_interfaces);

    ITEM **items;
    MENU *menu;
    int ch = 0;

    items = (ITEM **)calloc(num_items + 1, sizeof(ITEM *));
    for (int i = 0; i < num_items; i++) {
        items[i] = new_item(interfaces[i], "");
    }
    items[num_items] = NULL;

    menu = new_menu((ITEM **)items);
    set_menu_win(menu, win);
    set_menu_sub(menu, derwin(win, 6, getmaxx(win) - 4, 3, 2));
    set_menu_mark(menu, " # ");
    box(win, 0, 0);
    mvwprintw(win, 0, 2, "Interfaces");
    keypad(win, TRUE);
    post_menu(menu);

    int win_bottom = getbegy(win) + getmaxy(win);
    int win_center_x = getbegx(win) + getmaxx(win) / 2;

    mvprintw(win_bottom, win_center_x - strlen("Q - Exit") / 2, "Q - Exit");
    refresh();

    char *selected_interface = NULL;

    while ((ch = wgetch(win)) != 'q') {
        switch (ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10: // Enter
                selected_interface = strdup(item_name(current_item(menu)));
                goto end_menu;
        }
        wrefresh(win);
    }

end_menu:

    unpost_menu(menu);
    free_menu(menu);

    for (int i = 0; i < num_items; i++) {
        free_item(items[i]);
    }
    free(items);

    for(int i = 0; i < total_allocated_interfaces; i++){
        free(interfaces[i]);
    }
    free(interfaces);
    return selected_interface;

}


void draw_table(WINDOW *win) {
    mvwprintw(win, 1, 2, "Local Address");
    mvwprintw(win, 1, 32, "Foreign Address");
    mvwprintw(win, 1, 62, "Bytes");
    mvwprintw(win, 1, 77, "Protocol");
    mvwprintw(win, 1, 92, "Rate");
    mvwhline(win, 2, 1, ACS_HLINE, 108);
}


void update_table(WINDOW *win, connection_t *connections, int num_connections, int start_row) {
    int visible_rows = getmaxy(win) - 4;
    werase(win);
    box(win, 0, 0);
    draw_table(win);

    for (int i = 0; i < visible_rows && (start_row + i) < num_connections; i++) {
        mvwprintw(win, 3 + i, 2, "%s:%hu", connections[start_row + i].src_ip, connections[start_row + i].sport);
        mvwprintw(win, 3 + i, 32, "%s:%hu", connections[start_row + i].dst_ip, connections[start_row + i].dport);
        mvwprintw(win, 3 + i, 62, "%d B", connections[start_row + i].bytes);
        // mvwprintw(win, 3 + i, 62, "n/a"); // del later
        mvwprintw(win, 3 + i, 77, "%s", connections[start_row + i].protocol);

        double rate = (double)connections[start_row + i].bytes * 8 * 10 / 1024;
        mvwprintw(win, 3 + i, 92, "%.2f Kbps", rate);
        // mvwprintw(win, 3 + i, 92, "n/a"); // del later
    }

    wrefresh(win);
}

void draw_footer(WINDOW *footer_win, unsigned long long rx_bytes, unsigned long long tx_bytes, unsigned long long total, const char *interface_name, const interface_rate rate) {

    char rx_str[32], tx_str[32], total_str[32];
    char rx_rate[32], tx_rate[32];

    static unsigned long long rx = 0;
    static unsigned long long tx = 0;
    static unsigned long long Total = 0;

    rx+=rx_bytes;
    tx+=tx_bytes;
    Total+=total;

    format_bytes(rx, rx_str, sizeof(rx_str));
    format_bytes(tx, tx_str, sizeof(tx_str));
    format_bytes(Total, total_str, sizeof(total_str));

    format_bytes(rate.rx_rate, rx_rate, sizeof(rx_rate));
    format_bytes(rate.tx_rate, tx_rate, sizeof(tx_rate));

    werase(footer_win);
    box(footer_win, 0, 0);

    mvwprintw(footer_win, 1, 2, "RX: %s", rx_str);
    mvwprintw(footer_win, 2, 2, "TX: %s", tx_str);
    mvwprintw(footer_win, 3, 2, "Total: %s", total_str);

    mvwprintw(footer_win, 1, 32, "%s/s", rx_rate); // Rx rate
    mvwprintw(footer_win, 2, 32, "%s/s", tx_rate); // Rx rate

    mvwprintw(footer_win, 1, 70 ,"Interface:%s", interface_name);
    mvwprintw(footer_win, 3, 70, "PRESS Q TO EXIT...");

    wrefresh(footer_win);
}

void verify_terminal_size(int height, int width){

    if (height < 30 || width < 30) {
    endwin();
    fprintf(stderr, "Terminal size too small. Resize and try again.\n");
    exit(EXIT_FAILURE);
    }

}

void format_bytes(unsigned long long bytes, char *output, size_t output_size) {
    const char *units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "YB", "ZB"};
    int unit_index = 0;
    long double scaled_value = (long double)bytes;

    while (scaled_value >= 1024 && unit_index < 9) {
        scaled_value /= 1024.0;
        unit_index++;
    }

    snprintf(output, output_size, "%.2Lf %s", scaled_value, units[unit_index]);
}
