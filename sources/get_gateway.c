#include "../includes/ft_ping.h"

int get_gateway(t_ping *ping)
{
    FILE *fp;
    char line[256];
    char *gateway = NULL;

    // /proc/net/route dosyasını aç
    fp = fopen("/proc/net/route", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/net/route");
        return -1;
    }

    // Dosyayı satır satır oku
    while (fgets(line, sizeof(line), fp) != NULL) {
        char iface[IFNAMSIZ];
        unsigned long dest, gate;

        // İlk satırı atla (başlık satırı)
        if (sscanf(line, "%s\t%lx\t%lx", iface, &dest, &gate) == 3) {
            if (dest == 0) { // Default route
                struct in_addr addr;
                addr.s_addr = gate;
                gateway = inet_ntoa(addr);
                break;
            }
        }
    }

    fclose(fp);

    if (gateway == NULL) {
        return -1;
    }

    // IP adresini sockaddr_in yapısına çevir
    if (inet_pton(AF_INET, gateway, &(ping->gateway_addr.sin_addr)) <= 0) {
        perror("inet_pton");
        return -1;
    }

    return 0;
}