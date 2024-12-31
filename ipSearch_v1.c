#include "ipSearch_v1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <dirent.h>

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

        sscanf(line, "%*d: %lx:%hx %lx:%hx %*x %*x %*x %*x %*d %*d %llu",
               &local_ip, &connections[count].sport, &remote_ip, &connections[count].dport, &connections[count].bytes);

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

        sscanf(line, "%*d: %lx:%hx %lx:%hx %*x %*x %*x %*x %*d %*d %llu",
               &local_ip, &connections[count].sport, &remote_ip, &connections[count].dport, &connections[count].bytes);

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

        sscanf(line, "%*d: %lx:%hx %lx:%hx %*x %*x %*x %*x %*d %*d %llu",
               &local_ip, &connections[count].sport, &remote_ip, &connections[count].dport, &connections[count].bytes);

        hex_to_ip(local_ip, connections[count].src_ip);
        hex_to_ip(remote_ip, connections[count].dst_ip);
        strncpy(connections[count].protocol,"ICMP", 5);

        count++;
    }

    fclose(fp);
    return count;
}

char **get_interfaces(int *found_interfaces, int *allocated_interfaces){

    // Alloc dynamic array of string interfaces array

    *found_interfaces = 0;
    uint16_t max_interfaces = 32;
    uint16_t max_interface_name = 64;

    char **str = (char **)malloc(max_interfaces * sizeof(char *));

    if (!str){
        fprintf(stderr, "ouf of mem\n");
        return NULL;
    }

    for(int i = 0; i < max_interfaces; i++){
        str[i] = (char *)malloc(max_interface_name * sizeof(char));

        if (!str[i]){
            fprintf(stderr, "ouf of mem\n");
            for(int j = 0; j < i; j++){
                free(str[j]);
            }
            free(str);
            return NULL;
        }

    }

    // Search for interfaces in /sys/class/net

    char interfaces_path[] = "/sys/class/net";

    DIR *dir = opendir(interfaces_path);

    if (!dir){
        for(int i = 0; i < max_interfaces; i++) free(str[i]);
        free(str);
        return NULL;
    }

    struct dirent *entry;

    while((entry = readdir(dir)) != NULL){

        if (*found_interfaces == max_interfaces){
            max_interfaces = max_interfaces * 2;

            char **tmp = (char **)realloc(str, max_interfaces * sizeof(char *));

            if(!tmp){
                for(int i = 0; i < *found_interfaces; i++) free(str[i]);
                free(str);
                closedir(dir);
                return NULL;
            }

            str = tmp;
        }

        if (strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0 ) continue;

        strncpy(str[(*found_interfaces)++],entry->d_name, max_interface_name - 1);
    }

    *allocated_interfaces = max_interfaces;
    closedir(dir);

    return str;
}

interface_stat_t *get_interface_statistics(char *interface_name){

    static interface_stat_t statistics;

    statistics.rx_bytes = 0;
    statistics.tx_bytes = 0;

    char interface_rx_bytes[128];
    char interface_tx_bytes[128];

    snprintf(interface_rx_bytes, sizeof(interface_rx_bytes)-1, "/sys/class/net/%s/statistics/rx_bytes", interface_name);
    snprintf(interface_tx_bytes, sizeof(interface_tx_bytes)-1, "/sys/class/net/%s/statistics/tx_bytes", interface_name);

    FILE *rx_fp = fopen(interface_rx_bytes, "r");
    if (!rx_fp) {
        fprintf(stderr, "Error: Unable to open %s\n", interface_rx_bytes);
        return NULL;
    }

    FILE *tx_fp = fopen(interface_tx_bytes, "r");
    if (!tx_fp) {
        fprintf(stderr, "Error: Unable to open %s\n", interface_tx_bytes);
        fclose(rx_fp);
        return NULL;
    }

    if (fscanf(rx_fp, "%llu", &statistics.rx_bytes) != 1) {
        fclose(rx_fp);
        fclose(tx_fp);
        return NULL;
    }

    if (fscanf(tx_fp, "%llu", &statistics.tx_bytes) != 1) {
        fclose(rx_fp);
        fclose(tx_fp);
        return NULL;
    }

    fclose(rx_fp);
    fclose(tx_fp);

    return &statistics;   
}


interface_stat_t calculate_interface_bytes(interface_stat_t *interface, interface_stat_t *last_interface){

    interface_stat_t result;
    memset(&result, 0, sizeof(interface_stat_t)); 
    
    if (interface->rx_bytes - last_interface->rx_bytes > 0){
        result.rx_bytes = interface->rx_bytes - last_interface->rx_bytes;
    }

    if (interface->tx_bytes - last_interface->tx_bytes > 0){
        result.tx_bytes = interface->tx_bytes - last_interface->tx_bytes;
    }

    result.total = result.rx_bytes + result.tx_bytes;
    return result;

}

interface_rate calculate_interface_rate(interface_stat_t *interface, interface_stat_t *last_interface, double interval){

    interface_rate result;
    memset(&result, 0, sizeof(interface_rate));

    unsigned long long rx_diff = interface->rx_bytes - last_interface->rx_bytes;
    unsigned long long tx_diff = interface->tx_bytes - last_interface->tx_bytes;

    if (rx_diff > 0) {
        result.rx_rate = ((double)rx_diff * 8.0) / interval;
    } else {
        result.rx_rate = 0.0;
    }

    if (tx_diff > 0) {
        result.tx_rate = ((double)tx_diff * 8.0) / interval;
    } else {
        result.tx_rate = 0.0;
    }

    return result;
}

