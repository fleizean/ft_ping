#include "../includes/ft_ping.h"

void create_ping_packet(char *packet, int sequence)
{
    struct icmphdr *icmp_hdr = (struct icmphdr *)packet;

    icmp_hdr->type = ICMP_ECHO;
    icmp_hdr->code = 0;
    icmp_hdr->un.echo.id = getpid();
    icmp_hdr->un.echo.sequence = sequence;
    icmp_hdr->checksum = 0;
    memset(packet + ICMP_HDRLEN, 0, PING_PKT_S - ICMP_HDRLEN);

    icmp_hdr->checksum = calculate_checksum((unsigned short *)packet, PING_PKT_S);
}