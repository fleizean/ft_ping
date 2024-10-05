#include "../includes/ft_ping.h"

int resolve_hostname(t_ping *ping)
{
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *rp;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;    /* Allow IPv4 */
    hints.ai_socktype = SOCK_RAW; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_ICMP; /* 0: any protocol */

    ping->sock_fd = 0;

    s = getaddrinfo(ping->dest_ip, NULL, &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        ping->dest_addr = *(struct sockaddr_in *)rp->ai_addr;
        break;
    }

    freeaddrinfo(result);

    return 0;
}