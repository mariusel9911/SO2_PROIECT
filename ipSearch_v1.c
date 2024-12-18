#include "ipSearch_v1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LINE_LENGTH 512

void hex_to_ip(int hex_ip_addr, char *str_addr) {

    struct in_addr ip_addr;
    ip_addr.s_addr = hex_ip_addr;

    strncpy(str_addr, inet_ntoa(ip_addr), 15);
}

int get_tcp_connections(connection_t *connections, int max_connections) {

    FILE *fp = fopen("/proc/net/tcp", "r");
    if (fp == NULL) {
        perror("Unable to open /proc/net/tcp");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    fgets(line, sizeof(line) - 1, fp); 
    
    while (fgets(line, sizeof(line) - 1, fp) != NULL && count < max_connections) {

        unsigned long local_ip, remote_ip;

        sscanf(line, "%*d: %lx:%hx %lx:%hx %*x %*x %*x %*x %*d %*d",
               &local_ip, &connections[count].sport, &remote_ip, &connections[count].dport);

        hex_to_ip(local_ip, connections[count].src_ip);
        hex_to_ip(remote_ip, connections[count].dst_ip);
        strncpy(connections[count].protocol,"TCP",4);

        count++;
    }

    fclose(fp);
    return count;
}

int get_udp_connections(connection_t *connections, int max_connections) {

    FILE *fp = fopen("/proc/net/udp", "r");
    if (fp == NULL) {
        perror("Unable to open /proc/net/udp");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    fgets(line, sizeof(line) - 1, fp); 

    while (fgets(line, sizeof(line) - 1, fp) != NULL && count < max_connections) {
        unsigned long local_ip, remote_ip;

        sscanf(line, "%*d: %lx:%hx %lx:%hx %*x %*x %*x %*x %*d %*d",
               &local_ip, &connections[count].sport, &remote_ip, &connections[count].dport);

        hex_to_ip(local_ip, connections[count].src_ip);
        hex_to_ip(remote_ip, connections[count].dst_ip);
        strncpy(connections[count].protocol,"UDP",4);

        count++;
    }

    fclose(fp);
    return count;
}

int get_icmp_connections(connection_t *connections, int max_connections) {

    FILE *fp = fopen("/proc/net/icmp", "r");
    if (fp == NULL) {
        perror("Unable to open /proc/net/icmp");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    fgets(line, sizeof(line) - 1, fp); 
    
    while (fgets(line, sizeof(line) - 1, fp) != NULL && count < max_connections) {

        unsigned long local_ip, remote_ip;

        sscanf(line, "%*d: %lx:%hx %lx:%hx %*x %*x %*x %*x %*d %*d",
               &local_ip, &connections[count].sport, &remote_ip, &connections[count].dport);

        hex_to_ip(local_ip, connections[count].src_ip);
        hex_to_ip(remote_ip, connections[count].dst_ip);
        strncpy(connections[count].protocol,"ICMP", 5);

        count++;
    }

    fclose(fp);
    return count;
}
