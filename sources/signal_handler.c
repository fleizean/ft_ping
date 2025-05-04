#include "../includes/ft_ping.h"
#include <math.h>

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        if (g_ping != NULL)
        {
            struct timeval end_time;
            gettimeofday(&end_time, NULL);

            // Toplam geçen süreyi hesapla
            // İstatistikleri yazdır
            printf("\n--- %s ping statistics ---\n", g_ping->dest_ip);

            // Show errors if there are any
            if (g_ping->stats.nb_errors > 0)
                printf("%d packets transmitted, %d packets received, +%d errors, %.2f%% packet loss\n",
                       g_ping->sequence, g_ping->stats.packets_received, g_ping->stats.nb_errors,
                       ((g_ping->sequence - g_ping->stats.packets_received) / (double)g_ping->sequence) * 100.0
                       );
            else
                printf("%d packets transmitted, %d packets received, %.2f%% packet loss\n",
                       g_ping->sequence, g_ping->stats.packets_received,
                       ((g_ping->sequence - g_ping->stats.packets_received) / (double)g_ping->sequence) * 100.0
                       );
            
            // Only show RTT statistics if we received at least one packet
                        // In the section where you calculate variance and stddev
            if (g_ping->stats.packets_received > 0) {
                double min = g_ping->stats.rtt_min;
                double avg = g_ping->stats.rtt_total / (double)g_ping->stats.packets_received;
                double max = g_ping->stats.rtt_max;
                
                // Calculate variance and stddev correctly
                double mean_sq = g_ping->stats.rtt_total_sq / (double)g_ping->stats.packets_received;
                double avg_sq = avg * avg;
                double variance = mean_sq - avg_sq;
                
                // Ensure variance isn't negative due to floating point errors
                variance = (variance < 0) ? 0 : variance;
                double stddev = ft_sqrt(variance);
                
                // Print with proper precision using stddev
                printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
                      min, avg, max, stddev);
            }
            

            exit(0);
        }
    }
}