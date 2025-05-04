#include "../includes/ft_ping.h"
#include <sys/time.h>

int receive_ping(int sock, char *packet, int packet_size, bool *is_verbose_error)
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
    // This debug print can be commented out when everything is working
    // printf("%d bytes received\n", bytes_received);

    struct iphdr *ip_hdr = (struct iphdr *)packet;
    struct icmphdr *icmp_hdr = (struct icmphdr *)(packet + (ip_hdr->ihl * 4));

    if (icmp_hdr->type == ICMP_ECHOREPLY && icmp_hdr->un.echo.id == getpid())
    {
        return bytes_received;
    }
    else if (icmp_hdr->type == ICMP_DEST_UNREACH)
    {
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(r_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
        printf("%d bytes from gateway (%s): Destination Port Unreachable\n", bytes_received, ip_str);
        *is_verbose_error = true;
        return -1;
    }

    return -1;
}