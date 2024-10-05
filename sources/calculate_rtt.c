#include "../includes/ft_ping.h"

void calculate_rtt(struct timeval *start, struct timeval *end, t_stats *stats)
{
    suseconds_t rtt = (end->tv_sec - start->tv_sec) * 1000000 + (end->tv_usec - start->tv_usec);

    if (stats->packets_received == 0 || rtt < stats->rtt_min)
    {
        stats->rtt_min = rtt;
    }

    if (rtt > stats->rtt_max)
    {
        stats->rtt_max = rtt;
    }

    stats->rtt_total += rtt;
    stats->rtt_total_sq += rtt * rtt;
}