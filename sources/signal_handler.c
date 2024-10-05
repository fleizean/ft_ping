#include "../includes/ft_ping.h"

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {

        if (g_ping != NULL)
        {
            struct timeval end_time;
            gettimeofday(&end_time, NULL);

            // Toplam geçen süreyi hesapla
            double total_time = (end_time.tv_sec - g_ping->stats.start_time.tv_sec) * 1000.0 + 
                                (end_time.tv_usec - g_ping->stats.start_time.tv_usec) / 1000.0;

            // İstatistikleri yazdır
            printf("\n--- %s ping statistics ---\n", g_ping->dest_ip);
            printf("%d packets transmitted, %d received, %.2f%% packet loss, time %.0fms\n",
                   g_ping->sequence, g_ping->stats.packets_received,
                   ((g_ping->sequence - g_ping->stats.packets_received) / (double)g_ping->sequence) * 100.0,
                   total_time);
            printf("rtt min/avg/max = %.3f/%.3f/%.3f ms\n",
                   (double)g_ping->stats.rtt_min, 
                   (double)g_ping->stats.rtt_total / g_ping->stats.packets_received, 
                   (double)g_ping->stats.rtt_max);
        }

        exit(0);
    }
}