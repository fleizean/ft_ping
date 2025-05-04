#include "../includes/ft_ping.h"

int get_gateway_mac(t_ping *ping)
{
    int sock;
    struct arpreq arpreq;
    struct sockaddr_in *sin;

    // ARP isteğini düzgün şekilde yapılandırmak için arpreq'i sıfırlayın
    memset(&arpreq, 0, sizeof(struct arpreq));

    // Socket oluştur
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Socket oluşturulamadı");
        return -1;
    }

    // Gateway IP adresini yapılandır
    sin = (struct sockaddr_in *)&arpreq.arp_pa;
    sin->sin_family = AF_INET;
    sin->sin_addr = ping->gateway_addr.sin_addr;

    strncpy(arpreq.arp_dev, ping->interface_name, IFNAMSIZ - 1);

    // ARP isteği yap
    if (ioctl(sock, SIOCGARP, &arpreq) == -1)
    {
        perror("ARP isteği başarısız");
        close(sock);
        return -2;
    }

    close(sock);

    // MAC adresini al
    memcpy(ping->gateway_mac, arpreq.arp_ha.sa_data, 6);

    // printf("Gateway MAC Adresi: %02x:%02x:%02x:%02x:%02x:%02x\n",
    //        ping->gateway_mac[0], ping->gateway_mac[1], ping->gateway_mac[2], 
    //        ping->gateway_mac[3], ping->gateway_mac[4], ping->gateway_mac[5]);

    return 0;
}