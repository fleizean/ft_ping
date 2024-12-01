#include "../includes/ft_ping.h"

bool send_ping(int sock, struct sockaddr_in *dest_addr, char *packet, int packet_size)
{
    int bytes_sent = sendto(sock, packet, packet_size, 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr));
    if (bytes_sent <= 0)
    {
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(dest_addr->sin_addr), ip_str, INET_ADDRSTRLEN);
        fprintf(stderr, "%d bytes from gateway (%s): %s\n", packet_size, ip_str, strerror(errno));
        return false;
    }
    fprintf(stderr, "%d bytes from gateway (%s): %s\n", packet_size, strerror(errno));
    return true;
}