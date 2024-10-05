#include "../includes/ft_ping.h"

int get_interface_info(t_ping *ping)
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return -1;
    }

    // Arayüz bilgilerini al
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                return -1;
            }

            // IP adresini ve arayüz adını kaydet
            if (strcmp(ifa->ifa_name, "lo") != 0) { // "lo" loopback arayüzünü atla
                ping->interface_ip = strdup(host);
                ping->interface_name = strdup(ifa->ifa_name);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);

    // MAC adresini al
    struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    strncpy(ifr.ifr_name, ping->interface_name, IFNAMSIZ-1);
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        return -1;
    }

    close(fd);

    memcpy(ping->interface_mac, ifr.ifr_hwaddr.sa_data, 6);

    return 0;
}