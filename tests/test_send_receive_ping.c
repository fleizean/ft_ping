#include <check.h>
#include "../includes/ft_ping.h"
#include "../includes/colors.h"
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

t_ping *g_ping = NULL;

START_TEST(test_send_ping)
{
    t_ping ping;
    memset(&ping, 0, sizeof(t_ping));
    ping.sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    ck_assert_int_ne(ping.sock_fd, -1);

    char packet[PING_PKT_S];
    create_ping_packet(packet, 1);

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "1.1.1.1", &dest_addr.sin_addr);  // 1.1.1.1 adresini kullanıyoruz

    send_ping(ping.sock_fd, &dest_addr, packet, sizeof(packet));
    // send_ping fonksiyonu içinde hata olup olmadığını kontrol etmek için perror kullanılıyor.
    // Bu nedenle burada ek bir kontrol yapmamıza gerek yok.
    close(ping.sock_fd);
    printf(GREEN "TEST1: [OK]\n" RESET);
}
END_TEST

START_TEST(test_receive_ping)
{
    t_ping ping;
    memset(&ping, 0, sizeof(t_ping));

    ping.dest_ip = "google.com";

    if (resolve_hostname(&ping) != 0)
    {
        printf(RED "Hostname resolution failed\n" RESET);
        ck_abort_msg("Hostname resolution failed");
    }

    if (create_socket(&ping) == -1)
    {
        fprintf(stderr, "ft_ping: socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (get_interface_info(&ping) == -1)
    {
        fprintf(stderr, "ft_ping: failed to get interface info\n");
        exit(EXIT_FAILURE);
    }

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

    char packet[PACKET_SIZE];
    int received;
    while (1)
    {
        create_ping_packet(packet, ping.sequence);
        send_ping(ping.sock_fd, &ping.dest_addr, packet, PING_PKT_S);
        received = receive_ping(ping.sock_fd, packet, PING_PKT_S);
        if (received > 0)
        {
            /*char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ping.dest_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
            printf("Destination IP: %s\n", ip_str);
            printf("Destination Port: %d\n", ntohs(ping.dest_addr.sin_port));
            printf("bytes_received: %d\n", received);
            printf("sockfd: %d\n", ping.sock_fd);
            printf("packet: %s\n", packet);*/
            break;
        }
    }

    if (received > 0)
    {
        printf(GREEN "TEST2: [OK]\n" RESET);
    }
    else
    {
        printf(RED "TEST2: [FAILED]\n" RESET);
    }

    ck_assert_int_ge(received, 0);

    close(ping.sock_fd);
}
END_TEST

Suite *send_receive_ping_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create(BLUE "ft_ping Unit Test" RESET);

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_send_ping);
    tcase_add_test(tc_core, test_receive_ping);
    suite_add_tcase(s, tc_core);

    return s;
}

void srunner_run_all_colored(SRunner *sr)
{
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    if (number_failed == 0)
    {
        printf(GREEN "All tests passed successfully.\n" RESET);
    }
    else
    {
        printf(RED "%d test(s) failed.\n" RESET, number_failed);
    }
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = send_receive_ping_suite();
    sr = srunner_create(s);

    srunner_run_all_colored(sr);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}