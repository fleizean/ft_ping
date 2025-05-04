#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../includes/colors.h"
#include "../includes/ft_ping.h"

void help_message() 
{
    // ASCII sanatını ve yardım mesajını yazdır
    printf("%s", CYAN);
    printf("  _____  __            .__                \n");
    printf("_/ ____\\/  |_   ______ |__| ____    ____  \n");
    printf("\\   __\\   __\\  \\____ \\|  |/    \\  / ___\\ \n");
    printf(" |  |   |  |    |  |_> >  |   |  \\/ /_/  >\n");
    printf(" |__|   |__|____|   __/|__|___|  /\\___  / \n");
    printf("          /_____/__|           \\//_____/  \n");
    printf("%s\n", RESET);

    printf("%sUsage:%s\n", GREEN, RESET);
    printf("  %sft_ping%s [options] <destination>\n\n", BLUE, RESET);

    printf("%sOptions:%s\n", GREEN, RESET);
    printf("  %s-v%s        Enable verbose output\n", YELLOW, RESET);
    printf("  %s-h%s        Show this help message\n", YELLOW, RESET);

    printf("\n%sExample:%s\n", GREEN, RESET);
    printf("  %sft_ping%s -v google.com\n", BLUE, RESET);
}

/**
 * Custom square root function using binary search method
 * @param nb The number to calculate the square root of
 * @return The square root of nb, or -NAN if nb is negative
 */
double ft_sqrt(const double nb)
{
    const double accuracy = SQRT_NEWTON_ACCURACY;
    double lower;
    double upper;
    double sqrt;

    if (nb < 0)
        return (-NAN);
    if (nb == 0)
        return (0.0);
    if (nb < 1) {
        lower = nb;
        upper = 1;
    }
    else {
        lower = 1;
        upper = nb;
    }
    while ((upper - lower) > accuracy) {
        sqrt = (lower + upper) / 2;
        if (sqrt * sqrt > nb)
            upper = sqrt;
        else
            lower = sqrt;
    }
    return ((lower + upper) / 2);
}