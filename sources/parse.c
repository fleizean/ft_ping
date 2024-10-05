#include "../includes/ft_ping.h"

void parse_arguments(int ac, char **av, t_ping *ping)
{
    int opt;

    if (ac < 2)
    {
        help_message();
        exit(EXIT_FAILURE);
    }
    
    while ((opt = getopt(ac, av, "v")) != -1)
    {
        switch (opt)
        {
            case 'v':
                ping->is_verbose = true;
                break;
            case '?':
                help_message();
                exit(EXIT_SUCCESS);
            default:
                help_message();
                exit(EXIT_FAILURE);
        }
    }

    ping->dest_ip = av[ac - 1];
}