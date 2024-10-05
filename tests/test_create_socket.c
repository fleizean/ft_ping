#include <check.h>
#include "../includes/ft_ping.h"
#include "../includes/colors.h"

t_ping *g_ping = NULL;

START_TEST(test_create_socket_success)
{
    t_ping ping;
    memset(&ping, 0, sizeof(t_ping));
    int result = create_socket(&ping);
    ck_assert_int_eq(result, 0);
    ck_assert_int_ne(ping.sock_fd, -1);
    close(ping.sock_fd);
    printf(GREEN "TEST1: [OK]\n" RESET);
}
END_TEST

START_TEST(test_create_socket_failure)
{
    t_ping ping;
    memset(&ping, 0, sizeof(t_ping));
    // Invalid protocol to force failure
    int sock_fd = socket(AF_INET, SOCK_RAW, -1);
    ck_assert_int_lt(sock_fd, 0);
    printf(GREEN "TEST2: [OK]\n" RESET);
}
END_TEST

Suite *ping_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create(BLUE "ft_ping Unit Test" RESET);

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_socket_success);
    tcase_add_test(tc_core, test_create_socket_failure);
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

    s = ping_suite();
    sr = srunner_create(s);

    srunner_run_all_colored(sr);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}