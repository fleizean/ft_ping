#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_arp.h>
#include <sys/select.h>
#include <netdb.h>
#include <stdbool.h> // bool türü için ekleme
#include <math.h>

#define PACKET_SIZE 4096
#define PING_SLEEP_RATE 1000000
#define RECV_TIMEOUT 1

// Define the ICMP header structure
#define SQRT_NEWTON_ACCURACY 0.0001

// ICMP header len for echo req
#define ICMP_HDRLEN 8

// Define the Packet Constants
// ping packet size
#define PING_PKT_S 64

// Automatic port number
#define PORT_NO 0

typedef struct s_stats {
    suseconds_t rtt_min;
    suseconds_t rtt_max;
    suseconds_t rtt_total;
    double rtt_total_sq;
    struct timeval start_time;
    uint packets_received;
    uint nb_errors;
    uint sent_packets;
    double rtt;
} t_stats;

typedef struct s_ping {
    struct sockaddr_in dest_addr;
    char *dest_ip;
    int sock_fd; // Daha açıklayıcı isim
    uint16_t sequence;
    bool is_verbose;
    char *interface_ip;
    char *interface_name;
    uint8_t interface_mac[6];
    struct sockaddr_in gateway_addr;
    uint8_t gateway_mac[6];
    t_stats stats;
    t_stats *result;
    bool is_verbose_error;
} t_ping;

// Global değişken
extern t_ping *g_ping;

// Function prototypes
uint16_t calculate_checksum(void *b, int len);
void create_ping_packet(char *packet, int sequence);
bool send_ping(int sock, struct sockaddr_in *dest_addr, char *packet, int packet_size);
int receive_ping(int sock, char *packet, int packet_size, bool *is_verbose_error);
void handle_signal(int sig);
void calculate_rtt(struct timeval *start, struct timeval *end, t_stats *stats);
void parse_arguments(int ac, char **av, t_ping *ping);
int resolve_hostname(t_ping *ping);
int create_socket(t_ping *ping);
int get_interface_info(t_ping *ping);
int get_gateway(t_ping *ping);
int get_gateway_mac(t_ping *ping);
void start_ping(t_ping *ping);
void signal_handler(int sig);
double	ft_sqrt(const double nb);
void help_message(void);

#endif