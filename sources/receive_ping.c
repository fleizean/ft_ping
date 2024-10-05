#include "../includes/ft_ping.h"
#include <sys/time.h>

int receive_ping(int sock, char *packet, int packet_size)
{
    struct sockaddr_in r_addr;
    socklen_t addr_len = sizeof(r_addr);
    int bytes_received;
    struct timeval tv;

    // Zaman aşımını ayarla (örneğin, 1 saniye)
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    bytes_received = recvfrom(sock, packet, packet_size, 0, (struct sockaddr *)&r_addr, &addr_len);
    if (bytes_received <= 0)
    {
        perror("recvfrom");
        return -1;
    }

    struct iphdr *ip_hdr = (struct iphdr *)packet;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(packet + (ip_hdr->ihl * 4));

    if (icmp_hdr->type == ICMP_ECHOREPLY && icmp_hdr->un.echo.id == getpid())
    {
        return bytes_received;
    }

    return -1;
}