#include "../includes/ft_ping.h"

t_ping *g_ping = NULL;

int main(int ac, char **av)
{
    t_ping ping;
    memset(&ping, 0, sizeof(t_ping));

    g_ping = &ping;

    parse_arguments(ac, av, &ping);

    if (resolve_hostname(&ping) == -1)
    {
        fprintf(stderr, "ft_ping: %s: Name or service not known\n", ping.dest_ip);
        exit(EXIT_FAILURE);
    }

    // Socket oluştur
    if (create_socket(&ping) == -1)
    {
        fprintf(stderr, "ft_ping: socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Arayüz bilgilerini al
    if (get_interface_info(&ping) == -1)
    {
        fprintf(stderr, "ft_ping: failed to get interface info\n");
        exit(EXIT_FAILURE);
    }

    // Gateway bilgilerini al
    if (get_gateway(&ping) == -1)
    {
        fprintf(stderr, "ft_ping: failed to get gateway\n");
        exit(EXIT_FAILURE);
    }

    // Gateway MAC adresini al
    if (get_gateway_mac(&ping) == -1)
    {
        fprintf(stderr, "ft_ping: failed to get gateway MAC address\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, signal_handler);
    // Ping işlemini başlat
    start_ping(&ping);
    
    

    return 0;
}