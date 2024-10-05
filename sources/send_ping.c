#include "../includes/ft_ping.h"

void send_ping(int sock, struct sockaddr_in *dest_addr, char *packet, int packet_size)
{
    if (sendto(sock, packet, packet_size, 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) <= 0)
    {
        perror("sendto");
    }
}