#include "../includes/ft_ping.h"
#include <netdb.h>

void start_ping(t_ping *ping)
{
    struct timeval start, end;
    char packet[PACKET_SIZE];
    int received;

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ping->dest_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    printf("PING %s (%s): %ld data bytes\n", ping->dest_ip, ip_str, PING_PKT_S - sizeof(struct icmphdr));

    // Initialize all statistics properly
    ping->stats.packets_received = 0;
    ping->stats.sent_packets = 0;
    ping->stats.rtt_min = 0;
    ping->stats.rtt_max = 0;
    ping->stats.rtt_total = 0;
    ping->stats.rtt_total_sq = 0.0;  // Make sure this is initialized to 0.0
    ping->stats.nb_errors = 0;

    // Programın başlangıcında start_time'a zaman ataması yap
    gettimeofday(&ping->stats.start_time, NULL);

    while (1)
    {
        // Paket oluştur
        create_ping_packet(packet, ping->sequence);

        // Zamanı al
        gettimeofday(&start, NULL);

        // Paket gönder
        if (!send_ping(ping->sock_fd, &ping->dest_addr, packet, PING_PKT_S))
        {
            ping->is_verbose_error = true;
        }

        // Paket al
        received = receive_ping(ping->sock_fd, packet, PING_PKT_S, &(ping->is_verbose_error));
        
        // Zamanı al
        gettimeofday(&end, NULL);

        if (received > 0)
        {
            // RTT hesapla
            double rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

            // İstatistikleri güncelle
            ping->stats.packets_received++;
            ping->stats.rtt_total += rtt;
            ping->stats.rtt_total_sq += (rtt * rtt);  // This is correct
            
            // For the first packet, set min to this rtt value
            if (ping->stats.packets_received == 1)
                ping->stats.rtt_min = rtt;
            // Otherwise, compare if it's smaller than current min
            else if (rtt < ping->stats.rtt_min)
                ping->stats.rtt_min = rtt;
                
            // Set max if larger than current max
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
            
            if (!ping->is_verbose)
            {
                printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                       received, ip, icmp_hdr->un.echo.sequence,
                       ip_hdr->ttl, rtt);
            }
            else if (ping->is_verbose && !ping->is_verbose_error)
            {
                // Print extended information in verbose mode
                printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                       received, ip, icmp_hdr->un.echo.sequence,
                       ip_hdr->ttl, rtt);
            }
            
            ping->is_verbose_error = false;  
        }

        ping->sequence++;
        ping->stats.sent_packets++;

        // Uyku
        usleep(PING_SLEEP_RATE);
    }
}