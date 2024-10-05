#include "../includes/ft_ping.h"

int create_socket(t_ping *ping)
{
    int sock_fd;

    // RAW soket oluştur
    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_fd < 0) {
        return -1;
    }

    // Soket zaman aşımı ayarları
    struct timeval timeout;
    timeout.tv_sec = RECV_TIMEOUT;
    timeout.tv_usec = 0;

    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        close(sock_fd);
        return -1;
    }

    ping->sock_fd = sock_fd;
    return 0;
}