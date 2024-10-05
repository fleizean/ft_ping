#include "../includes/ft_ping.h"
#include <netdb.h>

void start_ping(t_ping *ping)
{
    struct timeval start, end;
    char packet[PACKET_SIZE];
    int received;

    // Programın başlangıcında start_time'a zaman ataması yap
    gettimeofday(&ping->stats.start_time, NULL);

    while (1)
    {
        // Paket oluştur
        create_ping_packet(packet, ping->sequence);

        // Zamanı al
        gettimeofday(&start, NULL);

        // Paket gönder
        send_ping(ping->sock_fd, &ping->dest_addr, packet, PING_PKT_S);

        // Paket al
        received = receive_ping(ping->sock_fd, packet, PING_PKT_S);

        // Zamanı al
        gettimeofday(&end, NULL);

        if (received > 0)
        {
            // RTT hesapla
            double rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

            // İstatistikleri güncelle
            ping->stats.packets_received++;
            ping->stats.rtt_total += rtt;
            if (rtt < ping->stats.rtt_min || ping->stats.packets_received == 1)
                ping->stats.rtt_min = rtt;
            if (rtt > ping->stats.rtt_max)
                ping->stats.rtt_max = rtt;

            // Hostname ve IP adresini al
            struct sockaddr_in sa;
            char host[NI_MAXHOST];
            char ip[INET_ADDRSTRLEN];

            sa.sin_family = AF_INET;
            sa.sin_addr = ping->dest_addr.sin_addr;
            inet_ntop(AF_INET, &sa.sin_addr, ip, sizeof(ip));
            getnameinfo((struct sockaddr *)&sa, sizeof(sa), host, sizeof(host), NULL, 0, 0);

            // Sonuçları yazdır
            struct iphdr *ip_hdr = (struct iphdr *)packet;
            struct icmphdr *icmp_hdr = (struct icmphdr *)(packet + (ip_hdr->ihl * 4));

            

            // Eğer -v bayrağı aktifse, daha ayrıntılı bilgi yazdır
            if (ping->is_verbose)
            {
                char host[NI_MAXHOST];
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &ping->dest_addr.sin_addr, ip, INET_ADDRSTRLEN);
                getnameinfo((struct sockaddr *)&ping->dest_addr, sizeof(ping->dest_addr), host, NI_MAXHOST, NULL, 0, 0);
                printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n",
                       received, host, ip, icmp_hdr->un.echo.sequence,
                       ip_hdr->ttl, rtt);
            }
            else {
                printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n",
                   received, host, ip, icmp_hdr->un.echo.sequence,
                   ip_hdr->ttl, rtt);
            }
        }

        ping->sequence++;
        ping->stats.sent_packets++;

        // Uyku
        usleep(PING_SLEEP_RATE);
    }
}