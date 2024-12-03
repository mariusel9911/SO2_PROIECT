#ifndef __IP_SEARCH_V1__
#define __IP_SEARCH_V1__

#include <stdint.h>

typedef struct connection_t {

    char src_ip[16];
    char dst_ip[16];
    char protocol[8];

    uint16_t sport;
    uint16_t dport;


}connection_t;

int get_tcp_connections(connection_t *connections, int max_connections);
int get_udp_connections(connection_t *connections, int max_connections);
void hex_to_ip(int hex_ip_addr, char *str_addr);

#endif