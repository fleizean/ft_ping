#include "../includes/ft_ping.h"

int get_gateway(t_ping *ping)
{
    FILE *fp;
    char line[128];
    char *gateway = NULL;

    // ip route komutunu çalıştır ve çıktıyı oku
    fp = popen("ip route | grep default", "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return -1;
    }

    // Çıktıyı oku ve "via" kelimesinden sonraki IP adresini ayıkla
    while (fgets(line, sizeof(line)-1, fp) != NULL) {
        char *via_ptr = strstr(line, "via ");
        if (via_ptr) {
            gateway = strdup(via_ptr + 4);  // "via " 4 karakter uzunluğunda
            char *space_ptr = strchr(gateway, ' ');  // IP adresinin sonunu bul
            if (space_ptr) {
                *space_ptr = '\0';  // IP adresinin sonuna null karakter ekle
            }
            break;
        }
    }

    pclose(fp);

    if (gateway == NULL) {
        return -1;
    }

    // IP adresini sockaddr_in yapısına çevir
    if (inet_pton(AF_INET, gateway, &(ping->gateway_addr.sin_addr)) <= 0) {
        perror("inet_pton");
        free(gateway);
        return -1;
    }

    free(gateway);
    return 0;
}