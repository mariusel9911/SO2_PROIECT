#ifndef __IP_SEARCH_V1__
#define __IP_SEARCH_V1__

#include <stdint.h>

typedef struct connection_t {

    char src_ip[16];
    char dst_ip[16];
    char protocol[8];

    uint16_t sport;
    uint16_t dport;

    unsigned long long bytes;


}connection_t;

typedef struct interface_stat_t{

    unsigned long long rx_bytes;
    unsigned long long tx_bytes;
    unsigned long long total;

}interface_stat_t;

typedef struct interface_rate{

    double rx_rate;
    double tx_rate;

}interface_rate;

int get_tcp_connections(connection_t *connections, int max_connections);
int get_udp_connections(connection_t *connections, int max_connections);
int get_icmp_connections(connection_t *connections, int max_connections);
void hex_to_ip(int hex_ip_addr, char *str_addr);
char **get_interfaces(int *found_interfaces, int *allocated_interfaces);
interface_stat_t *get_interface_statistics(char *interface_name);
interface_stat_t calculate_interface_bytes(interface_stat_t *interface, interface_stat_t *last_interface);
interface_rate calculate_interface_rate(interface_stat_t *interface, interface_stat_t *last_interface, double interval);

#endif